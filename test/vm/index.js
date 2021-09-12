const {
    vm,
    console,
    util
} = No;

{
    const ret = vm.run("function sum() { let sum = 0; for (let i = 0; i < 100; i++) { sum += i; } return sum; }; sum();", {mode: vm.MODE.CODE});
    console.log(ret);
}

{
    const ret = vm.run('test/vm/code.js', {mode: vm.MODE.FILE});
    console.log(ret);
}