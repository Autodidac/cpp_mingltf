<img align="left" src="models/Avocado_baseColor.png" width="100px"/>

# cpp_mingltf - Minimal glTF Library in C++20 [![Actions Status](https://github.com/Autodidac/cpp_mingltf/workflows/build/badge.svg)](https://github.com/Autodidac/cpp_mingltf/actions) [![codecov.io](https://codecov.io/github/Autodidac/cpp_mingltf/coverage.svg?branch=master)](https://codecov.io/github/Autodidac/cpp_mingltf?branch=master) ![MIT](https://img.shields.io/badge/license-MIT-blue.svg) [![GitHub](https://img.shields.io/badge/repo-github-green.svg)](https://github.com/Autodidac/cpp_mingltf)


[![Github Build Status](https://github.com/Autodidac/cpp_mingltf/workflows/Build/badge.svg)](https://github.com/Autodidac/cpp_mingltf/actions)

## Minimal glTF Serializer/Parser in C++20 for Game Development

A high-performance, header-only glTF parser and serializer library designed for C++20. This library is optimized for real-time applications, such as game development, where speed and low memory overhead are crucial.

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
│   ├── cpp_mingltf.hpp         # Main glTF parser and serializer/deserializer header
│   └── ...                     # Other utility headers or single-file libraries 
│
├── /src                        # Source directory (only for example or demo applications)
│   └── main.cpp                # Main application file demonstrating usage
│
├── /dep                        # Directory for third-party libraries
│   └── cpp_minjson.hpp         # Header-only JSON library (Autodidac/CppMinimalJSON)
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

## Dependencies
Autodidac/CppMinimalJSON (header-only JSON library)
