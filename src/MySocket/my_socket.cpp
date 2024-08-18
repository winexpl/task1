#include "my_socket.hpp"
#include <cstddef>


Socket::Socket(short int port) {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}
int Socket::connect_to() {
    return connect(sock, (struct sockaddr *)&addr, sizeof(addr));
}
int Socket::send_to(int sum) {
    return send(sock, &sum, sizeof(sum), 0);
}
int Socket::bind_to() {
    return bind(sock, (struct sockaddr *)&addr, sizeof(addr));
}
int Socket::recv_from(void* x, size_t n) {
    return recvfrom(sock, x, n, 0, NULL, NULL);
}
