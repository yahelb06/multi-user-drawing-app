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
	static bool sendAll(SOCKET socket, const char* data, int length);

	void setPendingHandler(SOCKET clientSocket, IRequestHandler* newHandler);
	IRequestHandler* checkAndApplyPendingHandler(SOCKET clientSocket);

private:
	
	void bindAndListen(int port) const;
	void handleNewClient();
	void clientHandler(SOCKET clientSocket);

	RequestHandlerFactory& m_handlerFactory;
	std::mutex userListMutex;

	std::map<SOCKET, std::unique_ptr<IRequestHandler>> m_client;
	SOCKET _serverSocket;

	std::map<SOCKET, std::unique_ptr<IRequestHandler>> m_pendingClients;
};