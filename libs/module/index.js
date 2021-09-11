const {
    loader,
} = No;

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
    static load(...args) {
        const module = new Module(...args);
        return module.load();
    }
};

module.exports = Module;