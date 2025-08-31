cmake_minimum_required(VERSION 3.14)
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Require at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------
# Fetch ASIO (header-only)
# -----------------------------
include(FetchContent)

FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-28-1  # Specify the version you want
)

FetchContent_MakeAvailable(asio)

# -----------------------------
# Add your executable
# -----------------------------
add_executable(MyApp main.cpp)

# Include ASIO headers
target_include_directories(MyApp PRIVATE ${asio_SOURCE_DIR}/asio/include)

# Optionally, enable ASIO standalone mode (no Boost needed)
target_compile_definitions(MyApp PRIVATE ASIO_STANDALONE)

# -----------------------------
# Optional: Add threading support if using ASIO io_context
# -----------------------------
find_package(Threads REQUIRED)
target_link_libraries(MyApp PRIVATE Threads::Threads)
