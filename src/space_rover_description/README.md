# `space_rover_description`

ROS 2 robot description package for the Curiosity-inspired rover model, including RViz visualization assets and Gazebo Sim world resources.

## Purpose

This package contains the visual, structural, and simulation-facing parts of the project. It is currently the most complete package in the repository and is the main reason the workspace can already be demonstrated in RViz and Gazebo Sim.

## Current Status

This package is usable, but the overall project is still in progress.

The description stack contains the main rover model, simulation resources, and visualization setup used throughout the workspace.

## What This Package Contains

- modular `URDF` and `Xacro` rover model files
- mesh and texture assets
- Gazebo-related files and world resources
- RViz configurations
- ROS 2 launch files
- older ROS 1 launch files kept for reference

## Key Files

- Main rover entry file:
  - `urdf/curiosity_rover.urdf.xacro`
- ROS 2 launch files:
  - `launch/display.launch.py`
  - `launch/gazebo.launch.py`
- RViz configuration:
  - `rviz/view_rover.rviz`
- Gazebo worlds:
  - `worlds/empty.sdf`
  - `worlds/simple.world`
  - `worlds/mars_curiosity.world`

## Xacro / URDF Modules

The rover model is split into reusable parts:

- `urdf/chassis.xacro`
- `urdf/wheel.xacro`
- `urdf/left_wheel_group.xacro`
- `urdf/right_wheel_group.xacro`
- `urdf/arm.xacro`
- `urdf/sensor_mast.xacro`
- `urdf/curiosity_mars_rover_properties.xacro`
- `urdf/macros.xacro`
- `urdf/curiosity_mars_rover.gazebo`

## Asset Folders

- `meshes/`
  Visual mesh resources and texture files for the rover
- `models/`
  Gazebo model assets such as rocks and path elements
- `worlds/`
  Simulation world definitions
- `rviz/`
  Stored RViz view configurations
- `config/`
  Configuration files related to control and simulation

## Runtime Dependencies

Dependencies currently declared in `package.xml`:

- `robot_state_publisher`
- `joint_state_publisher`
- `joint_state_publisher_gui`
- `xacro`
- `rviz2`
- `ros_gz_sim`
- `ros_gz_bridge`

## Build

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
colcon build --packages-select space_rover_description
source install/setup.bash
```

## Launch In RViz

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 launch space_rover_description display.launch.py
```

This is useful for checking:

- model structure
- joint relationships
- frame visibility
- visual appearance of the rover

## Launch In Gazebo

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 launch space_rover_description gazebo.launch.py
```

This launch path is intended to:

- process the Xacro rover model
- publish `robot_description`
- start Gazebo through `ros_gz_sim`
- spawn the rover into the world
- bridge the simulation clock into ROS 2

The launch file accepts a `world` argument for selecting a world from `worlds/`.

Examples:

Launch with the default empty world:

```bash
ros2 launch space_rover_description gazebo.launch.py
```

Launch with the Mars world:

```bash
ros2 launch space_rover_description gazebo.launch.py world:=mars_curiosity.world
```

## World Resources

World files available in this package:

- `worlds/empty.sdf`
- `worlds/simple.world`
- `worlds/mars_curiosity.world`

Gazebo model assets used by the Mars world are stored in:

- `models/curiosity_path/`
- `models/rock/`

The Gazebo launch file sets `GZ_SIM_RESOURCE_PATH` so the world can resolve these local models.

## Simulation Notes

- The package is configured for Gazebo Sim / `ros_gz`, not Gazebo Classic.
- Gazebo Classic plugin references were removed from the rover model because they are not compatible with the current launch path.
- The `box_inertia` macro in `urdf/macros.xacro` uses a corrected `izz` formula so Gazebo accepts the link inertias.
- The mast and manipulator arm are currently fixed in place in simulation to avoid passive joints folding under gravity before controllers are added.
- `arm_02` uses a simple collision box in simulation because the original collision mesh caused Gazebo physics crashes.

## Validate The Model

Generate a URDF from the main Xacro file:

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
source install/setup.bash
xacro src/space_rover_description/urdf/curiosity_rover.urdf.xacro > /tmp/curiosity_rover.urdf
```

Validate it if `check_urdf` is available:

```bash
check_urdf /tmp/curiosity_rover.urdf
```

## ROS 1 Compatibility Note

The `ros1_launch/` directory is kept only as a legacy reference.

- Use `launch/*.launch.py` with ROS 2
- Do not run `ros1_launch/*.launch` with `ros2 launch`

## Package Role In The Repository

Inside this repository, `space_rover_description` provides the rover model, simulation assets, and launch setup that define the visual and structural core of the project.
