#include <math.h>
#include "text.h"
TextHelper::TextHelper()
{
	for (GLuint i = 0; i <= 255; i++) {
		texts[i] = i;
	}
}

GLvoid TextHelper::TextOuts(int x, int y, const char *p)
{

	const char *pt = p;
	glRasterPos2i(x, y);

	while (*pt) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, texts[*pt]);
		pt++;
	}
}

GLvoid TextHelper::TextOuts(int x, int y, const char p)
{
	glRasterPos2i(x, y);

	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, texts[p]);
}

GLvoid TextHelper::TransformDigitToChar(GLfloat value, char *p, GLuint size)
{
	GLint i = 0;
	if (value < 0) {
		p[i++] = '-';
		value = -value;
	}

	GLint d = value;
	if (d != 0) {
		while (d != 0) {
			p[i++] = d % 10 + '0';
			d /= 10;
		}
		p[i++] = '.';
	}
	else {
		p[i++] = '0';
		p[i++] = '.';
	}

	int j;
	if (p[0] == '-')
		j = 1;
	else
		j = 0;
	for (GLint k = i - 2; j < k; j++, k--) {
		GLubyte m = p[j];
		p[j] = p[k];
		p[k] = m;
	}

	d = (GLint)value;
	GLfloat f = value - d;
	while (f < 1 && f > 0) {
		f *= 10;
		p[i++] = (GLint)f + '0';
		f -= (GLint)f;
		if (i >= size - 2)
			break;
	}
	p[i] = '\0';
}