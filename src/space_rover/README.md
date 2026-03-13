# space_rover

ROS 2 core package scaffold for the Space Rover project.

## Purpose

`space_rover` is intended to host rover runtime logic and integration code, while robot geometry and simulation assets are maintained in `space_rover_description`.

Typical future contents for this package:

- rover control nodes
- planning/navigation integration
- controller orchestration launch files
- mission behavior/state-machine code

## Current State

This package currently provides:

- valid ROS 2 package metadata
- `ament_cmake` build support
- dependency link to `space_rover_description`

## Build

From workspace root:

```bash
source /opt/ros/jazzy/setup.bash
colcon build --packages-select space_rover
source install/setup.bash
```

## Package Metadata

- Package name: `space_rover`
- Build type: `ament_cmake`
- License: `MIT`

## Repository Layout

This package is intended to live in the same repository as `space_rover_description`:

- repo: `space_rover`
- path: `src/space_rover/`
