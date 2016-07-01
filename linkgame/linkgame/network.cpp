#include "network.h"
#include "gamemanager.h"

#pragma comment (lib, "Ws2_32.lib")
extern GameManager gm;
extern NetWorkService netWorkService;

////����ģ��ȫ������
//int flag = -1;		//�ͻ��˱�ʶ�ţ�������ȡ������������
//GameState gamestate = GameState::beforstart;			//��Ϸ״̬����
//char recvbuf[BUFSIZE];
//char sendbuf[BUFSIZE];
//mutex m_recvlock;		//�����������ݻ�����
//mutex m_sendlock;		//�����������ݻ�����
//SOCKET sockClient;
//int recvsize = 0;
//int sendsize = 0;

void networkService()
{
	//��������ģ��
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
	//��ʼ��winsock2��
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();
		return -1;
	}
	//�����׽���
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == INVALID_SOCKET)
	{
		WSACleanup();
		return -1;
	}
	//��ȡip��ַ
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
	//ipv4 ��ַ
	struct sockaddr_in  *sockaddr_ipv4;
	sockaddr_ipv4 = (struct sockaddr_in *) presult->ai_addr;
	sockaddr_ipv4->sin_port = htons(12345);
	sockaddr_ipv4->sin_family = AF_INET;

	if (connect(sockClient, (SOCKADDR*)sockaddr_ipv4, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		return -1;
	}
	//�ͷ���Դ
	freeaddrinfo(presult);

	//�������ӳɹ�֮����շ������˷��͵ı�ʶ��
	recv(sockClient, recvbuf, BUFSIZE, 0);
	flag = getFlag(recvbuf);
	//while (1) {

	//	//�ӷ������������ݵ�����
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

//�ͻ��˷����߳�
void NetWorkService::sendService()
{
	//char buf[] = "hell";
	while (1) {
		//���ͱ�������
		m_sendlock.lock();
		send(sockClient, sendbuf, sendsize, 0);
		m_sendlock.unlock();
	}
}

//��ȡ���������͵�����
int NetWorkService::getDataFromServer(char *data, int bufsize)
{
	m_recvlock.lock();
	//������ת�浽����
	//������ʼ�������ֽڣ������洢��Ϣ��ʶ
	memcpy_s(data, bufsize, &recvbuf[3], recvsize - 3);

	m_recvlock.unlock();
	return recvsize - 3;
}

//���ݱ������ݸ�������
void NetWorkService::sendDataToServer(char *data, int size)
{
	//����������ת�浽�������ݻ�����
	sendsize = size;
	memcpy_s(sendbuf, BUFSIZE, data, size);
	//��������
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
	case 20:								//��Ϸ��ʼ��
		gamestate = GameState::start;
		gm.setGameState(gamestate);
		gm.startGame();
		break;
	case 21:								//�ͻ��˿�ʼ���ͱ�������
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