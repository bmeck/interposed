#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
int main(int argc, char** argv) {
  int sock = socket(AF_INET,SOCK_STREAM,0);

  struct sockaddr_in sin;
  int      PORT = 1337;
  /* complete the socket structure */
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(PORT);

  bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr));
  listen(sock, 128);
  return 0;
}
