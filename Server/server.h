#include <sys/types.h>
#include <string.h>
#include <string>
class Server
{
private:
    int listeningSocket;
    sockaddr_in hint;

public:
    Server(int);
    Server(int, std::string);
    ~Server();
    int listen();
};