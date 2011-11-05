
#include "DisplaySocket.h"

// the constant TCP_BUFSIZE_READ is the maximum size of the standard input
// buffer of TcpSocket
#define RSIZE TCP_BUFSIZE_READ

DisplaySocket::DisplaySocket(ISocketHandler& h) : TcpSocket(h)
{
}

void DisplaySocket::OnRead()
{
	// OnRead of TcpSocket actually reads the data from the socket
	// and moves it to the input buffer (ibuf)
	TcpSocket::OnRead();
	// get number of bytes in input buffer
	size_t n = ibuf.GetLength();
	if (n > 0)
	{
		char tmp[RSIZE]; // <--- tmp's here
		ibuf.Read(tmp,n);
		printf("Read %d bytes:\n",n);
		for (size_t i = 0; i < n; i++)
		{
			printf("%c",isprint(tmp[i]) ? tmp[i] : '.');
		}
		printf("\n");
	}
}
