#include "gamemanager.h"
#include <queue>
#include <stack>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <thread>
#include <assert.h>
using namespace std;

GameManager other;
//网络服务对象
NetWorkService netWorkService;

GameManager::GameManager() :gamestate(GameState::beforstart)
{
	startNetworkService();
}

void GameManager::startNetworkService()
{
	//打开网络服务线程
	//'netWorkService.startNetwork();
}

void GameManager::updateGame()
{
	netWorkService.changeGameState();
}

void GameManager::initGame()
{
	int itemcounts[itemtypes];
	itemRandCreate(itemcounts, itemtypes);

	for (int i = 0; i < itemtypes; i++) {
		for (int j = 0; j < itemcounts[i]; j++) {
			bool ok = false;
			Position pos;
			while (!ok) {
				//srand(time(NULL));
				pos.x = rand() % (row);
				//srand(time(NULL));			//生成行号
				pos.y = rand() % (column);		//生成列号
				if (items[pos.x][pos.y].type < 0) {
					items[pos.x][pos.y].type = i;
					items[pos.x][pos.y].pos = pos;
					ok = true;
				}
			}
		}
	}

	//初始化，表示不选中
	start.x = -1;
	start.y = -1;
	flag = 0;
}

void GameManager::itemRandCreate(int *ret, int size)
{
	for (int i = 0; i < size; i++) {
		srand(time(NULL));
		ret[i] = rand() % 3 + 2;
		if (ret[i] % 2) {
			ret[i] += 1;
		}
	}
}

void GameManager::startGame()
{
	netWorkService.getDataFromServer(datarecv, bufsize);
	//data为从服务器获取的游戏初始状态
	//解析data为坐标数据
	vector<int> itempos;
	parseData(0, itempos);
	initGameBoard(itempos);
}

void GameManager::drawOtherClients()
{
	//GameManager other;
	if (gamestate == GameState::start || gamestate == GameState::beforstart)
	{
		other = *this;
		glPushMatrix();
		glTranslatef(column * ITEMWIDTH + 1, 0, 0);
		other.draw();
		glPopMatrix();
		return;
	}

	//获取服务器传递的数据
	memset(datarecv, 0, bufsize);
	int recvsize = netWorkService.getDataFromServer(datarecv, bufsize);
	printf("%s\n", datarecv);
	//解析数据
	char *data = datarecv;
	int clientflag = netWorkService.getClientNum();
	int place = 0;
	//找到';'字符的索引位置
	while (*data != '#' && *data != ';' && *data != '\0') {
		++place;
		++data;
	}
	if (place < recvsize) {

		vector<int> itempos;
		other.emptyBoard();
		if (clientflag == 0) {
			parseData(place + 1, itempos);
			other.initGameBoard(itempos);
		}
		else {
			parseData(0, itempos);
			other.initGameBoard(itempos);
		}
	}

	if (gamestate == GameState::ingame) {
		glPushMatrix();
		glTranslatef(column * (ITEMWIDTH + 1), 0, 0);
		other.draw();
		glPopMatrix();
	}
	//other.draw();
}

void GameManager::draw()
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			items[i][j].draw();
		}
	}

	//绘制边框
	glColor3f(1, 1, 1);
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(0, (row)*ITEMHEIGHT);

	glVertex2f(0, (row)*ITEMHEIGHT);
	glVertex2f((column)*ITEMWIDTH, (row)*ITEMHEIGHT);

	glVertex2f((column)*ITEMWIDTH, (row)*ITEMHEIGHT);
	glVertex2f((column)*ITEMWIDTH, 0);
	glVertex2f((column)*ITEMWIDTH, 0);
	glVertex2f(0, 0);
	glEnd();


	//绘制start item
	if ((start.x >= 0 && start.x < row) && (start.y >= 0 && start.y < column)) {
		GLfloat x, y;
		x = start.y * ITEMWIDTH;
		y = start.x * ITEMHEIGHT;

		glColor3f(1, 1, 1);
		glPushMatrix();
		glTranslatef(x, y, 0);
		glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(0, ITEMHEIGHT);
		glVertex2f(0, ITEMHEIGHT);
		glVertex2f(ITEMWIDTH, ITEMHEIGHT);
		glVertex2f(ITEMWIDTH, ITEMHEIGHT);
		glVertex2f(ITEMWIDTH, 0);
		glVertex2f(ITEMWIDTH, 0);
		glVertex2f(0, 0);
		glEnd();
		glPopMatrix();
	}
}

void GameManager::canClear(int x, int y)
{

	if (findPath) return;
	else {
		for (int k = 0; k < 4; k++) {
			int nx = x + dx[k];
			int ny = y + dy[k];
			if (nx >= 0 && nx < row && ny >= 0 && ny < column && items[nx][ny].type < 0 && pathMap[nx][ny].pathprint == 0) {
				int count = pathMap[x][y].count;
				int turnFlag = pathMap[x][y].turnFlag;
				bool isTurn = false;
				if (k == 0 || k == 2) {
					if (turnFlag == 0) {
						turnFlag = 1;
					}
					else if (turnFlag == 2) {
						turnFlag = 1;
						isTurn = true;
						count++;
					}
				}
				else {
					if (turnFlag == 0) {
						turnFlag = 2;
					}
					else if (turnFlag == 1) {
						turnFlag = 2;
						isTurn = true;
						count++;
					}
				}

				if (count <= 2) {
					pathMap[nx][ny].count = count;
					pathMap[nx][ny].turnFlag = turnFlag;
					pathMap[nx][ny].pathprint = 1;
					if (isTurn && !findPath) {
						path[count] = Position(x, y);
					}
					if (end.x == nx && end.y == ny) {
						findPath = true;
						return;
					}

					canClear(nx, ny);
					pathMap[nx][ny].count = 0;
					pathMap[nx][ny].turnFlag = 0;
					pathMap[nx][ny].pathprint = 0;
				}
			}
		}
	}
}

bool GameManager::canClear()
{
	if (items[start.x][start.y].type != items[end.x][end.y].type)
		return false;

	path[0] = start;
	pathMap[start.x][start.y].pathprint = 1;
	int k = items[end.x][end.y].type;
	items[end.x][end.y].type = -1;
	canClear(start.x, start.y);
	if (!findPath)
	{
		items[end.x][end.y].type = k;

	}
	else {
		path[pathMap[end.x][end.y].count + 1] = end;
	}
	return findPath;

}

void GameManager::clearItems()
{

	//绘制连线
	float h = (start.x + 1) * ITEMHEIGHT;
	float w = (start.y + 1) * ITEMWIDTH;
	int i;
	for (i = 1; i < 4; i++) {
		if (path[i] == end) {
			break;
		}
	}
	for (int k = 0; k < i; k++) {
		float h = (path[k].x) * ITEMHEIGHT;
		float w = (path[k].y) * ITEMWIDTH;
		float h2 = (path[k + 1].x) * ITEMHEIGHT;
		float w2 = (path[k + 1].y) * ITEMWIDTH;
		glLineWidth(2);
		glColor3f(1, 1, 1);
		glBegin(GL_LINES);
		glVertex2f(w + ITEMWIDTH / 2, h + ITEMHEIGHT / 2);
		glVertex2f(w2 + ITEMWIDTH / 2, h2 + ITEMHEIGHT / 2);
		glEnd();
	}
	glFlush();
	for (int i = 0; i < 100000000; i++) {

	}

	items[start.x][start.y].type = -1;
	items[end.x][end.y].type = -1;
	start.x = -1;
	start.y = -1;
	flag = 0;
	findPath = false;
	memset(pathMap, 0, sizeof(pathMap));
}

void GameManager::emptyBoard()
{
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			items[i][j].type = -1;
		}
	}
}

void GameManager::parseData(int s, vector<int> &out)
{
	//从datarecv的第s个字符开始解析数据
	char buf[10];
	int i = 0;
	char *data = datarecv + s;
	//略过前面的空白字符
	while (*data++ == ' ');

	while (*data != ';' && *data != '#' && *data != '\0') {
		if (*data != ' ') {
			buf[i++] = *data;
		}
		else {
			int cord = stringToInterger(buf, i);
			out.push_back(cord);
			i = 0;
		}
		++data;
	}
}
void GameManager::initGameBoard(vector<int> &out)
{
	int i;
	for (i = 0; i < out.size(); i += 3) {

		int x = out[i];
		assert(i + 1 < out.size());
		int y = out[i + 1];
		assert(i + 2 < out.size());
		int t = out[i + 2];
		items[x][y].pos = Position(out[i], out[i + 1]);
		items[x][y].type = t;
	}
}

//向服务器传递本地数据
void GameManager::updateClient()
{
	//转换本地数据为可发送数据
	memset(datasend, 0, bufsize);

	//加上消息号
	/*if (gamestate == GameState::start) {
	memcpy_s(datasend, bufsize, "20%", 3);
	}
	else if (gamestate == GameState::ingame) {
	memcpy_s(datasend, bufsize, "21%", 3);
	}
	else if (gamestate == GameState::pause) {
	memcpy_s(datasend, bufsize, "22%", 3);
	}
	else if (gamestate == GameState::over) {
	memcpy_s(datasend, bufsize, "23%", 3);
	}*/


	int k = 0;
	for (int i = 0; i < row; ++i) {
		for (int j = 0; j < column; ++j) {
			if (items[i][j].type >= 0) {
				k = integerToString(datasend, k, i);
				datasend[k++] = ' ';
				k = integerToString(datasend, k, j);
				datasend[k++] = ' ';
				k = integerToString(datasend, k, items[i][j].type);
				datasend[k++] = ' ';
			}
		}
	}
	/*datasend[k - 1] = '#';*/

	//if(gamestate == GameState::ingame)
	netWorkService.sendDataToServer(datasend, k);
}