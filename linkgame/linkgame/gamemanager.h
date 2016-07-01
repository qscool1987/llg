#pragma once
#include "gameitem.h"
#include "utility.h"
#include "network.h"
#include <vector>
class GameManager {
public:
	enum { row = 10, column = 10, itemtypes = 10, bufsize = 4096 };
	GameItem items[row][column];
	GameState gamestate;
	char datarecv[bufsize];
	char datasend[bufsize];


	//��ʶѡ�е�����item
	Position start, end;
	//���ڱ�ʶѡ���˼���item����ʼֵΪ0��ѡ��start����Ϊ1��ѡ��end������ж�
	//��������������item��������Ϊ0�����������������start��Ϊend��flag����
	int flag;

	//
	bool findPath = false;
	int dx[4] = { 0,-1,0,1 };
	int dy[4] = { 1,0,-1,0 };
	//����·���ϵĹյ�
	Position path[4];

	struct PathMap {
		PathMap() :count(0), turnFlag(0) {}
		int count;
		int turnFlag;
		int pathprint;
	};

	PathMap pathMap[row][column];

	GameManager();

	bool hasItem(int x, int y)
	{
		return (items[x][y].type >= 0);
	}

	//��������ģ��
	void startNetworkService();

	//��ʼ����Ϸ
	void initGame();
	void startGame();

	//item���������
	void itemRandCreate(int *ret, int size);

	//����
	void draw();

	//����
	void clearItems();

	void canClear(int x, int y);
	//�ж��ܷ�����
	bool canClear();

	//������̣����¿�ʼ
	void emptyBoard();

	//��ȡ��Ϸ״̬
	GameState getGameState()
	{
		return gamestate;
	}
	//������Ϸ״̬
	void setGameState(GameState state)
	{
		gamestate = state;
	}
	//�ı���Ϸ״̬
	void changeGameState();
	//���������ͻ���
	void drawOtherClients();
	//�������ݲ����͵�������ת��
	void updateClient();
	//������Ϸ״̬
	void updateGame();

private:
	void parseData(int s, vector<int> &out);
	void initGameBoard(vector<int> &out);
};
