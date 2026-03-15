# `space_rover`

Core ROS 2 package for the overall rover project.

## Purpose

This package is reserved for the software side of the rover system. The description package already contains the robot model and simulation assets, while `space_rover` is where the project can grow into a full robotics stack.

## Current Status

This package is still in an early stage.

Right now it mainly provides:

- package metadata
- `ament_cmake` build support
- a clean place to add future project code

## Planned Responsibilities

Expected future contents include:

- rover control nodes
- actuator and command interfaces
- mission or behavior logic
- navigation integration
- high-level launch files that combine subsystems

## Build

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
colcon build --packages-select space_rover
source install/setup.bash
```

## Notes

- Package name: `space_rover`
- Build type: `ament_cmake`
- License: `MIT`
- Repository path: `src/space_rover/`

## Relationship To Other Packages

This package is intended to work alongside `space_rover_description`, which currently contains the model, simulation, and visualization resources for the rover.
