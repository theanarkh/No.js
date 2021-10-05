const { console } = No.buildin;

const log = (...args) => {
    for (let i = 0; i< args.length; i++) {
        const text = args[i];
        if (text instanceof Buffer) {
            text = text.getBytes();
        }
        console.log(text);
    }
}
module.exports = {
    log
}