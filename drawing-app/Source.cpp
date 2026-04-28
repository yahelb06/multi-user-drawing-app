
#include "Server.h"
#include <exception>
#include "RequestHandlerFactory.h"
#include "SqliteDatabase.h"
int main()
{
	try
	{
		SqliteDatabase db;
		RequestHandlerFactory factory(&db);
		Server myServer(factory);

		std::thread t(&Server::startHandleRequest, &myServer);
		std::cout << "Server is running\n";
		t.join();
	}
	catch (std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}
	system("PAUSE");
	return 0;
}