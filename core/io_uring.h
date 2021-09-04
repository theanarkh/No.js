#ifndef IO_URING_H
#define IO_URING_H

#include "../include/io_uring/liburing.h"
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
       #include <netinet/in.h>

#define QUEUE_DEPTH 1
#define BLOCK_SZ    1024

// io_uring相关的结构体
struct io_uring_info {
  int32_t pending;
  struct io_uring ring;
  int stop;
};


typedef void (*request_cb)(void *);

enum {
    IO_URING_REQUEST_AGAIN = 1
};

#define REQUEST \
        int op; \
        request_cb cb; \
        int res;\
        void * data; \
        int flag;

struct request {
    REQUEST
};

struct close_request {
    REQUEST
    int fd;
};

struct io_request {
    REQUEST
    int fd; 
    int offset; 
    void *buf;
    int len; 
};

struct io_batch_request {
    REQUEST 
    int fd; 
    int offset; 
    int nvecs;
    // 数据
    struct iovec iovecs[];    
};

struct tcp_request {
    REQUEST
    int fd;
};

struct openat_file_request {
    REQUEST
    int dirFd;
    char * path;
    int flags;
    mode_t mode;
};

struct connect_request {
    REQUEST
    int fd;
    struct sockaddr *addr;
	socklen_t addrlen;
};

namespace No {
    namespace io_uring {
        void SubmitRequest(struct request * req, struct io_uring_info *io_uring_data);
        void Stop(struct io_uring_info *io_uring_data);
        void RunIOUring(struct io_uring_info *io_uring_data);
        void incPending(struct io_uring_info *io_uring_data); 
        void decPending(struct io_uring_info *io_uring_data); 
    }
}

#endif
