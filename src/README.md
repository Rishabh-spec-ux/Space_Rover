# `src` Package Layout

All ROS 2 packages in this workspace live directly under `src/`.

## Rules

1. One folder per package (`src/<package_name>/`)
2. Each package must contain at least:
   - `package.xml`
   - `CMakeLists.txt` (or `setup.py` for Python packages)
   - `README.md`
3. Do not create nested git repositories inside `src/`.
4. Keep shared assets inside the package that owns them.
5. Build all packages together from workspace root.

## Add a New Package

From workspace root:

```bash
./scripts/create_ros2_pkg.sh <package_name> --cpp
```

or

```bash
./scripts/create_ros2_pkg.sh <package_name> --python
```

Then commit from workspace root so all package changes are versioned together.
