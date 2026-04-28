#pragma once
#include "Pch.h"
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"


class Server
{

public:

	Server(RequestHandlerFactory& factory);
	~Server();
	void startHandleRequest();

private:
	
	void bindAndListen(int port) const;
	void handleNewClient();
	void clientHandler(SOCKET clientSocket);

	RequestHandlerFactory& m_handlerFactory;
	std::mutex userListMutex;

	std::map<SOCKET, IRequestHandler*> m_client;
	SOCKET _serverSocket;
};