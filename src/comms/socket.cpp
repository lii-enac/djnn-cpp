#include "socket.h"

#include "core/utils/error.h"

namespace djnn {

SOCKET
socket_open_client (const char* _addr, int _port)
{
    struct sockaddr_in serv_addr;
    SOCKET             _fd;
    if ((_fd = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        warning (nullptr, "Failed to create socket");
        perror ("");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = htons (_port);

    if (inet_pton (AF_INET, _addr, &serv_addr.sin_addr) == SOCKET_ERROR) {
        warning (nullptr, "Failed to configure socket");
        perror ("");
        return -1;
    }
    if (connect (_fd, (struct sockaddr*)&serv_addr, sizeof (serv_addr)) == SOCKET_ERROR) {
        warning (nullptr, "Failed to connect socket");
        perror ("");
        return -1;
    }
    return _fd;
}

SOCKET
socket_open_server (int _port)
{
    struct sockaddr_in serv_addr;
    SOCKET             _fd;
    if ((_fd = socket (AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        warning (nullptr, "Failed to create socket");
        perror ("");
        return -1;
    }
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons (_port);
    int reuse                 = 1;

    if (setsockopt (_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof (reuse)) == SOCKET_ERROR) {
        warning (nullptr, "setsockopt(SO_REUSEADDR) failed");
        perror ("");
        return -1;
    }
    if (::bind (_fd, (struct sockaddr*)&serv_addr, sizeof (serv_addr)) == SOCKET_ERROR) {
        warning (nullptr, "ERROR on binding");
        perror ("");
        return -1;
    }
    return _fd;
}

int
socket_close (SOCKET sock)
{
    int status = 0;
#ifdef _WIN32
    status = shutdown (sock, SD_BOTH);
    if (status == 0) {
        status = closesocket (sock);
    }
#else
    status = shutdown (sock, SHUT_RDWR);
    if (status == 0) {
        status = close (sock);
    }
#endif
    return status;
}
} // namespace djnn
