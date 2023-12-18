function handleLogin() {
    var username = document.getElementById('username').value;
    var password = document.getElementById('password').value;

    // 创建一个新的XMLHttpRequest对象
    var xhr = new XMLHttpRequest();

    // 设置请求方法和URL
    xhr.open('POST', '/login', true);

    // 设置请求头
    xhr.setRequestHeader('Content-Type', 'application/json');

    // 监听请求状态变化
    xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 200) {
                // 请求成功，处理返回的数据
                var response = JSON.parse(xhr.responseText);
                if (response.success) {
                    // 登录成功，跳转到聊天页面
                    if (window.confirm('登录成功，点击确定跳转到聊天页面')) {
                        window.location.href = 'chat.html';
                    }
                } else {
                    // 登录失败，显示错误消息
                    alert(response.message);
                }
            } else {
                // 请求失败，显示错误消息
                alert('请求服务器失败');
            }
        }
    };

    // 将用户名和密码作为数据发送给后端
    var data = JSON.stringify({ username: username, password: password });
    xhr.send(data);
}

function handleRegister() {
    var username = document.getElementById('username').value;
    var password = document.getElementById('password').value;

    var xhr = new XMLHttpRequest();

    xhr.open('POST', '/register', true);

    xhr.setRequestHeader('Content-Type', 'application/json');

    xhr.onreadystatechange = function() {
        if (xhr.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 200) {
                var response = JSON.parse(xhr.responseText);
                if (response.success) {
                    if (window.confirm('注册成功，点击确定跳转到登录页面')) {
                        window.location.href = 'login.html';
                    }
                } else {
                    alert(response.message);
                }
            } else {
                alert('请求服务器失败');
            }
        }
    };

    var data = JSON.stringify({ username: username, password: password });
    xhr.send(data);
}