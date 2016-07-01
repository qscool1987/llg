#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <mutex>
using namespace std;
const int BUFSIZE = 4096;
enum GameState { beforstart, start, ingame, pause, over };

//网络线程函数
void networkService();

//网络服务类
class NetWorkService
{
public:
	NetWorkService() {}

	//启动网络模块
	int startNetwork();

	//获取服务器发送的数据
	int getDataFromServer(char *data, int bufsize);

	//传递本地数据给服务器
	void sendDataToServer(char *data, int bufsize);

	//提取客户端标识号
	int getFlag(const char *str);

	GameState getGameStateFromServer();

	//发送数据线程
	void sendService();

	//提取消息类型
	int getInfoType();

	//游戏状态控制
	void changeGameState();

	//获取客户端标识号
	int getClientNum();

	//关闭网络连接
	int closeNetWork();

private:

	//网络模块全局数据
	int flag = -1;		//客户端标识号，用于提取服务器端数据
	GameState gamestate = GameState::beforstart;			//游戏状态变量
	char recvbuf[BUFSIZE];
	char sendbuf[BUFSIZE];
	mutex m_recvlock;		//锁定接收数据缓冲区
	mutex m_sendlock;		//锁定发送数据缓冲区
	SOCKET sockClient;
	int recvsize = 0;
	int sendsize = 0;
};


////网络线程
//void networkService();
//
////启动网络模块
//int startNetwork();
//
////获取服务器发送的数据
//int getDataFromServer(char *data, int bufsize);
//
////传递本地数据给服务器
//void sendDataToServer(char *data, int bufsize);
//
////提取客户端标识号
//int getFlag(const char *str);
//
//GameState getGameStateFromServer();
//
////发送数据线程
//void sendService();
//
////提取消息类型
//int getInfoType();
//
////游戏状态控制
//void changeGameState();
//
////获取客户端标识号
//int getClientNum();