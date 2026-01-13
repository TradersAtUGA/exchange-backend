#! /usr/bin/bash
cmake -S /app -B /app/build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake \
  -DBUILD_UNIT_TESTING=ON \
  -DBUILD_PERF_TESTING=OFF \
  && cmake --build /app/build --target unit_tests \
  && cd /app/build \
  && ctest --output-on-failure