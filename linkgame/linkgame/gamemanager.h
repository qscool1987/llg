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


	//标识选中的两个item
	Position start, end;
	//用于标识选中了几个item，初始值为0，选中start后置为1，选中end后进行判断
	//如果能消除则将这对item消除，置为0，如果不能消除，则将start置为end，flag不变
	int flag;

	//
	bool findPath = false;
	int dx[4] = { 0,-1,0,1 };
	int dy[4] = { 1,0,-1,0 };
	//保存路径上的拐点
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

	//启动网络模块
	void startNetworkService();

	//初始化游戏
	void initGame();
	void startGame();

	//item随机生成器
	void itemRandCreate(int *ret, int size);

	//绘制
	void draw();

	//消除
	void clearItems();

	void canClear(int x, int y);
	//判断能否消除
	bool canClear();

	//清空棋盘，从新开始
	void emptyBoard();

	//获取游戏状态
	GameState getGameState()
	{
		return gamestate;
	}
	//设置游戏状态
	void setGameState(GameState state)
	{
		gamestate = state;
	}
	//改变游戏状态
	void changeGameState();
	//绘制其他客户端
	void drawOtherClients();
	//更新数据并发送到服务器转发
	void updateClient();
	//更新游戏状态
	void updateGame();

private:
	void parseData(int s, vector<int> &out);
	void initGameBoard(vector<int> &out);
};
