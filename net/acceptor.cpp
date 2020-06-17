//
// Created by duoml on 2020/6/16.
//

#include "acceptor.h"
#include "sockets_ops.h"

#include <co_routine.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <glog/logging.h>


Acceptor::Acceptor(TaskExecutor *executor, const StringArg &ip, uint16_t port, bool reuseport)
		: executor_(executor),
		  acceptSocket_(sockets::createSocket()),
		  listenning_(false),
		  idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
	acceptSocket_.setReuseAddr(true);
	acceptSocket_.setReusePort(reuseport);
	acceptSocket_.bindAddress(ip, port);
}

Acceptor::~Acceptor()
{
	listenning_ = false;
	::close(idleFd_);
}

void Acceptor::listen()
{
	listenning_ = true;
	acceptSocket_.listen();
	executor_->run([this] {
		LOG(INFO) << "server start";
		while (listenning_)
		{
			int connfd = acceptSocket_.accept();
			if (connfd < 0)
			{
				if (errno == EMFILE)
				{
					::close(idleFd_);
					idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
					::close(idleFd_);
					idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
				}
				else
				{
					struct pollfd pf = {0};
					pf.fd = acceptSocket_.fd();
					pf.events = (POLLIN | POLLERR | POLLHUP);
					co_poll(co_get_epoll_ct(), &pf, 1, 1000);
					continue;
				}
			}
			else{
				if(newConnectionCallback_)
					newConnectionCallback_(connfd);
				else
					sockets::close(connfd);
			}
		}
	});
}
