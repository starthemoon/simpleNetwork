# simpleNetwork

## 功能

- 1. 采用C/S架构和tcp链接，实现了服务端与客户端的简易聊天功能

- 2. 由客户端发送“close”字符串发起结束请求，断开tcp链接

- 3. 服务端接收到tcp链接后，自动开启新线程负责与单个客户端进行聊天功能

- 4. 现在支持客户端在聊天内容中发送空格