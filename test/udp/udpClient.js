const {
    udp,
    console,
    net,
} = No.buildin;
const { constant } = net;

const fd = udp.socket(constant.domain.AF_INET, constant.type.SOCK_DGRAM);
udp.bind(fd, '127.0.0.1', 8888);
udp.connect(fd, '127.0.0.1', 41234);
// udp.bind(fd, '127.0.0.1', 18989);
const data = Buffer.from('A');
udp.sendto(fd, data, 0);
const recvBuf = Buffer.alloc(1);
udp.recvfrom(fd, recvBuf, 0);

console.log(recvBuf);