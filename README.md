# CppMinimalglTF

## Overview

A minimal and efficient C++20 glTF serializer/deserializer with a header-only design philosophy and a functional programming approach. This project includes:

- A basic glTF parser and serializer/deserializer.
- Example usage with a demo application.
- Support for cross-platform compatibility using modern C++ standards.

## File Structure

```bash
/YourProject
│
├── /include                    # Directory for all header files (header-only library code)
│   ├── minimalgltf.hpp         # Main glTF parser and serializer/deserializer header
│   └── ...                     # Other utility headers or single-file libraries 
│
├── /src                        # Source directory (only for example or demo applications)
│   └── main.cpp                # Main application file demonstrating usage
│
├── /third_party                # Directory for third-party header-only libraries
│   └── nlohmann_json.hpp       # Header-only JSON library (nlohmann/json)
│
├── /models                     # Directory for example glTF models
│   └── Cube.gltf               # Sample glTF file
│
├── CMakeLists.txt              # CMake build file (if you are using CMake for your project)
│
└── README.md                   # Project README in Markdown format
```

## How to Build

To build the project using CMake:

```bash
mkdir build
cd build
cmake ..
make
```

## Usage
Run the demo application:
```bash
./GltfDemo
```

This will load the example glTF model from the /models directory and display its node information.

Dependencies
nlohmann/json (header-only JSON library)
