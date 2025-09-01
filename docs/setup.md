# Setup

In order to run this project locally you will need to follow these steps

1. Install g++ compiler and other related dependencies

On linux systems 

```bash
sudo apt update
sudo apt install build-essential
```

2. Install vcpkg 

Think of vcpkg as `pip` for C++, this is where we will install all dependency libraries. 

From the root of this project run these commands 

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install 
```

3. Build executable (via CMake)

From the root of this project run these commands

```bash
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake
make
./MyApp 
```

If the last command is able to run the file called `main.cpp` from the root of the repo then you have setup this project correctly. 