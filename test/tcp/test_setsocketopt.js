const {
    tcp,
    console,
    net,
} = No;
const { constant } = net;

{
    const fd = tcp.socket(constant.domain.AF_INET, constant.type.SOCK_STREAM);
    tcp.setsockopt(fd, constant.level.SOL_SOCKET, constant.socketOption.SO_REUSEPORT, 1);
    console.log(tcp.bind(fd, '127.0.0.1', 18989));
}

{
    const fd = tcp.socket(constant.domain.AF_INET, constant.type.SOCK_STREAM);
    tcp.setsockopt(fd, constant.level.SOL_SOCKET, constant.socketOption.SO_REUSEPORT, 1);
    console.log(tcp.bind(fd, '127.0.0.1', 18989));
}