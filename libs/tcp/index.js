const {
    tcp,
    console,
    net,
} = No.buildin;
const { constant } = net;
const { events } = No.libs;
class Server extends events {
    fd = -1;
    connections = 0;
    constructor(options = {}) {
        super();
        const fd = tcp.socket(constant.domain.AF_INET, constant.type.SOCK_STREAM);
        this.fd = fd;
        tcp.bind(fd, options.host, options.port);
        tcp.listen(fd, 512, (clientFd) => {
            this.connections++;
            const serverSocket = new ServerSocket({fd: clientFd});
            this.emit('connection', serverSocket);
        });
    }
}

class Socket extends events {
    fd = -1;
    write(buffer) {
        if (typeof buffer === 'string') {
            buffer = Buffer.from(buffer).getBuffer();
        }
        tcp.write(this.fd, buffer);
    }
}

class ClientSocket extends Socket {
    constructor(options = {}) {
        super(options);
        const fd = tcp.socket(constant.domain.AF_INET, constant.type.SOCK_STREAM);
        tcp.bind(fd, '127.0.0.1', 18989);
    }
}

class ServerSocket extends Socket {
    constructor(options = {}) {
        super(options);
        this.fd = options.fd;
        this.read();
    }
    read() {
        const buffer = Buffer.alloc(1024);
        tcp.read(this.fd, buffer.getBuffer(), 0, (status) => {
            if (status === 0) {
                this.emit('end');
            } else if (status > 0){
                this.emit('data', buffer);
                this.read();
            } else {
                this.emit('error', new Error('read socket error'));
            }
        })
    }
}
function createServer(...arg) {
    return new Server(...arg);
}

module.exports = {
    createServer,
    Server,
}