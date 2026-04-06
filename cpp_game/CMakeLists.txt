cmake_minimum_required(VERSION 3.10)
project(ShirotaBloodPolarity)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Fighter.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/Particle.cpp
    src/AudioManager.cpp
    src/Menu.cpp
    src/StoryMode.cpp
    src/StoryManager.cpp
    src/Level.cpp
    src/SaveSystem.cpp
    src/Animation.cpp
    src/EffectManager.cpp
    src/SpriteGenerator.cpp
    src/Cutscene.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link SFML
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-audio)

# Copy resources (optional)
# file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}/)
