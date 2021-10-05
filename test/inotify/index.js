
const { watch, unwatch } = require(`${process.cwd()}/libs/inotify/index.js`);
watch('/home/cyb/code/No.js/test/inotify/hello.js', () => {
    console.log(2);
    unwatch('/home/cyb/code/No.js/test/inotify/hello.js')
})