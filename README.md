# MCRewrite
An open source Minecraft Java Edition rewrite in C++ using raylib.
This branch version is rd-132211.

## TODO for rd-132211
- [ ] Lighting (probably using a depth buffer)
- [ ] Optimizations for camera ray hitting
- [ ] Maybe change the camera FOV as we are using just a random value rn :skull:
- [ ] Add frustum

## What do we want it to be?
This is meant to be a 1:1 user experience rewrite/decompilation (the game look and gameplay must match) but the code may and does not match with the original as we are using modern C++ features and trying to improve the original code structure.

## How did we do that?
We just decompiled a Java Minecraft binary and tried to rewrite it in C++ but with our own modifications.

## Is it better?
Yeah, rd-132211 runs at ~2500-3000 fps on my Windows 10 instead of 300 and you can ideally see no difference between this and the Java version.

## Running
Keep in mind that we are not publishing the game's assets so you have to find them yourself. You should place them into the `assets` directory which should be near the executable.

You can download a binary from the releases tab or build it yourself.

### Building
```
git clone https://github.com/JaanDev/MCRewrite.git --recursive
cd MCRewrite
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build
```
Or just open the folder in VS Code/Visual Studio (don't forget the CMake extension for VS Code) and build it from there.

### Contributions
The contributions are always welcomed! :<zero-width space>)

### Our contacts
Discord:
* Jaan#2897
* Kolyah35#0107
