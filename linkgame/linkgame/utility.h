#pragma once
//常用功能函数

int stringToInterger(char *buf, int bufsize);

//将val转换成字符序列，存储与buf中的s索引位置
//返回存储val之后，buf的当前位置
int integerToString(char *buf, int s, int val);

//通信流程
//1服务器开启
//2客户端接入，服务器分配标识
//3游戏开始
//消息类型
//1

