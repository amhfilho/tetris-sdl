# Tetris SDL

A simple Tetris clone written in C++20 with [SDL3](https://www.libsdl.org/).
This is an attempt to convert the project from this video, [Tetris in C++ with SDL2](https://www.youtube.com/watch?v=QpdhBUYk7K0), to SDL3.
## Features

- All seven tetrominoes (I, J, L, O, S, T, Z) with rotation
- Line clearing and score board
- "Next piece" preview
- Game over and restart

## Controls

| Key        | Action                        |
|------------|-------------------------------|
| Left/Right | Move piece                    |
| Down       | Soft drop                     |
| Up         | Rotate                        |
| Esc        | Quit                          |
| Any key    | Restart after game over       |

## Requirements

- C++20 compiler (clang on macOS)
- CMake 3.28+
- SDL3 and SDL3_image

Initially this project was developed on macOS with Homebrew:

```sh
brew install cmake sdl3 sdl3_image
```

## Build and run

```sh
cmake -S . -B build
cmake --build build
./build/tetris_sdl
```

### Linux

Install a compiler, CMake and SDL3 with your package manager. SDL3 is only packaged in recent distributions (for example Debian 13 or Ubuntu 25.04+):

```sh
sudo apt install build-essential cmake libsdl3-dev libsdl3-image-dev
```

On older distributions, build SDL3 and SDL3_image from source
([SDL](https://github.com/libsdl-org/SDL), [SDL_image](https://github.com/libsdl-org/SDL_image)) and install them, then build as above.

### Windows

Install [Visual Studio](https://visualstudio.microsoft.com/) (with the C++ workload), CMake and [vcpkg](https://github.com/microsoft/vcpkg), then from a Developer PowerShell:

```powershell
vcpkg install sdl3 sdl3-image
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
.\build\Release\tetris_sdl.exe
```

## Project layout

```
include/   Headers (Block, Colors, Game, Grid)
src/       Sources (main, Game, Grid, Block, Blocks, Colors)
```

`src/Blocks.cpp` defines the individual tetromino shapes and their rotations.