import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    SetEnvironmentVariable,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():

    pkg_share = get_package_share_directory('space_rover_description')

    # --- Paths ---
    xacro_file = os.path.join(pkg_share, 'urdf', 'curiosity_rover.urdf.xacro')
    world_file = os.path.join(pkg_share, 'worlds', 'empty.sdf')

    # Gazebo resolves package:// → model:// URIs by searching GZ_SIM_RESOURCE_PATH
    # for a folder named "space_rover_description". Point to the parent of pkg_share
    # so that model://space_rover_description/meshes/... resolves correctly.
    gz_resource_path = SetEnvironmentVariable(
        name='GZ_SIM_RESOURCE_PATH',
        value=os.path.dirname(pkg_share),
    )

    # --- Launch arguments ---
    use_sim_time = LaunchConfiguration('use_sim_time')

    declare_use_sim_time = DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulation (Gazebo) clock',
    )

    # --- Process XACRO → URDF ---
    robot_description = Command(['xacro ', xacro_file])

    # --- Nodes ---
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{
            'robot_description': ParameterValue(robot_description, value_type=str),
            'use_sim_time': use_sim_time,
        }],
        output='screen',
    )

    joint_state_publisher = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        parameters=[{'use_sim_time': use_sim_time}],
        output='screen',
    )

    # --- Gazebo Harmonic (ros_gz_sim) ---
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(
                get_package_share_directory('ros_gz_sim'),
                'launch',
                'gz_sim.launch.py',
            )
        ),
        launch_arguments={
            'gz_args': f'-r {world_file}',
        }.items(),
    )

    # Spawn robot from /robot_description topic
    spawn_entity = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=[
            '-topic', 'robot_description',
            '-name', 'curiosity_rover',
            '-x', '0.0',
            '-y', '0.0',
            '-z', '0.5',
        ],
        output='screen',
    )

    # Bridge: forward /clock from Gazebo to ROS
    gz_bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        arguments=['/clock@rosgraph_msgs/msg/Clock[gz.msgs.Clock'],
        output='screen',
    )

    return LaunchDescription([
        gz_resource_path,
        declare_use_sim_time,
        robot_state_publisher,
        joint_state_publisher,
        gazebo,
        spawn_entity,
        gz_bridge,
    ])
