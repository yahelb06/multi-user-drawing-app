#include "Server.h"


Server::Server(RequestHandlerFactory& factory) : m_handlerFactory(factory)
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0)
	{
		throw std::runtime_error("WSAStartup failed with error: " + std::to_string(result));
	}
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_serverSocket == INVALID_SOCKET)
		throw std::runtime_error(std::string(__FUNCTION__) + " - socket");
}

Server::~Server()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(_serverSocket);
		for (auto& item : m_client)
		{
			delete item.second;
		}
		WSACleanup();
	}
	catch (...) {}
}

void Server::startHandleRequest()
{
	bindAndListen(PORT);
	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		handleNewClient();
	}
}

void Server::updateClientHandler(SOCKET clientSocket, IRequestHandler* newHandler)
{
	std::lock_guard<std::mutex> lock(userListMutex);
	if (m_client.find(clientSocket) != m_client.end())
	{
		//delete m_client[clientSocket];
		m_client[clientSocket] = newHandler;
	}
}

void Server::bindAndListen(int port) const
{
	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (::bind(_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::runtime_error(std::string(__FUNCTION__) + " - bind");

	// Start listening for incoming requests of clients
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::runtime_error(std::string(__FUNCTION__) + " - listen");
	std::cout << "Listening on port " << port << std::endl;
}

void Server::handleNewClient()
{
	// this accepts the client and create a specific socket from server to this client
// the process will not continue until a client connects to the server
	SOCKET client_socket = accept(_serverSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::runtime_error(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;
	{
		std::lock_guard<std::mutex> lock(userListMutex);
		this->m_client[client_socket] = m_handlerFactory.CreateLoginRequest();
	}
	// the function that handle the conversation with the client
	std::thread t(&Server::clientHandler, this, client_socket);
	t.detach();
}

void Server::clientHandler(SOCKET clientSocket)
{
	std::string users;
	std::string name;
	int userMsgSize;
	IRequestHandler* currentHandler = nullptr;
	try
	{
		while (true)
		{
			char header[7];
			int bytesRead = 0;

			while (bytesRead < 7)
			{
				int bytesReturn = recv(clientSocket, header + bytesRead, 7 - bytesRead, 0);
				if (bytesReturn <= 0)
				{
					throw std::exception();
				}
				bytesRead += bytesReturn;
			}
			char messageCode = header[0];
			std::string lengthStr(header + 1, 6);
			int jsonLength = std::stoi(lengthStr);
			if (jsonLength <= 0)
			{
				throw std::exception();
			}
			std::vector<char> jsonBuffer(jsonLength + 1, '\0');
			int totalBytesReceived = 0;
			while (totalBytesReceived < jsonLength)
			{
				int bytesReturn = recv(clientSocket, jsonBuffer.data() + totalBytesReceived, jsonLength - totalBytesReceived, 0);
				if (bytesReturn <= 0)
				{
					throw std::exception();
				}
				totalBytesReceived += bytesReturn;
			}
			Buffer vecBuffer(jsonBuffer.begin(), jsonBuffer.begin() + jsonLength);
			for (auto& ch : vecBuffer)
			{
				std::cout << ch;
			}
			std::cout << "\n";
			RequestInfo info = { messageCode, time_t(), vecBuffer, clientSocket };
			{
				std::lock_guard<std::mutex> lock(userListMutex);
				if (m_client.find(clientSocket) != m_client.end())
				{
					currentHandler = m_client[clientSocket];
				}
			}
			if (currentHandler == nullptr)
			{
				return;
			}
			RequestResult res = currentHandler->handlerRequest(info);
			if (res.newHandler != currentHandler)
			{
				std::lock_guard<std::mutex> lock(userListMutex);

				if (m_client.find(clientSocket) != m_client.end())
				{
					delete currentHandler;
					this->m_client[clientSocket] = res.newHandler;
				}
			}
			for (auto& ch : res.response)
			{
				std::cout << ch;
			}
			std::cout << "\n";
			if (!sendAll(clientSocket, reinterpret_cast<const char*>(res.response.data()), res.response.size()))
			{
				throw std::exception();
			}
		}
	}
	catch (const nlohmann::json::parse_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const nlohmann::json::out_of_range& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Client socket closed" << std::endl;
		std::string usernameToLogout = this->m_handlerFactory.getLoginManager().getUserBySocket(clientSocket);
		if (!usernameToLogout.empty())
		{
			this->m_handlerFactory.getLoginManager().logout(usernameToLogout, clientSocket);
		}
		closesocket(clientSocket);

		std::lock_guard<std::mutex> lock(userListMutex);
		auto it = m_client.find(clientSocket);
		if (it != m_client.end())
		{
			delete it->second;
			m_client.erase(it);
		}
	}
}

bool Server::sendAll(SOCKET socket, const char* data, int length)
{
	int totalBytesSent = 0;
	while (totalBytesSent < length)
	{
		int bytesSent = send(socket, data + totalBytesSent, length - totalBytesSent, 0);
		if (bytesSent == SOCKET_ERROR)
		{
			return false;
		}
		totalBytesSent += bytesSent;
	}
	return true;
}

