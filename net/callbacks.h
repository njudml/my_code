//
// Created by duoml on 2020/6/16.
//

#ifndef MY_CODE_CALLBACK_H
#define MY_CODE_CALLBACK_H

#include <functional>
#include <memory>

class Buffer;

class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void()> TimerCallback;
typedef std::function<void(const TcpConnectionPtr &)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr &)> CloseCallback;
typedef std::function<void(const TcpConnectionPtr &)> WriteCompleteCallback;
typedef std::function<void(const TcpConnectionPtr &, size_t)> HighWaterMarkCallback;

// the data has been read to (buf, len)
typedef std::function<void(const TcpConnectionPtr &, Buffer *)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr &conn);

void defaultMessageCallback(const TcpConnectionPtr &conn, Buffer *buffer);

#endif //MY_CODE_CALLBACK_H
