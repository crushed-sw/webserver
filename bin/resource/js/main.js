const deg = 6;
const hr = document.querySelector('#hr');
const mn = document.querySelector('#mn');
const sc = document.querySelector('#sc');
// 设置定时器，每秒执行一次
setInterval(() => {
    let day = new Date();
    // 求一小时，一分钟，一秒的角度，一共360度
    let h = day.getHours() * 30;  // 一小时等于30度
    let m = day.getMinutes() * deg;   // 一分钟等于6度
    let s = day.getSeconds() * deg;   // 一秒等于6度
    hr.style.transform = `rotateZ(${h + m / 12}deg)`;
    mn.style.transform = `rotateZ(${m}deg)`;
    sc.style.transform = `rotateZ(${s}deg)`;
}, 1000)
