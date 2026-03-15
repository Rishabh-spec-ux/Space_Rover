# Space Rover

ROS 2 workspace for a Curiosity-inspired space rover project.

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

## Current Progress

What is already present:

- ROS 2 workspace layout
- package separation between project logic and robot description
- rover description package with Xacro-based organization
- launch files for local visualization and Gazebo simulation
- asset folders for meshes, models, and worlds

What is still incomplete or evolving:

- full rover control stack
- autonomy and navigation pipeline
- testing and CI setup
- documented hardware/software interface assumptions
- polished package metadata for a public GitHub release

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

## GitHub Preparation Notes

This repository is organized to be pushed as a single GitHub project with both ROS 2 packages under `src/`.

Before pushing publicly, review:

- package descriptions
- package versioning
- repository URL fields in package metadata
- license consistency
- screenshots or demo media you may want to add later

At the moment, the repository root has Git initialized, but there is no `origin` remote configured yet in this workspace. If you want to connect it to GitHub, add your remote and push from the workspace root.

Example:

```bash
cd /home/rishabh/space_rover
git branch -M main
git remote add origin https://github.com/<your-username>/space_rover.git
git push -u origin main
```

## Recommended Next Steps

Good next improvements for this repository:

- add screenshots or GIFs for RViz and Gazebo
- add a GitHub Actions workflow for `colcon build`
- add Xacro / URDF validation checks
- add real control and navigation nodes under `space_rover`
- document the rover kinematics and coordinate frames
- clean up any unused legacy assets over time

## Summary

This repository is already in a better state for GitHub when treated as a ROS 2 workspace with one active description package and one future-facing core package. The main rover model and simulation assets are present, but the project should still be presented as a developing robotics project rather than a finished product.
