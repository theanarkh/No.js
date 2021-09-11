
const { watch } = require('inotify');
watch('/home/cyb/code/No.js/hello.js', () => {
    console.log(2);
    unwatch('/home/cyb/code/No.js/hello.js')
})