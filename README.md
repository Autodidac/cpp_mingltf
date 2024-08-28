# CppMinimalglTF

## Overview

A minimal and efficient C++20 glTF serializer/deserializer with a header-only design philosophy and a functional programming approach. This project includes:

- A basic glTF parser and serializer/deserializer.
- Example usage with a demo application.
- Support for cross-platform compatibility using modern C++ standards.

## File Structure

/YourProject ├── /include │ ├── gltf_parser.hpp │ └── ... ├── /src │ └── main.cpp ├── /third_party │ └── nlohmann_json.hpp ├── /models │ └── example_model.gltf ├── CMakeLists.txt └── README.md


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
