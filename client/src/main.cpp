
#include <config.h>
#include <connection.h>

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "protobuf/protocol.pb.h" // remove ugly .pb

class GfxDevice {
public:
  virtual ~GfxDevice() {}

  void clear() {}
  void flip() {}
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
  void render(GfxDevice *gfx, double time) {}
};

class Entity {
public:
  virtual ~Entity() {}

  virtual void onNetUpdate(const PositionUpdate &) =0;
  virtual void onInput(const HiDevice::Input &) =0;
  virtual void update(double time) =0;
  virtual void render(GfxDevice *gfx, double time) const =0;
};

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
  
  void render(GfxDevice *gfx, double time) const {
    if (_delegate) {
      _delegate->render(gfx, time);
    }
    else {
      // render placeholder
    }
  }

  void setDelegate(Entity *delegate) {
    delete _delegate;
    _delegate = delegate;
  }
  
private:
  Entity *_delegate;
};


GfxDevice *createGfxDevice() {return new GfxDevice; }
HiDevice *createHiDevice() {return new HiDevice; }
void drawMessage(GfxDevice *, const char *) {}

int main() {
  typedef unsigned EntityId;
  typedef std::map<EntityId, Entity *> EntityMap;
  
  std::auto_ptr<GfxDevice> gfx(createGfxDevice());
  std::auto_ptr<HiDevice> hid(createHiDevice());
  TileMap map;
  Connection client;
  EntityMap entities;

  EntityId localPlayer = 0;
  bool running = true;
  double time = 0.0;
  unsigned long frame = 0;
  
  while (running) {
    gfx->clear();
    if (client.state() == Connection::CONNECTED) {
      // receive network messages and update game state according to events
      NetMessage msg;
      while (client.receive(msg)) {
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
      drawMessage(gfx.get(), "Connecting...");
    }
    else {
      /*         lobby->update(currentTime, hid);
            lobby->draw(currentTime);
            if (lobby->state().key == GuiScreen::STATE_CONNECT) { // this could be reversed, dip.
                client.connect(lobby->state().value);
                lobby->resetState();
                }*/

    }

    ++frame;
    gfx->flip();
  }
  
}

