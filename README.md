# MCRewrite
An open source Minecraft Java Edition rewrite in C++ using OpenGL focused on matching the original game as close as possible.  
This branch version is rd-132211.

## Demonstration
<img src="https://i.postimg.cc/yx2bc1ym/screenshot1.png" width="47%" align="left">
<img src="https://i.postimg.cc/QdQyKr97/screenshot2.png" width="47%" align="right">
<img src="https://i.postimg.cc/9Q5kffMV/screenshot3.png" width="47%" align="left">
<img src="https://i.postimg.cc/c48jZ3dX/screenshot4.png" width="47%" align="right">

*The screenshots were made with original Minecraft assets for demonstration purposes. These assets are not included in the project and are property of Mojang/Microsoft.*

## What do we want it to be?
This is meant to be a 1:1 user experience rewrite/decompilation (the game look and gameplay must match) but the code may not match with the original as we are using modern C++ and OpenGL features and trying to improve the original code structure.

## How did we do that?
We used a decompilation of the Java Minecraft (RubyDung) binary and tried to rewrite it in C++ but with our own modifications.

## Is it better?
It runs much faster and you can see no difference between our recreation and the original game.

## Building
1. Clone the branch of this repo that you need: `git clone https://github.com/JaanDev/MCRewrite.git -b rd-132211`
2. Navigate to the cloned repository: `cd MCRewrite`
3. Because we cant upload the original assets here, **you should get them by yourself** (For example, by downloading the original jar [here](https://piston-data.mojang.com/v1/objects/393e8d4b4d708587e2accd7c5221db65365e1075/client.jar)). You will need to put the terrain.png to the directory with the game executable before running!
4. Configure cmake: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release`
5. Build the game: `cmake --build build --config Release`
6. That's all. Depending on the compiler and the platform, your executable should be somewhere in the build folder. Dont forget about the assets =<zero-width space>)

## Contributions
Any contributions are always welcome! :<zero-width space>)

## Our contacts
Discord:
* jaan2897
* kolyah35
