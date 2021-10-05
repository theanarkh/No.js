const {
    loader,
} = No.buildin;

const map = {};
class Module {
    constructor(filename) {
        this.filename = filename;
        this.exports = {};
    }
    load() {
        const result = loader.compile(this.filename);
        result.call(this, Module.load, this.exports, this);
        return this.exports;
    }
    static load(filename, ...args) {
        if (map[filename]) {
            return map[filename];
        }
        const module = new Module(filename, ...args);
        return (map[filename] = module.load());
    }
};

module.exports = Module;