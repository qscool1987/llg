#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <mutex>
using namespace std;
const int BUFSIZE = 4096;
enum GameState { beforstart, start, ingame, pause, over };

//�����̺߳���
void networkService();

//���������
class NetWorkService
{
public:
	NetWorkService() {}

	//��������ģ��
	int startNetwork();

	//��ȡ���������͵�����
	int getDataFromServer(char *data, int bufsize);

	//���ݱ������ݸ�������
	void sendDataToServer(char *data, int bufsize);

	//��ȡ�ͻ��˱�ʶ��
	int getFlag(const char *str);

	GameState getGameStateFromServer();

	//���������߳�
	void sendService();

	//��ȡ��Ϣ����
	int getInfoType();

	//��Ϸ״̬����
	void changeGameState();

	//��ȡ�ͻ��˱�ʶ��
	int getClientNum();

	//�ر���������
	int closeNetWork();

private:

	//����ģ��ȫ������
	int flag = -1;		//�ͻ��˱�ʶ�ţ�������ȡ������������
	GameState gamestate = GameState::beforstart;			//��Ϸ״̬����
	char recvbuf[BUFSIZE];
	char sendbuf[BUFSIZE];
	mutex m_recvlock;		//�����������ݻ�����
	mutex m_sendlock;		//�����������ݻ�����
	SOCKET sockClient;
	int recvsize = 0;
	int sendsize = 0;
};


////�����߳�
//void networkService();
//
////��������ģ��
//int startNetwork();
//
////��ȡ���������͵�����
//int getDataFromServer(char *data, int bufsize);
//
////���ݱ������ݸ�������
//void sendDataToServer(char *data, int bufsize);
//
////��ȡ�ͻ��˱�ʶ��
//int getFlag(const char *str);
//
//GameState getGameStateFromServer();
//
////���������߳�
//void sendService();
//
////��ȡ��Ϣ����
//int getInfoType();
//
////��Ϸ״̬����
//void changeGameState();
//
////��ȡ�ͻ��˱�ʶ��
//int getClientNum();