
#include <config.h>
#include <connection.h>
#include <log.hpp>
#include <protocol.pb.h> // remove ugly .pb
#include <event_receiver.hpp>

#include <irrlicht/irrlicht.h>

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <sstream>
#include <iostream>

using namespace irr;

EventReceiver _receiver;
std::string _title = std::string("Cineland ") + BUILD_VERSION_MAJOR + "." +
                     + BUILD_VERSION_MINOR + "." + BUILD_VERSION_REVISION;

class GfxDevice
{
public:
  virtual ~GfxDevice() {}

  void clear() {}
  void flip() {}
  void drawMessage(const char *) {}
};

class HiDevice
{
public:
  struct Input {
    bool escapePressed;
  };

  virtual ~HiDevice() {}

  Input *read(Input &) const {
    return 0;
  }
};

class TileMap
{
public:
  void assign(const MapData &data) {}
  void render(IrrlichtDevice *gfx, double time) {}
};

class Entity
{
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
class ProxyEntity : public Entity
{
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
    } else {
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

IrrlichtDevice *createGfxDevice(int width, int height)
{
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
  params.EventReceiver = &_receiver;


  std::wstring capt;
  capt.assign(_title.begin(), _title.end());

  std::auto_ptr<IrrlichtDevice> device(createDeviceEx(params));
  device->setWindowCaption(capt.c_str());
  return device.release();
}

HiDevice *createHiDevice()
{
  return new HiDevice;
}

int main()
{
  Log::registerConsumer(Log::ToCoutConsumer());

  typedef unsigned EntityId;
  typedef std::map<EntityId, Entity *> EntityMap;

  std::auto_ptr<IrrlichtDevice> gfx(createGfxDevice(800, 600));
  std::auto_ptr<HiDevice> hid(createHiDevice());
  TileMap map;

  //Connection client;
  //client.connect("localhost");

  do {
    Log(DEBUG) << "Sending an initial position update";

    PositionUpdate position;
    position.set_entity(1);
    position.set_x(2);
    position.set_y(3);

    NetMessage message;
    message.set_type(NetMessage::POSITION_UPDATE);
    *(message.mutable_player_position()) = position;

    //client.send(message);
  } while (false);

  EntityMap entities;

  gui::IGUIFont *font = gfx->getGUIEnvironment()->getBuiltInFont();

  EntityId localPlayer = 0;
  bool running = true;
  double time = 0.0;
  unsigned long frame = 0;

  video::IVideoDriver* driver = gfx->getVideoDriver();
  scene::ISceneManager* smgr = gfx->getSceneManager();

  irr::scene::ISceneNode* node = smgr->addSphereSceneNode();
  if (node) {
    node->setPosition(core::vector3df(0,0,30));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
  }

  int lastFPS = -1;

  // In order to do framerate independent movement, we have to know
  // how long it was since the last frame
  irr::u32 then = gfx->getTimer()->getTime();

  // This is the movemen speed in units per second.
  const irr::f32 MOVEMENT_SPEED = 100.f;

  video::ITexture* image = driver->getTexture("../res/dalmatians.png");
  video::ITexture* twin = driver->getTexture("../res/twin.png");


  while(gfx->run()) {
    if(_receiver.isKeyDown(irr::KEY_ESCAPE)) {
      Log(INFO) << "Escape received => closing down the device";
      gfx->closeDevice();
    }

    const irr::u32 now = gfx->getTimer()->getTime();
    const irr::f32 frameDeltaTime = (irr::f32)(now - then) / 1000.f;
    then = now;


    irr::core::vector3df nodePosition = node->getPosition();
    if(_receiver.isKeyDown(irr::KEY_KEY_W))
      nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;
    else if(_receiver.isKeyDown(irr::KEY_KEY_S))
      nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;

    if(_receiver.isKeyDown(irr::KEY_KEY_A))
      nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
    else if(_receiver.isKeyDown(irr::KEY_KEY_D))
      nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

    node->setPosition(nodePosition);


    driver->beginScene(true, true, video::SColor(255,113,113,133));

    driver->draw2DImage(image, core::position2d<s32>(0,0),
                        core::rect<s32>(0,0,800,600), 0,
                        video::SColor(255,255,255,255), true);

    driver->draw2DImage(twin, core::position2d<s32>(nodePosition.X,nodePosition.Y),
                        core::rect<s32>(0,0,120,180), 0,
                        video::SColor(255,255,255,255), true);


    core::position2d<s32> m = gfx->getCursorControl()->getPosition();
    driver->draw2DRectangle(video::SColor(100,255,255,255),
                            core::rect<s32>(m.X-20, m.Y-20, m.X+20, m.Y+20));
    driver->endScene();

    int fps = driver->getFPS();

    if (lastFPS != fps) {
      std::stringstream ss;
      ss << _title << " [fps: " << fps << "]";
      std::string tmp = ss.str();
      std::wstring capt;
      capt.assign(tmp.begin(), tmp.end());
      gfx->setWindowCaption(capt.c_str());
      lastFPS = fps;
    }
  }


  //Log(INFO) << "Watch out, entering the main loop..";
  // while (running) {
  //   video::IVideoDriver *video = gfx->getVideoDriver();
  //   video->beginScene(true, true, video::SColor(255, 100, 101, 140));
  //
  //   if (client.state() == Connection::CONNECTED) {
  //     // receive network messages and update game state according to events
  //     NetMessage msg;
  //     while (client >> msg) {
  //       Log(INFO) << "received msg of type " << msg.type();
  //
  //       switch (msg.type()) {
  //       case NetMessage::POSITION_UPDATE: {
  //         const PositionUpdate &pos = msg.player_position();
  //         const EntityId eid = static_cast<EntityId>(pos.entity());
  //
  //         EntityMap::iterator iter = entities.find(eid);
  //         if (iter != entities.end()) {
  //           iter->second->onNetUpdate(pos);
  //         }
  //         else if (eid != 0) {
  //           Entity *entity = new ProxyEntity;
  //           entity->onNetUpdate(pos);
  //           entities[eid] = entity;
  //         }
  //
  //         break;
  //       }
  //
  //       case NetMessage::MAP_DATA:
  //         map.assign(msg.map());
  //         break;
  //
  //       case NetMessage::PLAYER_INFO:
  //         localPlayer = static_cast<EntityId>(msg.player_info().local_id());
  //         break;
  //       }
  //     }
  //
  //     // read user input and pass it to the local player's entity
  //     HiDevice::Input input;
  //     if (hid->read(input)) {
  //       if (input.escapePressed) {
  //         running = false;
  //       }
  //
  //       if (localPlayer) {
  //         EntityMap::iterator iter = entities.find(localPlayer);
  //         if (iter != entities.end()) {
  //           iter->second->onInput(input);
  //         }
  //       }
  //     }
  //
  //     // update state of entities
  //     for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter) {
  //       iter->second->update(time);
  //     }
  //
  //     map.render(gfx.get(), time);
  //
  //     // render entities according to internal state
  //     for (EntityMap::iterator iter = entities.begin(); iter != entities.end(); ++iter) {
  //       iter->second->render(gfx.get(), time);
  //     }
  //   }
  //   else if (client.state() == Connection::CONNECTING) {
  //     //gfx->drawMessage("Connecting...");
  //   }
  //   else {
  //     localPlayer = 0;

  //     if (font) {
  //       font->draw(L"Disconnected.",
  //                  core::rect<s32>(130,10,300,50),
  //                  video::SColor(255,255,255,255));
  //     }
  //
  //     /*         lobby->update(currentTime, hid);
  //                lobby->draw(currentTime);
  //                if (lobby->state().key == GuiScreen::STATE_CONNECT) { // this could be reversed, dip.
  //                client.connect(lobby->state().value);
  //                lobby->resetState();
  //                }*/
  //
  //   }
  //
  //   ++frame;
  //   video->endScene();
  // }

  return 0;
}

