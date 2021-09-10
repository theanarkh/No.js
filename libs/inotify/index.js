const {
    inotify,
    console
} = No;

const map = {};

function watch(filename, cb, options) {
    if (map[filename]) {
        map[filename].queue.push({
            cb,
            options
        })
    } else {
        const ctx = inotify.on(filename, () => {
            const { queue = [] } = map[filename];
            for (let i = 0; i < queue.length; i++) {
                const { options, cb } = queue[i];
                cb();
            }
        });
        map[filename] = {
            ctx,
            queue: [{cb, options}]
        };
    }
}

function unwatch(filename, callback) {
    if (!map[filename]) {
        return;
    }
    if (typeof callback !== "function") {
        const { ctx } = map[filename];
        delete map[filename];
        return inotify.off(ctx);
    } else {
        const { queue = [], ctx } = map[filename];
        for (let i = 0; i < queue.length; i++) {
            const { cb } = queue[i];
            if (cb === callback) {
                queue.splice(i, 1);
                break;
            }
        }
        if (!queue.length) {
            return inotify.off(ctx);
        }
        return 0;
    }
    
    
}

watch('/home/cyb/code/No.js/hello.js', () => {
    console.log(2);
    unwatch('/home/cyb/code/No.js/hello.js')
})