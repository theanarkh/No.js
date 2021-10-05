const {
    unix_domain,
    console,
    net,
} = No.buildin;

// const fds = unix_domain.socketpair(unix_domain.constant.domain.AF_UNIX, unix_domain.constant.type.SOCK_STREAM);
// console.log(fds);

const fd = unix_domain.socket(net.constant.domain.AF_UNIX, net.constant.type.SOCK_STREAM);
unix_domain.bind(fd, "./unix_domain.sock");
unix_domain.listen(fd, 512, (clientFd) => {
    console.log(clientFd);
})
