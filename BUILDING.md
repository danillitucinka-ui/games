# Build Instructions for Contributors

## Prerequisites
- Git
- CMake 3.10+
- MinGW-w64 or Visual Studio 2019+
- SFML 2.5.1 development libraries

## Windows (MinGW)
```bash
# Install MinGW-w64
# Download from: https://www.mingw-w64.org/

# Install SFML
# Download SFML 2.5.1 MinGW from: https://www.sfml-dev.org/download.php
# Extract to C:\SFML or similar

# Clone and build
git clone https://github.com/yourusername/ShirotaBloodPolarity.git
cd ShirotaBloodPolarity/cpp_game
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DSFML_DIR="C:\SFML\lib\cmake\SFML"
mingw32-make

# Copy DLLs
cp C:\SFML\bin\*.dll .
```

## Windows (Visual Studio)
```bash
# Install Visual Studio with C++ development
# Install SFML using vcpkg or manually

# Build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
```

## Linux
```bash
# Install dependencies
sudo apt-get install libsfml-dev cmake g++

# Build
mkdir build && cd build
cmake ..
make
```

## macOS
```bash
# Install SFML
brew install sfml

# Build
mkdir build && cd build
cmake ..
make
```

## Running the Game
```bash
./ShirotaBloodPolarity
```

## Troubleshooting
- Ensure SFML DLLs are in the same directory as the executable
- Check that your compiler supports C++17
- Verify CMake can find SFML
- For Windows, ensure MinGW bin directory is in PATH

## Development
- Use an IDE with CMake support (CLion, VSCode, Visual Studio)
- Enable warnings during development: `cmake .. -DCMAKE_CXX_FLAGS="-Wall -Wextra"`
- Run debug builds with: `cmake .. -DCMAKE_BUILD_TYPE=Debug`