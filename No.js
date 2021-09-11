const {
    loader,
    process,
    console,
} = No;
function loaderNativeModule() {
    const modules = [
        {
            module: 'libs/module/index.js',
            name: 'module'
        },
        {
            module: 'libs/inotify/index.js',
            name: 'inotify'
        }
    ];
    No.libs = {};
    for (let i = 0; i < modules.length; i++) {
        const module = {
            exports: {},
        };
        loader.compile(modules[i].module).call(null, loader.compile, module.exports, module);
        No.libs[modules[i].name] = module.exports;
        
    }
}

function runMain() {
    No.libs.module.load(process.argv[1]);
}

loaderNativeModule();
runMain();
