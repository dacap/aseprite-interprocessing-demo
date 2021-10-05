An example of using the proposed websocket api for aseprite. The server displays the sprite updating in real time.

[proposal PR](https://github.com/aseprite/aseprite/pull/2980)

![xNNIG155lM](https://user-images.githubusercontent.com/49268426/135765267-6e77fefe-7fa0-45eb-8a9b-9525d6713caf.gif)

[Windows exe for the server](https://github.com/lampysprites/aseprite-interprocessing-demo/releases)

## Dependencies

Needed by [Tigr](https://github.com/erkkah/tigr):

- opengl32 and gdi32 on Windows
- OpenGL and Cocoa on macOS
- libGLU, libGL and libX11 on Linux

## Usage
 - build aseprite **from the PR**
 - launch the server and run `client.lua` script in the patched aseprite
## Building server
C++
```shell
mkdir build && cd build
cmake -DUSE_ZLIB=OFF ..
cmake --build .
```
