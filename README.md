An example of using the proposed websocket api for aseprite. The server displays the sprite updating in real time.

I haven't tested linux or macox builds, the linking options might be wrong (see github.com/erkkah/tigr)

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
