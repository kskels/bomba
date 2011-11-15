#ifndef _EVENTRECEIVER_H_
#define _EVENTRECEIVER_H_

using namespace irr;

class EventReceiver : public IEventReceiver
{
public:

	EventReceiver();
	EventReceiver(IrrlichtDevice* device);
	virtual ~EventReceiver();

private:

	virtual bool OnEvent(const SEvent & event);

	IrrlichtDevice* m_Device;

};

#endif /* _EVENTRECEIVER_H_ */
