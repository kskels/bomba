
#include <Sockets/TcpSocket.h>
#include <Sockets/ISocketHandler.h>

class DisplaySocket : public TcpSocket
{
public:
	DisplaySocket(ISocketHandler& );

	void OnRead();
};

