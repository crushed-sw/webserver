let loginButton = document.querySelector("#a-container .submit");
let registerButton = document.querySelector("#b-container .submit");

let loginUsername = document.querySelector("#a-container .username");
let loginPassword = document.querySelector("#a-container .password");

let registerUsername = document.querySelector("#b-container .username");
let registerPassword = document.querySelector("#b-container .password");
let registerConfirmPassword = document.querySelector("#b-container .confirmpassword");

function showError(message, duration) {
  const notification = document.querySelector('.notification');
  notification.textContent = message;
  notification.classList.add('error');

  setTimeout(function() {
    notification.classList.remove('error');
  }, duration);
}

function showInfo(message, duration) {
  const notification = document.querySelector('.notification');
  notification.textContent = message;
  notification.classList.add('success');

  setTimeout(function() {
    notification.classList.remove('success');
  }, duration);
}

function isStrRight(str) {
    var regex = /^[a-zA-Z0-9_]+$/;
    return regex.test(str);
}

loginButton.addEventListener("click", function(){
    var username = loginUsername.value;
    var password = loginPassword.value;

    if(username === "" || password === "") {
        showError("用户名和密码不能为空", 3000);
    } else if(!isStrRight(username) || !isStrRight(password)) {
        showError("用户名和密码必须为 数字，字母，下划线组成", 3000);
    } else {
        axios.get('/login', {
            params: {
                username: username,
                password: password
            }})
            .then(function(response){
                var code = response.data.code;
                var data = response.data.data;

                if(code === 20001) {
                    window.location.href = "/html/main.html";
                    localStorage.setItem('username', username);
                } else {
                    showError(data, 3000);
                }
            })
            .catch(function(error){
                console.log(error);
            });
    }
});

registerButton.addEventListener("click", function(){
    var username = registerUsername.value;
    var password = registerPassword.value;
    var confirmPassword = registerConfirmPassword.value;

    if(username === "" || password === "" || confirmPassword === "") {
        showError("用户名和密码不能为空", 3000);
    } else if(password !== confirmPassword) {
        showError("两次输入密码不一致", 3000);
    } else if(!isStrRight(password) || !isStrRight(username)) {
        showError("用户名和密码必须为 数字，字母，下划线组成", 3000);
    } else if(password.length < 6) {
        showError("密码长度必须大于6", 3000);
    } else {
        axios.post('/register', {
                username: username,
                password: password
            })
            .then(function(response){
                var code = response.data.code;
                var data = response.data.data;

                if(code === 20001) {
                    showInfo("注册成功", 3000);
                } else {
                    showError(data, 3000);
                }
            })
            .catch(function(error){
                console.log(error);
            });
    }
});


