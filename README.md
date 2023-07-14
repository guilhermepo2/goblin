# RIP :(

This "game engine" is no long being developed or anything (for a multitude of reasons!!) - But there might be something interesting here, so I will just keep the repository.

![Goblin](gge.png)
# Goblin Game Engine ![Goblin](gbln.png)

### emscripten

Important notes on how to build with Emscripten.

First, configure the CMake project with something like this. This is the command I use on my MacBook so might it's probably going to be different for you!! The important thing is that as long as you have a emscripten cmake toolchain (currently using version 3.1.32) it should all work fine.

```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=/opt/homebrew/Cellar/emscripten/3.1.32/libexec/cmake/Modules/Platform/Emscripten.cmake -S/Users/gdeoliveira/workspace/goblin -B/Users/gdeoliveira/workspace/goblin/www
```

Second, move into the www (`cd www`) folder, and `make all`

The output can only be run on a web server, or `emrun goblin-engine.html`
