# Base image with build tools
FROM ubuntu:22.04


# Install essentials
RUN apt-get update && apt-get install -y \
    build-essential cmake git curl unzip zip python3 pkg-config && \
    rm -rf /var/lib/apt/lists/*


# Set working directory
WORKDIR /workspace

# Clone and bootstrap vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg && \
    /vcpkg/bootstrap-vcpkg.sh

# Copy project files
COPY . /workspace

# Install dependencies from manifest
RUN /vcpkg/vcpkg install --triplet x64-linux

# Default command
CMD ["bash"]
