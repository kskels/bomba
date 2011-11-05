
#include <Sockets/SocketHandler.h>
#include <Sockets/ListenSocket.h>

#include "DisplaySocket.h"


int main()
{
	SocketHandler h;
	ListenSocket<DisplaySocket> l(h);

	if (l.Bind(12346))
	{
		exit(-1);
	}
	h.Add(&l);
	h.Select(1,0);
	while (true)
	{
		h.Select(1,0);
	}
}

