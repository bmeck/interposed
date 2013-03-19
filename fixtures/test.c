#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
int main(int argc, char** argv) {
  int sock;

  struct sockaddr_in sin;
  short PORT = 1000;
  /* complete the socket structure */
  while (1) {
    sock = socket(AF_INET,SOCK_STREAM,0);
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(PORT);
    bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr));
    listen(sock, 128);
    close(sock);
  }
  return 0;
}
