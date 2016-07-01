#pragma once
#include "text.h"
#include <gl/glut.h>

/*
** 连连看的棋盘大小为25*20即一行最多25张位图，一列最多20张位图
** 每个小区域的大小为20*20像素，屏幕大小为800*600，横坐标从0-500，纵坐标从200-600
** 第一行位于屏幕最下方，第20行位于屏幕最上方，第一列位于屏幕最左方，最后一列位于屏幕最右方
**/

#define ITEMWIDTH 20
#define ITEMHEIGHT 20

struct Position {
	int x;
	int y;
	Position() :x(-1), y(-1) {}
	Position(int _x, int _y) :x(_x), y(_y) {}
	bool operator == (const Position &des) {
		return (x == des.x && y == des.y);
	}
	bool operator != (const Position &des) {
		return !((*this) == des);
	}
};

class GameItem {
public:
	int  type;				//标识是那种纹理图片从0-35
	Position pos;			//位置坐标，可以换算到屏幕区域
	GameItem() :type(-1) {}
	GameItem(int t, const Position _pos) :type(t), pos(_pos) {}

	virtual void draw();	//根据type和pos绘制出该元素

private:

};
