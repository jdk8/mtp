# mtp

m(ulti) t(hread) p(rogramming) in C++

## 服务端

服务端运行一个server线程和若干worker线程，每个线程维护了一个无锁队列，线程间通过队列通信。

Server线程和客户端建立长连接，收发数据。

Worker线程运行若干actor，actor间通过mail相互调用。

服务端的actor类：

- ServerActor：可以调用NewLocalActor在当前的Worker建立ServerActor。

  方法：

  - CallClient：调用客户端。
  - CallServer：调用服务端。
  - CallRandService：随机调用某个服务的实例。
  - CallShardService：根据key调用某个服务的实例。

- ServiceActor：继承自ServerActor，服务器启动时初始化的actor，通过set_instance指定服务的实例数量。

- ClientService：继承自ServiceActor。只能有一个类继承ClientService，客户端在连上服务器后，会分配到某个ClientService，客户端调用CallClientService直接调用到ClientService。

## 客户端

客户端只有一个线程，和服务端建立长连接，收发数据，运行一个actor，可以调用服务端和其他客户端的actor。

客户端的actor类：

- ClientActor：客户端只能运行一个ClientActor。

  方法：

  - CallClient：调用另一个客户端。
  - CallServer：调用服务端。
  - CallClientService：调用服务端的ClientService。

## 依赖

- protobuf-3.8.0
- boost
- python

## 安装

- cmake .
- make && make install

## 例子

- room: 房间聊天
- word count

## 例子安装

- cd protos && python gen_proto.py
- cd reflect && python gen_reflect.py
- cmake . && make