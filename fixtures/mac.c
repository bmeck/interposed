#include <stdio.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define RTLD_SEND(params, ret) __RTLD_SEND(__FUNCTION__, params, ret)
void __RTLD_SEND(const char name[], char* params, char* ret) {
  dprintf(4, "{\"method\":\"%s\",\"params\":%s,\"result\":%s}\n",name,params,ret);
}

void sockaddr_json(const struct sockaddr *addr, char addr_str[128]) {
  struct sockaddr_in* in_addr = (struct sockaddr_in*)addr;
  snprintf(addr_str, 128, "{\"port\":%d}", in_addr->sin_port);
}

// our fopen override implmentation
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  static const int (*original_bind) (int, const struct sockaddr *, socklen_t) = NULL;
  if (!original_bind) {
    original_bind = dlsym(RTLD_NEXT, "bind");
  }
  int result = original_bind(sockfd, addr, addrlen);
  char ret[15];
  snprintf(ret, 15, "%d", result);
  char params[256];
  char addr_str[128];
  sockaddr_json(addr, addr_str);
  snprintf(params, 64, "[%d,%s,%d]", sockfd, addr_str, addrlen);
  RTLD_SEND(params,ret);
  // return the result
  return result;
}

// our fopen override implmentation
int listen(int sockfd, int backlog)
{
  static const int (*original_listen) (int, int) = NULL;
  if (!original_listen) {
    original_listen = dlsym(RTLD_NEXT, "listen");
  }
  int result = original_listen(sockfd, backlog);
  char ret[15];
  snprintf(ret, 15, "%d", result);
  char params[64];
  snprintf(params, 64, "[%d,%d]", sockfd, backlog);
  RTLD_SEND(params,ret);
  // return the result
  return result;
}
