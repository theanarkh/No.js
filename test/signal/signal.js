const {
    signal,
    console,
    process,
} = No;
signal.on(signal.constant.SIG.SIGUSR1, () => {
    console.log("135");
});
signal.on(signal.constant.SIG.SIGUSR1, () => {
    console.log("246");
    //process.exit();
});
while(1) {

}