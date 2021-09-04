const {
    console,
    timer
} = No;
const id1 = timer.setTimeout(() => {
    console.log("timeout 3s")
},3000);

const id2 = timer.setTimeout(() => {
    console.log("timeout 5s")
},5000);

const id3 = timer.setTimeout(() => {
    console.log("timeout 7s")
},7000);
// console.log(id1, id2);
// timer.clearTimeout(id2);
