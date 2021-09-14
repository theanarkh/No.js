const {
    fs,
    console
} = No;
const dirfd = fs.openSync('./');
const fd = fs.openat(dirfd, 'test/server.js');
