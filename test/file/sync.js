const {
    fs,
    console,
    util
} = No.buildin;
const fd = fs.openSync('./test/file/1.txt', fs.constant.FLAG.O_RDWR);
{
    const arr = Buffer.alloc(100);
    const nbytes = fs.readSync(fd, arr);
    console.log(arr);
}

{
    const nbytes = fs.writeSync(fd, Buffer.from('AB'));
    console.log(nbytes);
}
