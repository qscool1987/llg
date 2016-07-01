#include "gameitem.h"

extern TextHelper textHelper;


void GameItem::draw()	//根据type和pos绘制出该元素
{
	if (type < 0) return;

	char flag;

	if (type >= 10) {
		flag = 'A' + type - 10;
	}
	else {
		flag = '0' + type;
	}

	//获取绘制区域
	GLfloat sx, sy;
	sy = ITEMHEIGHT*pos.x;
	sx = ITEMWIDTH*pos.y;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glColor3f(1, 0, 0);
	glLineWidth(1);
	glTranslatef(sx, sy, 0);
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
	glPopAttrib();


	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glTranslatef(ITEMWIDTH / 4, ITEMHEIGHT / 4, 0);
	glColor3f(0, 1, 1);
	textHelper.TextOuts(sx, sy, flag);
	glPopMatrix();
	glPopAttrib();
}