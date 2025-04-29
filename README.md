# Cub3d Graphics Project

![MLX42 Logo](https://github.com/codam-coding-college/MLX42/raw/main/assets/mlx42.png)

A graphics project using the MLX42 library - a performant, cross-platform windowing and graphics library built on OpenGL.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Dependencies](#dependencies)
- [Troubleshooting](#troubleshooting)
- [License](#license)

## Features ✨
- **Cross-platform** (Windows, macOS, Linux)
- **Hardware-accelerated** via OpenGL
- **Simple API** for window management and rendering
- **Texture manipulation** at runtime
- **Emscripten support** for web deployment

## Installation

### Prerequisites
- CMake (minimum version 3.10)
- GLFW 3.3+
- C compiler (GCC/Clang/MSVC)

### Building MLX42
```bash
cd .MLX42
cmake -B build
cmake --build build -j4
```

### Usage
Basic Compilation
macOS:
gcc main.c -Iinclude -Lbuild -lmlx42 -lglfw -framework Cocoa -framework OpenGL -framework IOKit

Linux:
gcc main.c -Iinclude -Lbuild -lmlx42 -lglfw -ldl -pthread -lm

Windows (WSL2):
gcc main.c -Iinclude -Lbuild -lmlx42 -lglfw -lX11

### Project Structure
.
├── .MLX/                # MLX42 library files
│   └── libmlx42.a      # Compiled library
├── src/                # Project source files
├── includes
├── maps
├── textures
├── gnl
├── utils
└── Makefile           # Build configuration

### Dependencies

GLFW: Window and input handling
OpenGL: Graphics rendering
CMake: Build system

Install dependencies on:

macOS:
brew install glfw cmake

Ubuntu/Debian:
sudo apt install libglfw3-dev libx11-dev cmake

Arch Linux:
sudo pacman -S glfw-x11 cmake

### License
MLX42 is licensed under the BSD 3-Clause License.

