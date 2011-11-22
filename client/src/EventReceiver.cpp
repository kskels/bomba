#include <irrlicht/irrlicht.h>
#include "EventReceiver.h"

EventReceiver::EventReceiver()
{

}

EventReceiver::EventReceiver(IrrlichtDevice* device)
 :m_Device(device)
{
}

EventReceiver::~EventReceiver()
{
    
}

bool EventReceiver::OnEvent(const SEvent & event)
{
    if(event.EventType == EET_KEY_INPUT_EVENT) {
	if (event.KeyInput.PressedDown) {
	    switch (event.KeyInput.Key) {
	    case irr::KEY_ESCAPE:
		m_Device->closeDevice();
	    default:
		//InputBuffer::GetInstance().InputKeyDown(event.KeyInput.Key);
		break;
	    }
	} else {
	    //InputBuffer::GetInstance().InputKeyUp(event.KeyInput.Key);
	}
    }
    return false;
}
