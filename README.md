# myproject
项目练习代码

## 自己实现http server 
- 1.tcp通讯,linux网络接口的使用
- 2.http协议,读取并解析报头,构建响应,发送数据
- 3.线程池技术,生产者消费者模型,互斥锁和条件变量
- 4.单例设计, CGI编写使用:网络计算器,mysql数据库访问(注册),todo: 日期计算器,搜索引擎(代理)
- todo: IO多路复用(reactor模型), 音乐播放功能, 留言板功能, http/1.1长链接功能, 视频播放功能? 网络.js小游戏功能?
 "尝试学习实现http/1.1长链接功能, 目前难点:线程延时退出计时, alarm不行,timer_create待学习"

## 高并发内存池
- 1. 定长内存池
- 2. TC malloc (Thread Cache Memory Allocate)简化版实现