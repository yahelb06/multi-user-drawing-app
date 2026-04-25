#pragma once
#include "Pch.h"

class Server
{

public:

	Server();
	~Server();
	void startHandleRequest();

private:

	void bindAndListen(int port) const;
	void handleNewClient();
	void clientHandler(SOCKET clientSocket);

	SOCKET _serverSocket;
};