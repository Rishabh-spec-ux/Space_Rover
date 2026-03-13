# Space Rover ROS 2 Workspace

This repository is a ROS 2 workspace containing two packages:

1. `space_rover` (core package scaffold)
2. `space_rover_description` (Curiosity-style robot description, meshes, launch files, worlds)

This layout keeps runtime/control code separate from robot description and simulation assets.

## Workspace Structure

- `src/space_rover/`
  Core package for rover-side integration
- `src/space_rover_description/`
  Robot model package with URDF/Xacro, meshes, RViz config, Gazebo world/models
- `scripts/create_ros2_pkg.sh`
  Helper for creating new ROS 2 packages directly under `src/`
- `build/`, `install/`, `log/`
  Colcon-generated artifacts (ignored by `.gitignore`)

## Prerequisites

- Ubuntu + ROS 2 Jazzy
- `colcon` and `xacro`
- Gazebo bridge packages used by launch files:
  - `ros_gz_sim`
  - `ros_gz_bridge`

## Build

```bash
cd /home/rishabh/space_rover
source /opt/ros/jazzy/setup.bash
colcon build
source install/setup.bash
```

## Add More Packages (Monorepo Workflow)

Create a new C++ package:

```bash
cd /home/rishabh/space_rover
./scripts/create_ros2_pkg.sh my_new_pkg --cpp
```

Create a new Python package:

```bash
cd /home/rishabh/space_rover
./scripts/create_ros2_pkg.sh my_python_pkg --python
```

Then build all packages together:

```bash
source /opt/ros/jazzy/setup.bash
colcon build
```

## Run (RViz)

```bash
ros2 launch space_rover_description display.launch.py
```

## Run (Gazebo)

```bash
ros2 launch space_rover_description gazebo.launch.py
```

## Important Note About ROS1 Launch Files

Legacy ROS1 `.launch` files are stored under:

- `src/space_rover_description/ros1_launch/`

Do not run these with `ros2 launch`.
Use only `.launch.py` files in `src/space_rover_description/launch/`.

## Push Both Packages to One GitHub Repo

Use a single GitHub repository named `space_rover`.
Both packages stay together under `src/`.

### 1. Initialize Git at workspace root

```bash
cd /home/rishabh/space_rover
git add .
git commit -m "Initial ROS 2 workspace with space_rover and space_rover_description"
```

### 2. Connect and push

```bash
git branch -M main
git remote add origin https://github.com/<your-username>/space_rover.git
git push -u origin main
```

### 3. Expected repo layout

- `src/space_rover/`
- `src/space_rover_description/`

## Recommended Next Step

Add CI (GitHub Actions) to run:

- `colcon build`
- `xacro` check on the top-level rover model
- optional lint (`ament_lint_auto`)
