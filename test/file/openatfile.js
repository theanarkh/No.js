const {
    fs,
    console
} = No.buildin;
const dirfd = fs.openSync('./');
const fd = fs.openat(dirfd, 'test/server.js');
