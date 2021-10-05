const {
    signal,
    console,
    process,
    timer,
} = No.buildin;
signal.on(signal.constant.SIG.SIGUSR1, () => {
    console.log("135");
});
signal.on(signal.constant.SIG.SIGUSR1, () => {
    console.log("246");
    //process.exit();
});
// for keep process alive
timer.setInterval(() => {
    
},10000, 10000);
