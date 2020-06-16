//
// Created by duoml on 2020/6/16.
//

#ifndef MY_CODE_SOCKETS_OPS_H
#define MY_CODE_SOCKETS_OPS_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdint>

namespace sockets
{
int create_socket();

int connect(int sockFd, const struct sockaddr *addr);

void listen(int sockFd);

void bind(int sockFd, const char *ip, uint16_t port);

void close(int sockFd);

void shutdownWrite(int sockFd);

int getSocketError(int sockFd);

struct sockaddr_in getLocalAddr(int sockFd);

struct sockaddr_in getPeerAddr(int sockFd);

int setNoDelay(int sockFd,bool on);

int setReuseAddr(int sockFd,bool on);

int setReusePort(int sockFd,bool on);

void setKeepAlive(int sockFd,bool on);

void setCloseWait(int sockFd,int seconds);

const struct sockaddr *sockaddr_cast(const struct sockaddr_in *addr);

const struct sockaddr_in *sockaddr_in_cast(const struct sockaddr *addr);
struct sockaddr *sockaddr_cast(struct sockaddr_in *addr);
struct sockaddr_in *sockaddr_in_cast(struct sockaddr *addr);
}

#endif //MY_CODE_SOCKETS_OPS_H
