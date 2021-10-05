const {
    vm,
    console,
    util
} = No.buildin;

{
    const ret = vm.run("function sum() { let sum = 0; for (let i = 0; i < 100; i++) { sum += i; } return sum; }; sum();", {mode: vm.MODE.CODE});
    console.log(ret);
}

{
    const ret = vm.run('test/vm/code.js', { mode: vm.MODE.FILE });
    console.log(ret);
}

{
    const fun = vm.compileFunction('return 1 + 1', { mode: vm.MODE.CODE });
    console.log(fun());
}

{
    const fun = vm.compileFunction('test/vm/compileCode.js', { mode: vm.MODE.FILE });
    console.log(fun());
}

{
    const ret = vm.compileFunction('return arguments[0]', { mode: vm.MODE.CODE, call: true, paramValues: ["hello"]});
    console.log(ret);
}

{
    const fun = vm.compileFunction('return a + b', { mode: vm.MODE.CODE, paramNames: ["a", "b"]});
    console.log(fun(1,2));
}

{
    const ret = vm.compileFunction('return a + b', { mode: vm.MODE.CODE, call: true, paramNames: ["a", "b"], paramValues: [1, 2] });
    console.log(ret);
}

{
    const func = vm.compileFunction('return a + b', { mode: vm.MODE.CODE, paramNames: ["a", "b"], paramValues: [1, 2] });
    console.log(func(1,2));
}

{
    const func = vm.compileFunction('return a + b', { mode: vm.MODE.CODE, extensions: [{a: 1, b: 2}] });
    console.log(func(1,2));
}

{
    const func = vm.compileFunction('return a + b', { mode: vm.MODE.CODE, extensions: [{a: 1}, {b: 2}] });
    console.log(func(1,2));
}

{
    const ret = vm.compileFunction('return a + b', { mode: vm.MODE.CODE, call: true, extensions: [{a: 1}, {b: 2}] });
    console.log(ret);
}