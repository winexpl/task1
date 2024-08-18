#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 4096
struct Socket {
    Socket(short int=PORT);
    int connect_to();
    int send_to(int);
    int bind_to();
    int recv_from(void* x, size_t n);
    private:
    int sock;
    struct sockaddr_in addr;
};