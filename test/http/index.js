
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
});