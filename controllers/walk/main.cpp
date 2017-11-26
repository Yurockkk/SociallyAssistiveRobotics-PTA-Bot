// Description:   Manage the entree point function

#include "Walk.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define SOCKET_PORT 10020
#define SOCKET_SERVER "127.0.0.1"   /* local host */

using namespace webots;

bool initClient(){
  struct sockaddr_in address;
  struct hostent *server;
  int fd, rc;
  //int n;
  //char buffer[256];
  /* create the socket */
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        printf("cannot create socket\n");

        return -1;
    }
    
    /* fill in the socket address */
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(SOCKET_PORT);
    server = gethostbyname(SOCKET_SERVER);

    if (server) {
        memcpy((char *) &address.sin_addr.s_addr, (char *) server->h_addr,
               server->h_length);
    } else {
        printf("cannot resolve server name: %s\n", SOCKET_SERVER);

        return -1;
    }

    /* connect to the server */
    rc = connect(fd, (struct sockaddr *) &address, sizeof(struct sockaddr));
    if (rc == -1) {
        printf("cannot connect to the server\n");

        return -1;
    }
    //TODO when to trigger
    return 1;
}

int main()
{
  
  if(initClient()){
    Walk *controller = new Walk();
    controller->run();
    delete controller;
  }
  return EXIT_FAILURE;
}


