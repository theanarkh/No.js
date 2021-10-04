
const {
    console,
} = No;
let i = 0;
const { http } = No.libs;
http.createServer({host: '127.0.0.1', port: 8888}, (req, res) => {
    // console.log(JSON.stringify(req.headers));
    req.on('data', (buffer) => {
        console.log(buffer);
    });
    req.on('end', () => {
        console.log('end');
    });
    const body  = `<html>
        <head></head>
        <body>
            hello world    
        </body>
        </html>`;
    const response = `HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-length: ${body.length}\r\n\r\n${body}\r\n\r\n`
    const responseBuffer = new ArrayBuffer(response.length);
    const bytes = new Uint8Array(responseBuffer);
    for (let i = 0; i < response.length; i++) {
        bytes[i] = response[i].charCodeAt(0);
    }
    res.write(responseBuffer);
});