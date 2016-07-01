#include "utility.h"

//将字符串变为整数
int stringToInterger(char *buf, int bufsize)
{
	int ans = 0;
	int i = 0;
	while (i < bufsize) {
		ans *= 10;
		ans += buf[i] - '0';
		i++;
	}
	return ans;
}

int integerToString(char *buf, int s, int val)
{
	if (val >= 10) {
		buf[s++] = val / 10 + '0';
		buf[s++] = val % 10 + '0';
	}
	else {
		buf[s++] = val + '0';
	}
	return s;
}