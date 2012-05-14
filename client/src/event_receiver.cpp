
#include <log.hpp>
#include <event_receiver.hpp>

EventReceiver::EventReceiver()
{
  Log(DEBUG) << "";
  for(irr::u32 i(0); i != irr::KEY_KEY_CODES_COUNT; ++i) {
    _keyIsDown[i] = false;
  }
}

EventReceiver::~EventReceiver()
{
  Log(DEBUG) << "";
}

bool EventReceiver::OnEvent(const irr::SEvent& event)
{
  if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
    Log(DEBUG) << "Key " << event.KeyInput.Key  << " "
               << (event.KeyInput.PressedDown ? "pressed" : "released");
    _keyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
  }
  return false;
}

bool EventReceiver::isKeyDown(irr::EKEY_CODE keyCode) const
{
  return _keyIsDown[keyCode];
}

