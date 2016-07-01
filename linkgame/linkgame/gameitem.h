#pragma once
#include "text.h"
#include <gl/glut.h>

/*
** �����������̴�СΪ25*20��һ�����25��λͼ��һ�����20��λͼ
** ÿ��С����Ĵ�СΪ20*20���أ���Ļ��СΪ800*600���������0-500���������200-600
** ��һ��λ����Ļ���·�����20��λ����Ļ���Ϸ�����һ��λ����Ļ���󷽣����һ��λ����Ļ���ҷ�
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
	int  type;				//��ʶ����������ͼƬ��0-35
	Position pos;			//λ�����꣬���Ի��㵽��Ļ����
	GameItem() :type(-1) {}
	GameItem(int t, const Position _pos) :type(t), pos(_pos) {}

	virtual void draw();	//����type��pos���Ƴ���Ԫ��

private:

};
