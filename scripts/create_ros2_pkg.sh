#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 2 ]]; then
  echo "Usage: $0 <package_name> (--cpp|--python)"
  exit 1
fi

PKG_NAME="$1"
PKG_TYPE="$2"

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC_DIR="${ROOT_DIR}/src"
PKG_DIR="${SRC_DIR}/${PKG_NAME}"

if [[ ! -d "${SRC_DIR}" ]]; then
  echo "Error: src directory not found at ${SRC_DIR}"
  exit 1
fi

if [[ -d "${PKG_DIR}" ]]; then
  echo "Error: package already exists: ${PKG_DIR}"
  exit 1
fi

source /opt/ros/jazzy/setup.bash

case "${PKG_TYPE}" in
  --cpp)
    ros2 pkg create "${PKG_NAME}" --build-type ament_cmake --license MIT
    ;;
  --python)
    ros2 pkg create "${PKG_NAME}" --build-type ament_python --license MIT
    ;;
  *)
    echo "Error: second argument must be --cpp or --python"
    exit 1
    ;;
esac

mv "${ROOT_DIR}/${PKG_NAME}" "${PKG_DIR}"

if [[ ! -f "${PKG_DIR}/README.md" ]]; then
  cat > "${PKG_DIR}/README.md" <<EOF
# ${PKG_NAME}

ROS 2 package: ${PKG_NAME}
EOF
fi

echo "Created package: ${PKG_DIR}"
