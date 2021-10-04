const net = require('net');
function handle() {
   //  setTimeout(() => {
        const socket = net.connect({host: '127.0.0.1', port: 8888});
        socket.on('connect', () => {
            console.log('connect');
            socket.write('GET /1.html HTTP/1.1\r\nhello:world\r\ncontent-length:1');
            setTimeout(() => {
                socket.write('\r\n\r\na\r\n\r\n');
            }, 1000)
            // socket.destroy();
            socket.on('data', (buffer) => {
                console.log(buffer.toString('utf-8'));
            });
            // socket.on('end', () => {
            //     console.log('end');
            //     handle();
            // });
        });
 //   }, 1000);
}

handle();
