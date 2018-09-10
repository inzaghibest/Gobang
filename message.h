/* 提供基础定义 */
#ifndef MESSAGE_H
#define MESSAGE_H
#include <QPoint>
/* 棋子信息定义 */
class DrawData
{
public:
    QPoint point; // 点坐标
    int color; // 0:黑色 1:白色
    bool isUse; // 是否使用 true:使用 false:未使用
    bool operator ==(const DrawData& d1); // 判断是否为同一个棋子,坐标相同即可
public:
    DrawData();
};


#endif // MESSAGE_H
