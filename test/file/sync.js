const {
    fs,
    console,
    util
} = No;
const fd = fs.openSync('./test/file/1.txt', fs.constant.FLAG.O_RDWR);
{
    const arr = new ArrayBuffer(100);
    const nbytes = fs.readSync(fd, arr);
    console.log(new Uint8Array(arr));
}

{
    const arr = new ArrayBuffer(1);
    const byteArr = new Uint8Array(arr);
    byteArr[0] = 65;
    const nbytes = fs.writeSync(fd, arr);
    console.log(nbytes);
}
