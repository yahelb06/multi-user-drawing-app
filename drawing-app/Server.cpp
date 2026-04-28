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
	//one extra index
	char buffer[BUFFER_SIZE + 1];
	int size = 0;
	std::string users;
	std::string name;
	int userMsgSize;
	IRequestHandler* handler = nullptr;
	try
	{
		while (true)
		{
			userMsgSize = recv(clientSocket, buffer, BUFFER_SIZE, 0);
			if (userMsgSize <= 5 || userMsgSize >= BUFFER_SIZE)
			{
				throw std::exception();
			}
			buffer[userMsgSize] = '\0';
			Buffer vecBuffer;
			vecBuffer.insert(vecBuffer.begin(), buffer + START_OF_DATA, buffer + userMsgSize);
			for (auto& a : vecBuffer)
			{
				std::cout << a;
			}
			std::cout << "\n";
			RequestInfo info = { buffer[0], time_t(), vecBuffer };
			{
				std::lock_guard<std::mutex> lock(userListMutex);
				handler = m_client[clientSocket];
			}
			RequestResult res = handler->handlerRequest(info);

			send(clientSocket, reinterpret_cast<const char*>(res.response.data()), res.response.size(), 0);
		
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
		closesocket(clientSocket);
	}
}