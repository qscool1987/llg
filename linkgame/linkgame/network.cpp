#include "network.h"
#include "gamemanager.h"

#pragma comment (lib, "Ws2_32.lib")
extern GameManager gm;
extern NetWorkService netWorkService;

////网络模块全局数据
//int flag = -1;		//客户端标识号，用于提取服务器端数据
//GameState gamestate = GameState::beforstart;			//游戏状态变量
//char recvbuf[BUFSIZE];
//char sendbuf[BUFSIZE];
//mutex m_recvlock;		//锁定接收数据缓冲区
//mutex m_sendlock;		//锁定发送数据缓冲区
//SOCKET sockClient;
//int recvsize = 0;
//int sendsize = 0;

void networkService()
{
	//开启网络模块
	netWorkService.startNetwork();
}

int NetWorkService::closeNetWork()
{
	closesocket(sockClient);
	WSACleanup();
	return 0;
}

int NetWorkService::startNetwork()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);
	//初始化winsock2库
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return -1;
	}
	//创建套接字
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == INVALID_SOCKET)
	{
		WSACleanup();
		return -1;
	}
	//获取ip地址
	ADDRINFOA hint, *presult = NULL;
	ZeroMemory(&hint, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	char hostname[128];
	gethostname(hostname, 128);
	err = getaddrinfo(hostname, NULL, &hint, &presult);
	if (err != 0) {
		return -1;
	}
	//ipv4 地址
	struct sockaddr_in  *sockaddr_ipv4;
	sockaddr_ipv4 = (struct sockaddr_in *) presult->ai_addr;
	sockaddr_ipv4->sin_port = htons(12345);
	sockaddr_ipv4->sin_family = AF_INET;

	if (connect(sockClient, (SOCKADDR*)sockaddr_ipv4, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		return -1;
	}
	//释放资源
	freeaddrinfo(presult);

	//建立连接成功之后接收服务器端发送的标识号
	recv(sockClient, recvbuf, BUFSIZE, 0);
	flag = getFlag(recvbuf);
	//while (1) {

	//	//从服务器接收数据到本地
	//	//m_recvlock.lock();
	//	memset(recvbuf, 0, BUFSIZE);
	//	m_recvlock.lock();
	//	recvsize = recv(sockClient, recvbuf, BUFSIZE, 0);
	//	m_recvlock.unlock();
	//	changeGameState();
	//}
	//closesocket(sockClient);
	//WSACleanup();
	return 0;
}

//客户端发送线程
void NetWorkService::sendService()
{
	//char buf[] = "hell";
	while (1) {
		//发送本地数据
		m_sendlock.lock();
		send(sockClient, sendbuf, sendsize, 0);
		m_sendlock.unlock();
	}
}

//获取服务器发送的数据
int NetWorkService::getDataFromServer(char *data, int bufsize)
{
	m_recvlock.lock();
	//将数据转存到本地
	//跳过开始的三个字节，即不存储消息标识
	memcpy_s(data, bufsize, &recvbuf[3], recvsize - 3);

	m_recvlock.unlock();
	return recvsize - 3;
}

//传递本地数据给服务器
void NetWorkService::sendDataToServer(char *data, int size)
{
	//将本地数据转存到发送数据缓冲区
	sendsize = size;
	memcpy_s(sendbuf, BUFSIZE, data, size);
	//发送数据
	send(sockClient, sendbuf, sendsize, 0);
}

int NetWorkService::getFlag(const char *str)
{
	int flag = 0;
	while (*str != '#') {
		flag *= 10;
		flag += *str - '0';
		++str;
	}
	return flag;
}

GameState NetWorkService::getGameStateFromServer()
{
	return gamestate;
}

int NetWorkService::getInfoType()
{
	return getFlag(recvbuf);
}

void NetWorkService::changeGameState()
{
	recvsize = recv(sockClient, recvbuf, BUFSIZE, 0);

	int infoflag = getInfoType();
	switch (infoflag)
	{
	case 20:								//游戏初始化
		gamestate = GameState::start;
		gm.setGameState(gamestate);
		gm.startGame();
		break;
	case 21:								//客户端开始发送本地数据
		gamestate = GameState::ingame;
		gm.setGameState(gamestate);
		break;
	default:
		break;
	}
	//gm.updateClient();
}

int NetWorkService::getClientNum()
{
	return flag;
}