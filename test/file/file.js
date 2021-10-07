const {
    fs,
    console
} = No.buildin;
const fd = fs.openSync('./test/file/1.txt');
const arr = Buffer.alloc(100);
fs.readv(fd, arr , 0, (res) => {console.log(res), console.log(arr)});
// console.log(JSON.stringify(fs.constant, null, 4))
//console.log(new Uint8Array(arr));
