#include <math.h>
#include <iostream>
#include <vector>
#include "text.h"
#include "time.h"
#include <gl/glut.h>
#include "gamemanager.h"
using namespace std;



GLint winw = 500, winh = 300;
GLfloat theta = 0.0f;

TextHelper textHelper;

GameManager gm;
GameTimer gtimer;

GLuint texbg;
extern NetWorkService netWorkService;

//加载纹理图片
unsigned int LoadTex(string Image)
{
	unsigned int Texture;

	FILE* img = NULL;
	fopen_s(&img, Image.c_str(), "rb");

	unsigned long bWidth = 0;
	unsigned long bHeight = 0;
	long size = 0;

	fseek(img, 18, SEEK_SET);
	fread(&bWidth, 4, 1, img);
	fread(&bHeight, 4, 1, img);
	fseek(img, 0, SEEK_END);
	size = ftell(img) - 54;

	unsigned char *data = (unsigned char*)malloc(size);

	fseek(img, 54, SEEK_SET); // image data
	fread(data, size, 1, img);

	fclose(img);

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bWidth, bHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_BGR_EXT, bWidth, bHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);
	if (data)
		free(data);

	return Texture;
}

GLvoid drawBackGround()
{
	glBindTexture(GL_TEXTURE_2D, texbg);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(0, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(0.0f, winh);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(winw, winh);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(winw, 0.0f);
	glEnd();
}

void frameFrenquency(GameTimer &gtimer)
{
	//输出帧频
	GLfloat frequency = gtimer.Frequency();
	char buffer[20];
	textHelper.TransformDigitToChar(frequency, buffer, 20);
	textHelper.TextOuts(0, winh - 20, buffer);
}

void myDisplay(void)
{
	gtimer.Tick();
	//glColor3f(1.0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	frameFrenquency(gtimer);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gm.updateGame();
	gm.draw();
	gm.drawOtherClients();
	//向服务器发送本地数据
	gm.updateClient();

	glFlush();
}
void init()
{
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	//加载背景
	//texbg = LoadTex("img/1.bmp");
	glEnable(GL_TEXTURE_2D);
	netWorkService.startNetwork();
	//开启网络模块
	//gm.initGame();

}

void winReshapeFcn(GLint w, GLint h)
{
	winw = w;
	winh = h;


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, w, 0, h, -300, 300);
}

void mousedown(int button, int state, int x, int y)
{
	//转换坐标 旋转，平移
	int sx = x;
	int sy = winh - y;

	sx = sx / ITEMWIDTH;
	sy = sy / ITEMHEIGHT;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {	//记录第一次按下

		if (gm.flag == 0) {
			if (sx >= 0 && sx < gm.column && sy >= 0 && sy < gm.row && gm.hasItem(sy, sx)) {
				gm.start.x = sy;
				gm.start.y = sx;
				gm.flag = 1;
			}
		}
		else if (gm.flag == 1) {
			if (sx >= 0 && sx < gm.column && sy >= 0 && sy < gm.row &&
				gm.hasItem(sy, sx) && (sx != gm.start.y || sy != gm.start.x)) {
				gm.end.x = sy;
				gm.end.y = sx;
				//判断能否清除这对items
				if (gm.canClear()) {
					gm.clearItems();
				}
				else {
					gm.start = gm.end;

				}
			}
		}

	}

	glutPostRedisplay();
}

void idle()
{
	theta += 10.0f;
	if (theta >= 360.0f) {
		theta = theta - 360.0f;
	}
	glutPostRedisplay();
}

GLvoid menuControl(GLint option)
{
	switch (option) {
	case 1:
	{
		gm.emptyBoard();
		gm.initGame();
	}
	break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(winw, winh);
	glutCreateWindow("连连看");
	init();
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(winReshapeFcn);
	glutMouseFunc(mousedown);
	glutIdleFunc(idle);
	glutCreateMenu(menuControl);
	glutAddMenuEntry("重新开始", 1);
	glutAddMenuEntry("暂停", 2);
	glutAddMenuEntry("开始", 3);
	glutAddMenuEntry("退出", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();

	return 0;
}