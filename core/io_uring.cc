#include "io_uring.h"
#include <stdio.h>
struct sockaddr sa;
	socklen_t sa_size = sizeof(sa);
// 向内核提交一个请求
void No::io_uring::SubmitRequest(struct request * req, struct io_uring_info *io_uring_data) {
    // 获取一个io_uring的请求结构体
    struct io_uring_sqe *sqe = io_uring_get_sqe(&io_uring_data->ring);
    // 填充请求
    switch (req->op)
    {
        
        case IORING_OP_READ:
            {
                struct io_request * file_req = (struct io_request *)req;
                io_uring_prep_read(sqe, file_req->fd, file_req->buf, file_req->len, file_req->offset);
                break;
            }
        case IORING_OP_WRITE:
           {
                struct io_request * file_req = (struct io_request *)req;
                io_uring_prep_write(sqe, file_req->fd, file_req->buf, file_req->len, file_req->offset);
                break;
           }

        case IORING_OP_READV:
            {
                struct io_batch_request * file_req = (struct io_batch_request *)req;
                io_uring_prep_readv(sqe, file_req->fd, file_req->iovecs, file_req->nvecs, file_req->offset);
                break;
            }
        case IORING_OP_WRITEV:
           {
                struct io_batch_request * file_req = (struct io_batch_request *)req;
                io_uring_prep_writev(sqe, file_req->fd, file_req->iovecs, file_req->nvecs, file_req->offset);
                break;
           }
        case IORING_OP_ACCEPT: 
        {
            struct tcp_request * tcp_req = (struct tcp_request *)req;
            io_uring_prep_accept(sqe, tcp_req->fd, &sa,
                            &sa_size, 0);
                break;
        }
        case IORING_OP_OPENAT: 
        {
            struct openat_file_request * open_file_req = (struct openat_file_request *)req;
            io_uring_prep_openat(sqe, open_file_req->dirFd, open_file_req->path,
                            open_file_req->flags, open_file_req->mode);
                break;
        }
        case IORING_OP_CONNECT: 
        {
            struct connect_request * connect_req = (struct connect_request *)req;
            io_uring_prep_connect(sqe, connect_req->fd, connect_req->addr, connect_req->addrlen);
                break;
        }
        case IORING_OP_CLOSE: 
        {
            struct close_request * close_req = (struct close_request *)req;
            io_uring_prep_close(sqe, close_req->fd);
            break;
        }
        case IORING_OP_NOP: 
        {

            io_uring_prep_nop(sqe);
            break;
        }
       
        default:
            return;
    }
    ++io_uring_data->pending;
    // 保存请求上下文，响应的时候用
    io_uring_sqe_set_data(sqe, (void *)req);
	io_uring_submit(&io_uring_data->ring);
}

void No::io_uring::incPending(struct io_uring_info *io_uring_data) {
    ++io_uring_data->pending;
}

void No::io_uring::decPending(struct io_uring_info *io_uring_data) {
    --io_uring_data->pending;
}

void No::io_uring::RunIOUring(struct io_uring_info *io_uring_data) {
    struct io_uring* ring = &io_uring_data->ring;
    struct io_uring_cqe* cqe;
    struct request* req;
    while(io_uring_data->stop != 1 && io_uring_data->pending != 0) {
        // 提交请求给内核
	    int count = io_uring_submit_and_wait(ring, 1);
      	// 处理每一个完成的请求
        while (1) { 
            io_uring_peek_cqe(ring, &cqe);
            if (cqe == NULL)
                break;
            --io_uring_data->pending;
            // 拿到请求上下文
            req = (struct request*) (uintptr_t) cqe->user_data;
            req->res = cqe->res;
            io_uring_cq_advance(ring, 1);
            // 执行回调
            if (req->cb != nullptr) {
                req->cb((void *)req);
            }
        }
    }
}

// void No::io_uring::RunIOUring(struct io_uring_info *io_uring_data) {
//     struct io_uring* ring = &io_uring_data->ring;
//     struct io_uring_cqe* cqe;
//     struct request* req;
//     while(io_uring_data->stop != 1 && io_uring_data->pending != 0) {
//             int ret = io_uring_wait_cqe(ring, &cqe);
//             --io_uring_data->pending;
//             // 拿到请求上下文
//             req = (struct request*) (uintptr_t) cqe->user_data;
//             req->res = cqe->res;
//             // 执行回调
//             req->cb((void *)req);
//             io_uring_cqe_seen(ring, cqe);
//          }
//     }
// }

void No::io_uring::Stop(struct io_uring_info *io_uring_data) {
    io_uring_data->stop = 1;
}
