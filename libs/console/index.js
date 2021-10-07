const { console } = No.buildin;

const log = (...args) => {
    for (let i = 0; i< args.length; i++) {
        const text = args[i];
        console.log(text);
    }
}
module.exports = {
    log
}