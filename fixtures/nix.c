#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>

static int __interposed_CHANNEL_FD = 1;
static int __interposed_active = 0;

#pragma CALL_ON_MODULE_BIND initptrs
#pragma CALL_ON_LOAD initptrs
#pragma init(initptrs)
__attribute__((constructor))
static void __interposed_init()
{
  char* env_channel_fd = getenv("NODE_CHANNEL_FD");
  int found = 0;
  if (env_channel_fd) {
    found = sscanf(env_channel_fd,"%d", &__interposed_CHANNEL_FD);
  }
  if (found) {
    __interposed_active = 1;
  }
}

#define RTLD_SEND(params, ret) __interposed_RTLD_SEND(__FUNCTION__, params, ret)
static void __interposed_RTLD_SEND(const char name[], char* params, char* ret) {
  char msg[4096];
  int length = sprintf(msg, "{\"method\":\"%s\",\"params\":%s,\"result\":%s}\n",name,params,ret);
  write(__interposed_CHANNEL_FD,msg,length);
}

static void sockaddr_json(struct sockaddr_in *in_addr, char addr_str[256]) {
  char str[INET_ADDRSTRLEN];
  int size = inet_ntop(in_addr->sin_family, &(in_addr->sin_addr), str, INET_ADDRSTRLEN);
  snprintf(addr_str, 256, "{\"port\":%d,\"address\":\"%s\"}", ntohs(in_addr->sin_port), str);
}

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
  static const int (*original_bind) (int, const struct sockaddr *, socklen_t) = NULL;
  if (!original_bind) {
    original_bind = dlsym(RTLD_NEXT, "bind");
  }
  if (__interposed_active && addr->sa_family == AF_INET) {
    struct sockaddr_in* in_addr = (struct sockaddr_in*)addr;
    int desired = in_addr->sin_port;
    int result = original_bind(sockfd, addr, addrlen);
    while (result = -1 && (errno == EADDRINUSE || errno == EACCES)) {
      in_addr->sin_port = ntohs(htons(in_addr->sin_port)+1);
      in_addr->sin_addr.s_addr = INADDR_ANY;
      result = original_bind(sockfd, addr, addrlen);
    }
    return result;
  }
  return original_bind(sockfd, addr, addrlen);
}

int listen(int sockfd, int backlog)
{
  static const int (*original_listen) (int, int) = NULL;
  if (!original_listen) {
    original_listen = dlsym(RTLD_NEXT, "listen");
  }
  int result = original_listen(sockfd, backlog);
  if (__interposed_active) {
    struct sockaddr_in addr;
    static socklen_t addr_len = sizeof addr;
    int len = getsockname(sockfd, (struct sockaddr*)&addr, &addr_len);
    if(len <= sizeof addr && addr.sin_family == AF_INET) {
      char ret[15];
      snprintf(ret, 15, "%d", result);
      char params[512];
      char addr_str[256];
      sockaddr_json(&addr, addr_str);
      snprintf(params, 512, "{\"fd\":%d,\"address\":%s,\"backlog\":%d]", sockfd, addr_str, backlog);
      RTLD_SEND(params,ret);
    }
  }
  return result;
}
