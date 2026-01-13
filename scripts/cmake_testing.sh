#! /usr/bin/bash
cmake -S /app -B /app/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
  && cmake --build /app/build --target test_comp \
  && ./build/apps/test_comp