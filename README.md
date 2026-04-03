# Space Rover

ROS 2 workspace for a Curiosity-inspired space rover project using ROS 2 Jazzy and Gazebo Sim via `ros_gz`.

## Project Status

This project is still in progress.

The repository already contains a working ROS 2 workspace structure, a detailed rover description package, launch files for RViz and Gazebo, and supporting assets such as meshes and worlds. The runtime and autonomy side of the project is not finished yet, and some packages are still scaffolds for future development.

## Overview

The goal of this repository is to organize the rover project in a clean ROS 2 workspace so it can grow into a complete robotics project rather than remain only a robot model. Right now the strongest part of the repository is the description and simulation package.

Current focus areas in the repo:

- Modular rover model using `URDF` and `Xacro`
- Visualization in `RViz`
- Simulation launch setup for Gazebo / `ros_gz`
- Mesh, texture, and world assets collected in one package
- Workspace layout ready for additional control, navigation, and autonomy packages

## Repository Structure

```text
space_rover/
├── src/
│   ├── space_rover/
│   │   ├── CMakeLists.txt
│   │   ├── package.xml
│   │   └── README.md
│   ├── space_rover_description/
│   │   ├── config/
│   │   ├── launch/
│   │   ├── meshes/
│   │   ├── models/
│   │   ├── ros1_launch/
│   │   ├── rviz/
│   │   ├── scripts/
│   │   ├── urdf/
│   │   ├── worlds/
│   │   ├── CMakeLists.txt
│   │   ├── package.xml
│   │   └── README.md
│   └── README.md
├── build/
├── install/
├── log/
├── .gitignore
└── README.md
```

## Packages

### `space_rover`

This is the main project package intended for rover-side logic. At the moment it is mostly a clean package scaffold and a placeholder for future work such as:

- control nodes
- rover behavior logic
- navigation integration
- mission-level orchestration
- interfaces between simulation and higher-level software

### `space_rover_description`

This is the most complete package in the workspace. It contains:

- the main rover model entry file: `urdf/curiosity_rover.urdf.xacro`
- modular Xacro files for chassis, wheels, arm, mast, and grouped assemblies
- simulation resources for Gazebo
- RViz configurations
- world files
- mesh and texture assets
- legacy ROS 1 launch files kept for reference

## Project Scope

The workspace is structured around two ROS 2 packages:

- `space_rover`
- `space_rover_description`

The repository combines robot description, visualization, simulation resources, and the base structure for future rover software in one workspace.

## Main Files To Know

If someone opens this repository for the first time, these are the most useful files to start with:

- `src/space_rover_description/urdf/curiosity_rover.urdf.xacro`
- `src/space_rover_description/launch/display.launch.py`
- `src/space_rover_description/launch/gazebo.launch.py`
- `src/space_rover_description/rviz/view_rover.rviz`
- `src/space_rover_description/worlds/empty.sdf`
- `src/space_rover_description/package.xml`

## Requirements

Recommended environment:

- Ubuntu
- ROS 2 Jazzy
- `colcon`
- `xacro`
- `robot_state_publisher`
- `joint_state_publisher`
- `joint_state_publisher_gui`
- `rviz2`
- `ros_gz_sim`
- `ros_gz_bridge`

## Build The Workspace

From the workspace root:

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.bash
```

## Run The Rover In RViz

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 launch space_rover_description display.launch.py
```

This launch file starts:

- `robot_state_publisher`
- `joint_state_publisher_gui`
- `rviz2`

## Run The Rover In Gazebo

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 launch space_rover_description gazebo.launch.py
```

This launch file is intended to:

- process the rover Xacro file
- publish the robot description
- start Gazebo through `ros_gz_sim`
- spawn the rover entity
- bridge the simulation clock into ROS 2

The `gazebo.launch.py` file also accepts a `world` argument, so you can start the rover in a different world without editing code.

Launch the rover in the Mars world:

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 launch space_rover_description gazebo.launch.py world:=mars_curiosity.world
```

Available world files in `space_rover_description/worlds/`:

- `empty.sdf`
- `simple.world`
- `mars_curiosity.world`

## Gazebo Notes

- The simulation uses Gazebo Sim through `ros_gz_sim`, not Gazebo Classic.
- Older Gazebo Classic ROS plugins were removed from the rover model because they are not available in this setup.
- The front arm and camera mast are currently fixed in place in simulation so they do not collapse under gravity before controllers are added.
- If Gazebo prints `libEGL` warnings, those are graphics-driver warnings and are usually separate from the robot model itself.

## ROS 1 vs ROS 2 Notes

This repository contains both ROS 2 launch files and older ROS 1 launch files.

Use these for ROS 2:

- `src/space_rover_description/launch/*.launch.py`

Reference only, not for `ros2 launch`:

- `src/space_rover_description/ros1_launch/*.launch`

## Validation

To expand the main Xacro file into a URDF:

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
source install/setup.bash
xacro src/space_rover_description/urdf/curiosity_rover.urdf.xacro > /tmp/curiosity_rover.urdf
```

If `check_urdf` is installed, you can validate the generated file:

```bash
check_urdf /tmp/curiosity_rover.urdf
```

## Summary

This repository presents a ROS 2 rover workspace centered on a Curiosity-inspired robot model. It brings together modular Xacro-based robot description files, RViz visualization, Gazebo simulation resources, and a clean package layout for continued rover software development.
