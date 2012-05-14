#ifndef _BOMBA_EVENT_RECEIVER_
#define _BOMBA_EVENT_RECEIVER_

#include <irrlicht/irrlicht.h>


class EventReceiver : public irr::IEventReceiver
{
public:
  EventReceiver();
  virtual ~EventReceiver();

  virtual bool OnEvent(const irr::SEvent& event);
  virtual bool isKeyDown(irr::EKEY_CODE keyCode) const;

private:
  bool _keyIsDown[irr::KEY_KEY_CODES_COUNT];
};

#endif // _BOMBA_EVENT_RECEIVER_

