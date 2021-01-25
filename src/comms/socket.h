#pragma once

#ifdef _WIN32
  /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
  #include <unistd.h> /* Needed for close() */
  #define INVALID_SOCKET -1
  #define SOCKET int
  #define SOCKET_ERROR -1
#endif

namespace djnn {
SOCKET socket_open_client (const char* addr, int port);
SOCKET socket_open_server (int port);
int socket_close (SOCKET sock);
}
