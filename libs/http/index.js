const {
    tcp,
    console,
    net,
    HTTPParser,
} = No;
const { constant } = net;
class Server extends No.libs.tcp.Server {
    constructor(options = {}, cb) {
        super(options);
        this.options = options;
        if (typeof cb === 'function') {
            this.on('request', cb);
        }
        this.on('connection', (socket) => {
            new HTTPRequest({socket, server: this});
        });
    }
}

class HTTPRequest extends No.libs.events {
    socket = null;
    httpparser = null;
    constructor({socket, server}) {
        super();
        this.server = server;
        this.socket = socket;
        this.httpparser = new HTTPParser();
        this.httpparser.onHeaderComplete = (data) => {
            this.major = data.major;
            this.minor = data.minor;
            this.keepalive = data.keepalive;
            this.upgrade = data.upgrade;
            this.headers = data.headers;
            const response = new HTTPResponse({socket, server});
            this.server.emit('request', this, response);
        }
        this.httpparser.onBody = (data) => {
            this.emit('data', data);
        }
        this.httpparser.onBodyComplete = (data) => {
            // console.log(data);
        }
        
        socket.on('data', (buffer) => {
            this.httpparser.parse(buffer);
        });
        socket.on('end', () => {
            this.emit('end');
        });
    }
}

class HTTPResponse extends No.libs.events {
    socket = null;
    constructor({socket, server}) {
        super();
        this.server = server;
        this.socket = socket;
    }
    write(buffer) {
        this.socket.write(buffer);
    }
}

function createServer(...arg) {
    return new Server(...arg);
}

module.exports = {
    createServer
}