#pragma once
#include <gl/glut.h>

class TextHelper {
public:
	TextHelper();
	GLvoid TextOuts(int x, int y, const char *pc);
	GLvoid TextOuts(int x, int y, const char pc);
	GLvoid TransformDigitToChar(GLfloat value, char *p, GLuint size);

private:
	GLubyte texts[256];
};

