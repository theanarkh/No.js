
const {
    console,
} = No.buildin;
const { http } = No.libs;
http.createServer({host: '127.0.0.1', port: 8888}, (req, res) => {
    console.log(JSON.stringify(req.headers));
    req.on('data', (buffer) => {
        console.log(buffer);
    });
    req.on('end', () => {
        console.log('end');
    });
    const body  = `<html>
        <head></head>
        <body>
            你好!
        </body>
        </html>`;
    res.setHeaders({
        "Content-Type": "text/html; charset=UTF-8"
    });
    res.end(body);
});