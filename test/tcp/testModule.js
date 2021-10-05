const {
    console,
    libs
} = No.buildin;
const server = libs.tcp.createServer({host: '127.0.0.1', port: 8888});
server.on('connection', (socket) => {
    socket.on('data', (buffer) => {
        console.log(new Uint8Array(buffer));
    })
})
// const { constant } = net;

// const fd = tcp.socket(constant.domain.AF_INET, constant.type.SOCK_STREAM);
// tcp.bind(fd, '127.0.0.1', 18989);
// tcp.listen(fd, 512, (clientFd) => {;
//     console.log(clientFd);
//     // tcp.close(clientFd, (ret) => {
//     //     console.log(ret)
//     // });
// });