const {
    console,
    timer
} = No.buildin;

const id = timer.setInterval(() => {
    console.log("timeout");
    //timer.clearInterval(id);
}, 1000, 1000);
