# space_rover_description

Curiosity-style rover description package for ROS 2.

## What This Package Contains

- Modular Xacro/URDF model for rover body, suspension, wheels, arm, and mast
- Mesh assets (`.dae`, `.stl`, textures)
- RViz configs
- Gazebo world and model assets
- ROS 2 launch files for display and simulation
- Legacy ROS1 launch files kept in `ros1_launch/` for reference

## Main Files

- Robot entry Xacro:
  - `urdf/curiosity_rover.urdf.xacro`
- ROS 2 launch files:
  - `launch/display.launch.py`
  - `launch/gazebo.launch.py`
- RViz config:
  - `rviz/view_rover.rviz`

## Included URDF Modules

- `urdf/chassis.xacro`
- `urdf/wheel.xacro`
- `urdf/left_wheel_group.xacro`
- `urdf/right_wheel_group.xacro`
- `urdf/arm.xacro`
- `urdf/sensor_mast.xacro`
- `urdf/curiosity_mars_rover_properties.xacro`
- `urdf/macros.xacro`
- `urdf/curiosity_mars_rover.gazebo`

## Dependencies

Runtime dependencies declared in `package.xml`:

- `robot_state_publisher`
- `joint_state_publisher`
- `joint_state_publisher_gui`
- `xacro`
- `rviz2`
- `ros_gz_sim`
- `ros_gz_bridge`

## Build

From workspace root:

```bash
source /opt/ros/jazzy/setup.bash
colcon build --packages-select space_rover_description
source install/setup.bash
```

## Run in RViz

```bash
ros2 launch space_rover_description display.launch.py
```

## Run in Gazebo

```bash
ros2 launch space_rover_description gazebo.launch.py
```

## Validate URDF/Xacro

```bash
source /opt/ros/jazzy/setup.bash
source install/setup.bash
xacro $(ros2 pkg prefix space_rover_description)/share/space_rover_description/urdf/curiosity_rover.urdf.xacro > /tmp/curiosity.urdf
check_urdf /tmp/curiosity.urdf
```

## Important ROS1 Compatibility Note

Legacy ROS1 `.launch` files are preserved under:

- `ros1_launch/`

These files are not for `ros2 launch`.
Use only `.launch.py` files in `launch/`.

## Assets Origin

This package includes model resources derived from a Curiosity rover description workflow and was integrated into this ROS 2 package structure.

## Repository Layout

This package is intended to live in the same repository as `space_rover`:

- repo: `space_rover`
- path: `src/space_rover_description/`
