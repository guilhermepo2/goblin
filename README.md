![Goblin](gge.png)
# Goblin Game Engine ![Goblin](gbln.png)

The reason why I decided to build a "general-purpose" game-engine is because I tried making a game on my game library, gueepo2D, and had a really bad time, so I decided I need some abstractions to be able to make small, simple games really quickly.

Here's what "the goblin game engine"

1. Built with gueepo2D
2. ~~Resource Manager~~ this was eventually added on gueepo2D/utils
3. ECS
4. LUA

And eventually, the goal is to have a visual editor.

1. The editor should be able to create/edit/save/load maps, and tilemaps
2. The editor should be able to create/edit/save/load entities
3. The editor should be able to create/edit/save/load scenes
4. The editor should be able to create/edit/save/load resources
5. etc., etc.,

The end goal is to be able to create an entire game without coding C++, but if I can and think it's a good idea to do C++ while making a game with The Goblin Game Engine, then cool.

---

### emscripten

Important notes on how to build with Emscripten.

First, configure the CMake project with something like this. This is the command I use on my MacBook so might it's probably going to be different for you!! The important thing is that as long as you have a emscripten cmake toolchain (currently using version 3.1.32) it should all work fine.

```
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=/opt/homebrew/Cellar/emscripten/3.1.32/libexec/cmake/Modules/Platform/Emscripten.cmake -S/Users/gdeoliveira/workspace/goblin -B/Users/gdeoliveira/workspace/goblin/www
```

Second, move into the www (`cd www`) folder, and `make all`

The output can only be run on a web server, or `emrun goblin-engine.html`