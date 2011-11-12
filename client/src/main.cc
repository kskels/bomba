
#include <config.h>

#include <irrlicht/irrlicht.h>

#include <memory>
#include <iostream>

using namespace irr;


const int WIDTH = 320;
const int HEIGHT = 240;

int main() {
  std::cout << "\\Weeeeee/" << std::endl;

  using core::dimension2d;
  
  SIrrlichtCreationParameters params;
  params.AntiAlias = true;
  params.WithAlphaChannel = true;
  params.DeviceType = EIDT_BEST;
  params.DriverType = video::EDT_OPENGL;
  params.WindowSize = dimension2d<u32>(WIDTH,HEIGHT);
  params.Doublebuffer = true;
  params.Bits = 32;
  params.ZBufferBits = 16;
  params.Fullscreen = false;
  params.Vsync = params.Fullscreen;

  std::auto_ptr<IrrlichtDevice> device(createDeviceEx(params));
  
  // TODO: Fix this to use new version defines from config.h
  std::wstring capt = L"Bomba - ver: 0.0.1b ";
  device->setWindowCaption(capt.c_str());

  while(device->run()) {
    irr::video::IVideoDriver* pDriver = device->getVideoDriver();
    pDriver->beginScene(true, true, video::SColor(255,100,101,140));
    pDriver->endScene();
  }

  device.release()->drop();

}
