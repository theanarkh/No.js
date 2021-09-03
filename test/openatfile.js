const {
    fs,
    console
} = No;
const dirfd = fs.open('./');
const fd = fs.openat(dirfd, 'test/server.js');
