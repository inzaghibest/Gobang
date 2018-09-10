#include "message.h"
int g_Own = 0; // 自己是黑色
DrawData::DrawData()
{
    point.setX(0);
    point.setY(0);
    isUse = false;
    color = 3;
}

bool DrawData::operator ==(const DrawData& d1)
{
    return this->point == d1.point;
}
