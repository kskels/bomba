#include <iostream>
#include <irrlicht/irrlicht.h>
#include <memory>

using namespace irr;

const int WIDTH = 320;
const int HEIGHT = 240;

int main() {
  std::cout << "\\Weeeeee/" << std::endl;

  SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
  params.AntiAlias = true;
  params.WithAlphaChannel = true;
  params.DeviceType = EIDT_BEST;
  params.DriverType = video::EDT_OPENGL;
  params.WindowSize = core::dimension2d<u32>(WIDTH,HEIGHT);
  params.Doublebuffer = true;
  params.Bits = 32;
  params.ZBufferBits = 16;
  params.Fullscreen = false;
  params.Vsync = params.Fullscreen;
  std::auto_ptr<IrrlichtDevice> device(createDeviceEx(params));
  
  std::wstring capt = L"Bomba - ver: 0.0.1b ";
  device->setWindowCaption(capt.c_str());

  while(device->run()) {
  }

  device.release()->drop();

}
