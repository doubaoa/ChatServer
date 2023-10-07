# ChatServer
基于muduo库的 集群聊天服务器 
## 运行环境 
* Linux 
* nginx
编译时需要带tcp模块
* redis
* muduo库 编译安装参考https://blog.csdn.net/QIANGWEIYUAN/article/details/89023980
* hiredis
1. git clone https://github.com/redis/hiredis
2. cd hiredis
3. make
4. sudo make install
* mysql
* cmake

## 编译
执行 ./autobuild.sh 在./bin目录下生成可执行文件ChatServer ChatClient
## nginx配置
在nginx.conf中添加四层负载均衡模块
```c
stream {
    upstream MyServer {
        server 127.0.0.1:6000 weight=1 max_fails=3 fail_timeout=30s;
        server 127.0.0.1:6002 weight=1 max_fails=3 fail_timeout=30s;
    }
    server {
        proxy_connect_timeout 1s;
        listen 8000;
        proxy_pass MyServer;
        tcp_nodelay on;
    }
}
```
## 待完成
* 数据安全性设计与实现
* 分布式设计
* 用户状态缓存
* 消息可靠传输
