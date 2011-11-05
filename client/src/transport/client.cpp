
#include <Sockets/SocketHandler.h>
#include <Sockets/TcpSocket.h>

#include <iostream>
#include <cstdlib>


class SendSocket : public TcpSocket
{
public:
	SendSocket(ISocketHandler& h) : TcpSocket(h) {}
	SendSocket(ISocketHandler& h, const std::string& data) : 
        TcpSocket(h), m_data(data) {}

	void OnConnect() {
		SetLineProtocol();
		if (m_data.empty())
			Send("Random text.\n");
		else
			Send(m_data + "\n");
	}

private:
	std::string m_data;

};

int main(int argc, char* argv[])
{
    if (argc < 2 || !argv[1] || !argv[2]) {
        std::cout << "Usage: client <host> <port>" << std::endl;
        return -1;
    }
	SocketHandler h;
    SendSocket sock(h, "OnConnect: Gotchyaaa..");
	sock.Open(argv[1], atoi(argv[2]));
	h.Add(&sock);
	h.Select(1, 0);
    sock.Send("OnKaspars: Bye bye..");
}

