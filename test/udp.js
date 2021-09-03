const {
    udp,
    console,
    net,
} = No;
const { constant } = net;

const fd = udp.socket(constant.domain.AF_INET, constant.type.SOCK_DGRAM);
udp.bind(fd, '127.0.0.1', 8888);
udp.connect(fd, '127.0.0.1', 41234);
// udp.bind(fd, '127.0.0.1', 18989);
const buf = new ArrayBuffer(1);
const data = new Uint8Array(buf);
data[0] = 65;
udp.sendto(fd, buf, 0);
const recvBuf = new ArrayBuffer(1);
udp.recvfrom(fd, recvBuf, 0);

console.log(new Uint8Array(recvBuf));