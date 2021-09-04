const net = require('net');
function handle() {
   //  setTimeout(() => {
        const socket = net.connect({host: '127.0.0.1', port: 18989});
        socket.on('connect', () => {
            console.log('connect');
            socket.destroy();
            // socket.on('data', (buffer) => {
            //     console.log(buffer.toString('utf-8'));
            // });
            // socket.on('end', () => {
            //     console.log('end');
            //     handle();
            // });
        });
 //   }, 1000);
}

handle();
