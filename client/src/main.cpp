
#include <config.h>
#include <connection.h>
#include <log.hpp> 

#include <iostream>
#include <vector>
#include <map>
#include <memory>

#include <irrlicht/irrlicht.h>
#include "protobuf/protocol.pb.h" // remove ugly .pb

using namespace irr;


class GfxDevice {
public:
  virtual ~GfxDevice() {}

  void clear() {}
  void flip() {}
  void drawMessage(const char *) {}
};

class HiDevice {
public:
  struct Input {
    bool escapePressed;
  };

  virtual ~HiDevice() {}
  
  Input *read(Input &) const {return 0; }
};

class TileMap {
public:
  void assign(const MapData &data) {}
  void render(IrrlichtDevice *gfx, double time) {}
};

class Entity {
public:
  virtual ~Entity() {}

  virtual void onNetUpdate(const PositionUpdate &) =0;
  virtual void onInput(const HiDevice::Input &) =0;
  virtual void update(double time) =0;
  virtual void render(IrrlichtDevice *gfx, double time) const =0;
};

/**
 * A proxy entity can be used to store delta packets even though
 * we don't have the complete definition of the entity yet.
 */
class ProxyEntity : public Entity {
public:
  ProxyEntity() {
    _delegate = 0;
  }
  
  void onNetUpdate(const PositionUpdate &packet) {
    if (_delegate) _delegate->onNetUpdate(packet);
  }
  
  void onInput(const HiDevice::Input &input) {
    if (_delegate) _delegate->onInput(input);
  }
  
  void update(double time) {
    if (_delegate) _delegate->update(time);
  }
  
  void render(IrrlichtDevice *gfx, double time) const {
    if (_delegate) {
      _delegate->render(gfx, time);
    }
    else {
      // render placeholder: gfx->drawBox(1, 1, 1, pos)
    }
  }

  void setDelegate(Entity *delegate) {
    // takes ownership
    delete _delegate;
    _delegate = delegate;
    if (_delegate) {
      // _delegate->onNetUpdate(storedUpdate);
    }
  }
  
private:
  Entity *_delegate; // TODO: smartptr
};

IrrlichtDevice *createGfxDevice(int width, int height) {
  using core::dimension2d;
  
  SIrrlichtCreationParameters params;
  params.AntiAlias = true;
  params.WithAlphaChannel = true;
  params.DeviceType = EIDT_BEST;
  params.DriverType = video::EDT_OPENGL;
  params.WindowSize = dimension2d<u32>(width, height);
  params.Doublebuffer = true;
  params.Bits = 32;
  params.ZBufferBits = 16;
  params.Fullscreen = false;
  params.Vsync = params.Fullscreen;
  
  std::string title = std::string("Bomba ") + BUILD_VERSION_MAJOR + "." + 
    + BUILD_VERSION_MINOR + "." + BUILD_VERSION_REVISION;
  std::wstring capt;
  capt.assign(title.begin(), title.end());

  std::auto_ptr<IrrlichtDevice> device(createDeviceEx(params));
  device->setWindowCaption(capt.c_str());
  return device.release();
}

HiDevice *createHiDevice() {return new HiDevice; }

int main() {
  Log::registerConsumer(Log::DefaultLogConsumer());

  typedef unsigned EntityId;
  typedef std::map<EntityId, Entity *> EntityMap;
  
  std::auto_ptr<IrrlichtDevice> gfx(createGfxDevice(320, 240));
  std::auto_ptr<HiDevice> hid(createHiDevice());
  TileMap map;
  Connection client;
  client.connect("localhost");
  EntityMap entities;

  gui::IGUIFont *font = gfx->getGUIEnvironment()->getBuiltInFont();
  
  EntityId localPlayer = 0;
  bool running = true;
  double time = 0.0;
  unsigned long frame = 0;
  
  while (running) {
    video::IVideoDriver *video = gfx->getVideoDriver();    
    video->beginScene(true, true, video::SColor(255, 100, 101, 140));
    
    if (client.state() == Connection::CONNECTED) {
      // receive network messages and update game state according to events
      NetMessage msg;
      while (client >> msg) {
        switch (msg.type()) {
        case NetMessage::POSITION_UPDATE: {
          const PositionUpdate &pos = msg.player_position();
          const EntityId eid = static_cast<EntityId>(pos.entity());
          
          EntityMap::iterator iter = entities.find(eid);
          if (iter != entities.end()) {
            iter->second->onNetUpdate(pos);
          }
          else if (eid != 0) {
            Entity *entity = new ProxyEntity;
            entity->onNetUpdate(pos);
            entities[eid] = entity;         
          }
          
          break;
        }
          
        case NetMessage::MAP_DATA:
          map.assign(msg.map());
          break;
          
        case NetMessage::PLAYER_INFO:
          localPlayer = static_cast<EntityId>(msg.player_info().local_id());
          break;
        }
      }
      
      // read user input and pass it to the local player's entity
      HiDevice::Input input;
      if (hid->read(input)) {
        if (input.escapePressed) {
          running = false;
        }
        
        if (localPlayer) {
          EntityMap::iterator iter = entities.find(localPlayer);
          if (iter != entities.end()) {
            iter->second->onInput(input);
          }
        }
      }
      
      // update state of entities
      for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter) {
        iter->second->update(time);
      }
      
      map.render(gfx.get(), time);
      
      // render entities according to internal state
      for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter) {
        iter->second->render(gfx.get(), time);
      }
    }
    else if (client.state() == Connection::CONNECTING) {
      //gfx->drawMessage("Connecting...");
    }
    else {
      localPlayer = 0;

	  if (font) {
		font->draw(L"Disconnected.",
				   core::rect<s32>(130,10,300,50),
				   video::SColor(255,255,255,255));
	  }
	  
      /*         lobby->update(currentTime, hid);
                 lobby->draw(currentTime);
                 if (lobby->state().key == GuiScreen::STATE_CONNECT) { // this could be reversed, dip.
                 client.connect(lobby->state().value);
                 lobby->resetState();
                 }*/
      
    }
    
    ++frame;
    video->endScene();
  }
  
}

