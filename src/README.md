# `src` Workspace Layout

All ROS 2 packages for this repository live directly inside `src/`.

## Purpose

This folder keeps the workspace organized in the standard ROS 2 style:

- one directory per package
- package-local assets stored with the package that owns them
- shared version control from the workspace root

## Current Packages

- `space_rover`
  Main project package intended for future rover logic, control, and integration work.
- `space_rover_description`
  Robot description and simulation package containing URDF/Xacro files, meshes, launch files, RViz config, and Gazebo assets.

## Rules For This Workspace

1. Keep each package in its own folder: `src/<package_name>/`
2. Do not create nested Git repositories inside `src/`
3. Keep package metadata complete: `package.xml`, build file, and `README.md`
4. Build packages from the workspace root, not from inside individual packages
5. Keep simulation assets inside the package that uses them

## Build From Workspace Root

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
colcon build
```

## Adding New Packages

If you add more rover-related packages later, place them directly under `src/` so the repository stays clean and easy to maintain on GitHub.
