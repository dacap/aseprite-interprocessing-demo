/*
* Sync aseprite image via websocket - server
*/

#include "tigr/tigr.h"
#include "ixwebsocket/IXWebSocketServer.h"
#include "ixwebsocket/IXWebSocketMessageType.h"

TPixel* pixels;
unsigned long w, h;
bool update = false;

void tigrResize(Tigr* bmp, int w, int h)
{
  TPixel* newpix = (TPixel*)calloc(w * h, sizeof(TPixel));

  free(bmp->pix);
  bmp->pix = newpix;
  bmp->w = w;
  bmp->h = h;
}


// async callback
void handleMessage(std::shared_ptr<ix::ConnectionState> connectionState,
  ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg) 
{    
  if (msg->type == ix::WebSocketMessageType::Message && msg->binary) {
    unsigned long* hdr = (unsigned long*)msg->str.c_str();
    unsigned char* data = (unsigned char*)msg->str.c_str() + 3 * sizeof(unsigned long);

    if (hdr[0] == 'I') {
      w = hdr[1];
      h = hdr[2];
      pixels = (TPixel*)realloc(pixels, w * h * 4);
      memcpy(pixels, data, w * h * 4);

      update = true;
    }
  }
}


int main() 
{
  // init
  ix::initNetSystem();

  Tigr* screen = tigrWindow(320, 240, "Rawr", 0);
  Tigr* bmp = tigrBitmap(1, 1);
  pixels = (TPixel*)malloc(1);

  tigrSetPostFX(screen, 1.0, 1.0, 0.4, 1.4); // zoomer scanlines

  ix::WebSocketServer serv(34613);
  serv.disablePerMessageDeflate();
  serv.setOnClientMessageCallback(&handleMessage);
  serv.listenAndStart();

  // main loop
  while (!tigrClosed(screen))
  {
    if (update) {
      tigrResize(bmp, w, h);
      screen->w = bmp->w;
      screen->h = bmp->h;
      memcpy(bmp->pix, pixels, w * h * sizeof(TPixel));

      update = false;
    }

    tigrClear(screen, tigrRGB(0x80, 0x80, 0x80));
    tigrBlitAlpha(screen, bmp, 0, 0, 0, 0, bmp->w, bmp->h, 1.0);
    tigrUpdate(screen);
  }

  // clean up
  tigrFree(screen);
  tigrFree(bmp);
  serv.stop();
  ix::uninitNetSystem();

  return 0;
}