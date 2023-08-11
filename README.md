# webserver

### 依赖
编译需要依赖`mysqlcppconn`库
```
sudo apt install libmysqlcppconn-dev
```

### 编译
编译源文件
```
./make.sh
```

### 使用
将'include'和`lib`文件夹下的文件移动到相应位置，如：
```
sudo mv include/* /usr/local/include
sudo mv lib/* /usr/local/lib
```

在编译时添加库：
```
g++ main.cpp -lwebserver
```

### 运行
`bin`下的`server`运行：

创建数据库：
```
create database webserver;
use webserver;
create table user(username varchar(50) primary key, password varchar(50));
```

在`server.conf`填写相应的配置信息，详看`server.conf`文件
运行程序
```
./server -i server.conf
```


