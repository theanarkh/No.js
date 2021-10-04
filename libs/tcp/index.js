const {
    tcp,
    console,
    net,
} = No;
const { constant } = net;

class Server {
    fd = -1;
    connections = 0;
    constructor(options = {}) {
        const fd = tcp.socket(constant.domain.AF_INET, constant.type.SOCK_STREAM);
        this.fd = fd;
        tcp.bind(fd, options.host, options.port);
        tcp.listen(fd, 512, (clientFd) => {
            this.connections++;
            new ServerSocket({fd: clientFd});
        });
    }
}

class Socket {
    fd = -1;
}

class ClientSocket extends Socket {
    constructor(options = {}) {
        const fd = tcp.socket(constant.domain.AF_INET, constant.type.SOCK_STREAM);
        tcp.bind(fd, '127.0.0.1', 18989);
    }
}

class ServerSocket extends Socket {
    constructor(options = {}) {
        this.fd = options.fd;
        this.read();
    }
    read() {
        const buffer = new ArrayBuffer(1024);
        tcp.read(fd, buffer, 0, (status) => {
            if (status == -1) {

            } else {
                console.log(buffer)
            }
        })
    }
}
function createServer(...arg) {
    return new Server(...arg);
}

module.exports = {
    createServer
}