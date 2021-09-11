const {
    tcp,
    console,
    net,
} = No;
const { constant } = net;

const fd = tcp.socket(constant.domain.AF_INET, constant.type.SOCK_STREAM);
tcp.bind(fd, '127.0.0.1', 18989);
tcp.listen(fd, 512, (clientFd) => {;
    console.log(clientFd);
    // tcp.close(clientFd, (ret) => {
    //     console.log(ret)
    // });
});