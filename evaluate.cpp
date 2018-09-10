/* 局势判断函数的实现 */
#include "evaluate.h"
#include "score.h"
#include "message.h"
#include <QDebug>
int Length = 420; // 棋盘长度
#define DEBUG
// 棋型判断
long AnalysisOfChessShape(int ChessColor[], int continueCount, int leftBreak,
                          int rightBreak, int hiscolor)
{
    if(continueCount == 5)    // 中心线五连
    {
#ifdef DEBUG
        qDebug()<<"成五";
#endif
        return WIN5; // 成五
    }else if(continueCount == 4) // 中心线4连
    {
        #ifdef DEBUG
        qDebug()<<"-------四连---------";
        #endif
        // 两边均为空,活4
        if(ChessColor[leftBreak] == -1 && ChessColor[rightBreak] == -1)
        {
            qDebug()<<"活4";
            return ALIVE4; // 活4
        }// 两边均为非空
        else if(ChessColor[leftBreak] != -1 && ChessColor[rightBreak] != -1)
        {
           // qDebug()<<"NOTHING";
            return NOTHING; // 没有威胁
        } // 两边有一个为空
        else if(ChessColor[leftBreak] == -1 || ChessColor[rightBreak] == -1)
        {
            qDebug()<<"死4";
            return DIE4; // 死4
        }
       // qDebug()<<"NOTHING";
        return NOTHING; // 没有威胁
    }else if(continueCount == 3) // 中心线三连
    {
        //qDebug()<<"-------三连---------";
        int colorleft1 = ChessColor[leftBreak - 1];
        int colorright1 = ChessColor[rightBreak + 1];
        int colorleft = ChessColor[leftBreak];
        int colorright = ChessColor[rightBreak];
        if (colorleft == -1 && colorright == -1)//两边断开位置均空
        {
            // 均为对手棋子
            if (colorleft1 == (hiscolor + 1)%2 && colorright1 == (hiscolor + 1)%2)
            {
                qDebug()<<"DIE3";
                return DIE3; // 死3
            }else if(colorleft1 == hiscolor && colorright1 == hiscolor) // 两边均为自己棋子
            {
                qDebug()<<"活4";
                return ALIVE4; // 活4
            }
            else if (colorleft1 == hiscolor || colorright1 == hiscolor) //只要一个为自己的棋子
            {
                qDebug()<<"低级死4";
                return LOWDIE4;
            }
            else if (colorleft1 == -1 || colorright1 == -1)//只要有一个空
            {
                //qDebug()<<"活3";
                return ALIVE3; // 活3
            }else
            {
               // qDebug()<<"NOTHING";
                return NOTHING; // 没有威胁
            }
        }
        else if (colorleft == (hiscolor+1)%2 && colorright == (hiscolor+1)%2)//两边断开位置均非空
        {
            //qDebug()<<"NOTHING";
            return NOTHING;
        }
        else if (colorleft == -1 || colorright == -1)//两边断开位置只有一个空
        {

            if (colorleft == (hiscolor+1)%2) {//左边被对方堵住
                if (colorright1 == (hiscolor+1)%2)//右边也被对方堵住
                {
                    //qDebug()<<"NOTHING";
                    return NOTHING; // 没有威胁
                }
                else if (colorright1 == -1)//右边空
                {
                    qDebug()<<"DIE3";
                    return DIE3; // 死3
                }
                else if (colorright1 == hiscolor) //右边为自己颜色棋子
                {
                    qDebug()<<"低级死4";
                    return LOWDIE4; // 低级死4
                }else
                {
                    //qDebug()<<"NOTHING";
                    return NOTHING;
                }
            }
            else if (colorright == (hiscolor+1)%2) {//右边被对方堵住
                if (colorleft1 == (hiscolor+1)%2)//左边也被对方堵住
                {
                    //qDebug()<<"NOTHING";
                    return NOTHING;
                }
                else if (colorleft1 == -1)//左边空
                {
                    qDebug()<<"DIE3";
                    return DIE3;
                }
                else if (colorleft1 == hiscolor)//左边还有自己的棋子
                {
                    qDebug()<<"LOWDIE4";
                    return LOWDIE4;
                }
            }
        }
        //qDebug()<<"NOTHING";
       return NOTHING;
    }else if(continueCount == 2) // 中心二连
    {
        //qDebug()<<"-------二连---------";
        int colorleft1 = ChessColor[leftBreak - 1];
        int colorright1 = ChessColor[rightBreak + 1];
        int colorleft2 = ChessColor[leftBreak - 2];
        int colorright2 = ChessColor[rightBreak + 2];
        int colorleft = ChessColor[leftBreak];
        int colorright = ChessColor[rightBreak];
        int mycolor = hiscolor;
        int hiscolor = (hiscolor + 1)%2;
        if (colorleft == -1 && colorright == -1)//两边断开位置均空
        {
            if ((colorright1 == -1 && colorright2 == mycolor) ||
                (colorleft1 == -1 && colorleft2 == mycolor))
            {
                qDebug()<<"DIE3";
                return DIE3;
            }
            else if (colorleft1 == -1 && colorright1 == -1)
            {
                qDebug()<<"ALIVE2";
                return ALIVE2;
            }
            else if ((colorright1 == mycolor && colorright2 == hiscolor) ||
                (colorleft1 == mycolor && colorleft2 == hiscolor))
            {
                qDebug()<<"DIE3";
                return DIE3;
            }
            else if ((colorright1 == mycolor && colorright2 == mycolor) ||
                (colorleft1 == mycolor && colorleft2 == mycolor))
            {
                qDebug()<<"低级死4";
                return LOWDIE4;
            }
            else if ((colorright1 == mycolor && colorright2 == -1) ||
                (colorleft1 == mycolor && colorleft2 == -1))
            {
                qDebug()<<"跳活3";
                return JUMPLIVE3;
            }
            qDebug()<<"NOTHING";
            return NOTHING;
        }
        else if (colorleft == hiscolor && colorright == hiscolor)//两边断开位置均非空
        {
            qDebug()<<"NOTHING";
            return NOTHING;
        }
        else if (colorleft == -1 || colorright == -1)//两边断开位置只有一个空
        {
            if (colorleft == hiscolor) {//左边被对方堵住
                if (colorright1 == hiscolor || colorright2 == hiscolor) {//只要有对方的一个棋子
                    qDebug()<<"NOTHING";
                    return NOTHING;//没有威胁
                }
                else if (colorright1 == -1 && colorright2 == -1) {//均空
                    qDebug()<<"死2";
                    return DIE2;//死2
                }
                else if (colorright1 == mycolor && colorright2 == mycolor) {//均为自己的棋子
                    qDebug()<<"低级死4";
                    return LOWDIE4;
                }
                else if (colorright1 == mycolor || colorright2 == mycolor) {//只有一个自己的棋子
                    qDebug()<<"死3";
                    return DIE3;
                }
                //qDebug()<<"NOTHING";
                return NOTHING;
            }
            if (colorright == hiscolor) {//右边被对方堵住
                if (colorleft1 == hiscolor || colorleft2 == hiscolor) {//只要有对方的一个棋子
                    //qDebug()<<"NOTHING";
                    return NOTHING;//没有威胁
                }
                else if (colorleft1 == -1 && colorleft2 == -1) {//均空
                    qDebug()<<"DIE2";
                    return DIE2;//死2
                }
                else if (colorleft1 == mycolor && colorleft2 == mycolor) {//均为自己的棋子
                    qDebug()<<"低级死4";
                    return LOWDIE4;
                }
                else if (colorleft1 == mycolor || colorleft2 == mycolor) {//只有一个自己的棋子
                    qDebug()<<"死3";
                    return DIE3;
                }
                //qDebug()<<"NOTHING";
                return NOTHING;
            }
        }
    }else if(continueCount == 1) // 中心线一连
    {
        //qDebug()<<"-------一连---------";
        int colorleft1 = ChessColor[leftBreak - 1];
        int colorright1 = ChessColor[rightBreak + 1];
        int colorleft2 = ChessColor[leftBreak - 2];
        int colorright2 = ChessColor[rightBreak + 2];
        int colorleft3 = ChessColor[leftBreak - 3];
        int colorright3 = ChessColor[rightBreak + 3];
        int colorleft = ChessColor[leftBreak];
        int colorright = ChessColor[rightBreak];
        int mycolor = hiscolor;
        int hiscolor = (hiscolor + 1) % 2;
        if (colorleft == -1 && colorleft1 == mycolor &&
            colorleft2 == mycolor && colorleft3 == mycolor)
        {
            qDebug()<<"低级死4";
            return LOWDIE4;
        }
        else if (colorright == -1 && colorright1 == mycolor &&
            colorright2 == mycolor && colorright3 == mycolor)
        {
            qDebug()<<"低级死4";
            return LOWDIE4;
        }
        else if (colorleft == -1 && colorleft1 == mycolor &&
            colorleft2 == mycolor && colorleft3 == -1 && colorright == -1)
        {
            qDebug()<<"跳活3";
            return JUMPLIVE3;
        }
        else if (colorright == -1 && colorright1 == mycolor &&
            colorright2 == mycolor && colorright3 == -1 && colorleft == -1)
        {
            qDebug()<<"跳活3";
            return JUMPLIVE3;
        }
        else if (colorleft == -1 && colorleft1 == mycolor &&
            colorleft2 == mycolor && colorleft3 == hiscolor && colorright == -1)
        {
            qDebug()<<"死3";
            return DIE3;
        }
        else if (colorright == -1 && colorright1 == mycolor &&
            colorright2 == mycolor && colorright3 == hiscolor && colorleft == -1)
        {
            qDebug()<<"死3";
            return DIE3;
        }
        else if (colorleft == -1 && colorleft1 == -1 &&
            colorleft2 == mycolor && colorleft3 == mycolor)
        {
            qDebug()<<"死3";
            return DIE3;
        }
        else if (colorright == -1 && colorright1 == -1 &&
            colorright2 == mycolor && colorright3 == mycolor)
        {
            qDebug()<<"死3";
            return DIE3;
        }
        else if (colorleft == -1 && colorleft1 == mycolor &&
            colorleft2 == -1 && colorleft3 == mycolor)
        {
            qDebug()<<"死3";
            return DIE3;
        }
        else if (colorright == -1 && colorright1 == mycolor &&
            colorright2 == -1 && colorright3 == mycolor)
        {
            qDebug()<<"死3";
            return DIE3;
        }
        else if (colorleft == -1 && colorleft1 == mycolor &&
            colorleft2 == -1 && colorleft3 == -1 && colorright == -1)
        {
            //qDebug()<<"低级活2";
            return LOWALIVE2;
        }
        else if (colorright == -1 && colorright1 == mycolor &&
            colorright2 == -1 && colorright3 == -1 && colorleft == -1)
        {
            //qDebug()<<"低级活2";
            return LOWALIVE2;
        }

        else if (colorleft == -1 && colorleft1 == -1 &&
            colorleft2 == mycolor && colorleft3 == -1 && colorright == -1)
        {
            //qDebug()<<"低级活2";
            return LOWALIVE2;
        }
        else if (colorright == -1 && colorright1 == -1 &&
            colorright2 == mycolor && colorright3 == -1 && colorleft == -1)
        {
            //qDebug()<<"低级活2";
            return LOWALIVE2;
        }else if((colorright == -1 && colorright1 == -1 &&
                 colorright2 == -1 && colorright3 == -1 && colorleft == -1) || (colorleft == -1 && colorleft2 == -1 &&
                                       colorleft3 == -1 && colorleft1 == -1 &&
                                       colorright == -1 ))
        {
            //qDebug()<<"活1";
             return ALIVE1;
        }
        //qDebug()<<"死1";
        return DIE1;
    }
    //qDebug()<<"NOTHING";
    return NOTHING;
}

// 横向
long CalcuScoreBetterCross(const QPoint &point, int color,
                           const QList<DrawData>& blackArray,
                           const QList<DrawData>& whiteArray)
{

    long score = 0;
    int iStartX = point.x();
    int iStartY = point.y();
    int Count = 0; // 找了多少次
    // -1表示空,0表示黑子,1表示白子,3表示没有坐标了,以所下点为中心横向或纵向或斜向
    // 各取四个点,包括要下的点一共9个点.用来判断棋形.初始化每个点不可下
    int ChessColor[9] = {3,3,3,3,3,3,3,3,3};
    // 向左找四个点.
    int hiscolor = color; // 记录连续的颜色.
    int contiCount = 1; // 连续点的个数,至少下的这一点是个连续点
    int colorleft = 3; // 不连续后左端点,从4-1开始
    int colorright = 5; // 不连续后右端点,从4+1开始
    bool bIsContinue = true; // 是否连续的标志
    int ConLeftColor = 3; // 左边第一个不连续的坐标
    int ConRightColor = 5; // 右边第一个不连续的坐标
    bool bIsFlag = true; // 是否更新中断位置的标识,只在第一次中断时更新
    // 从要下坐标向左找4个点的信息
    for(iStartX -= Length/14; iStartX>=120 && Count<4; iStartX -=Length/14)
    {
            QPoint point(iStartX, iStartY);
            DrawData date;
            date.point = point;
            if(blackArray.indexOf(date) != -1) // 这个坐标如果为黑色
            {
                if(hiscolor == 0 && bIsContinue) // 同色
                {
                    contiCount++;
                }else // 不同色则连续中断
                {
                    if(bIsFlag)
                    {
                        ConLeftColor = colorleft;
                        bIsFlag = false;
                    }
                    bIsContinue = false;
                }
                Count++;
                ChessColor[colorleft] = 0;
                colorleft--;
            }else if(whiteArray.indexOf(date) != -1) // 这个坐标为白色
            {
                if(hiscolor == 1 && bIsContinue) // 同色
                {
                    contiCount++;
                }else // 不同色则连续中断
                {
                    if(bIsFlag)
                    {
                        ConLeftColor = colorleft;
                        bIsFlag = false;
                    }
                    bIsContinue = false;
                }
                Count++;
                ChessColor[colorleft] = 1;
                colorleft--;
            }else // 白色黑色均不是,为空
            {
                if(bIsFlag)
                {
                    ConLeftColor = colorleft;
                    bIsFlag = false;
                }
                bIsContinue = false;
                Count++;
                ChessColor[colorleft] = -1;
                colorleft--;
            }
    }
    // 向右找四个点
    iStartX = point.x();
    iStartY = point.y();
    Count = 0;
    bIsContinue = true;
    bIsFlag = true;
    for(iStartX += Length/14; iStartX<=120+Length && Count<4; iStartX +=Length/14)
    {
        QPoint point(iStartX, iStartY);
        DrawData date;
        date.point = point;
        if(blackArray.indexOf(date) != -1) // 这个坐标如果为黑色
        {
            if(hiscolor == 0 && bIsContinue) // 同色
            {
                contiCount++;
            }else // 不同色则连续中断
            {
                if(bIsFlag)
                {
                    ConRightColor = colorright;
                    bIsFlag = false;
                }
                bIsContinue = false;
            }
            Count++;
            ChessColor[colorright] = 0;
            colorright++;
        }else if(whiteArray.indexOf(date) != -1) // 这个坐标为白色
        {
            if(hiscolor == 1 && bIsContinue) // 同色
            {
                contiCount++;
            }else // 不同色则连续中断
            {
                if(bIsFlag)
                {
                    ConRightColor = colorright;
                    bIsFlag = false;
                }
                bIsContinue = false;
            }
            Count++;
            ChessColor[colorright] = 1;
            colorright++;
        }else // 为空
        {
            if(bIsFlag)
            {
                ConRightColor = colorright;
                bIsFlag = false;
            }
            bIsContinue = false;
            Count++;
            ChessColor[colorright] = -1;
            colorright++;
        }
    }
    return AnalysisOfChessShape(ChessColor, contiCount,
                                ConLeftColor, ConRightColor,
                                color);
}

long CalcuScoreBetterColumn(const QPoint &point, int color,
                            const QList<DrawData>& blackArray,
                            const QList<DrawData>& whiteArray)
{
    // 取出以当前点为中心的一条线上的左右,上下,斜1和斜2四个直线两边各四个点信息,并判断棋型
    long score = 0;
    int iStartX = point.x();
    int iStartY = point.y();
    int Count = 0; // 找了多少次
    // -1表示空,0表示黑子,1表示白子,3表示没有坐标了,以所下点为中心横向或纵向或斜向
    // 各取四个点,包括要下的点一共9个点.用来判断棋形.
    int ChessColor[9] = {3,3,3,3,3,3,3,3,3};
    // 向左找四个点.
    int hiscolor = color; // 记录连续的颜色.
    int contiCount = 1; // 连续点的个数,至少下的这一点是个连续点
    int colorleft = 3; // 不连续后左端点,从4-1开始
    int colorright = 5; // 不连续后右端点,从4+1开始
    bool bIsContinue = true; // 是否连续
    int ConLeftColor = 3; // 左边第一个不连续的坐标
    int ConRightColor = 5; // 右边第一个不连续的坐标
    bool bIsFlag = true;
    // 从要下坐标向上找4个点的信息
    for(iStartY -= Length/14; iStartY>=120 && Count<4; iStartY -=Length/14)
    {
            QPoint point(iStartX, iStartY);
            DrawData date;
            date.point = point;
            if(blackArray.indexOf(date) != -1) // 这个坐标如果为黑色
            {
                if(hiscolor == 0 && bIsContinue) // 同色
                {
                    contiCount++;
                }else // 不同色则连续中断
                {
                    if(bIsFlag)
                    {
                        ConLeftColor = colorleft;
                        bIsFlag = false;
                    }
                    bIsContinue = false;
                }
                Count++;
                ChessColor[colorleft] = 0;
                colorleft--;
            }else if(whiteArray.indexOf(date) != -1) // 这个坐标为白色
            {
                if(hiscolor == 1 && bIsContinue) // 同色
                {
                    contiCount++;
                }else // 不同色则连续中断
                {
                    if(bIsFlag)
                    {
                        ConLeftColor = colorleft;
                        bIsFlag = false;
                    }
                    bIsContinue = false;
                }
                Count++;
                ChessColor[colorleft] = 1;
                colorleft--;
            }else // 为空
            {
                if(bIsFlag)
                {
                    ConLeftColor = colorleft;
                    bIsFlag = false;
                }
                bIsContinue = false;
                Count++;
                ChessColor[colorleft] = -1;
                colorleft--;
            }
    }
    // 向右找四个点
    iStartX = point.x();
    iStartY = point.y();
    Count = 0;
    bIsContinue = true;
    bIsFlag = true;
    for(iStartY += Length/14; iStartY<=120+Length && Count<4; iStartY +=Length/14)
    {
        QPoint point(iStartX, iStartY);
        DrawData date;
        date.point = point;
        if(blackArray.indexOf(date) != -1) // 这个坐标如果为黑色
        {
            if(hiscolor == 0 && bIsContinue) // 同色
            {
                contiCount++;
            }else // 不同色则连续中断
            {
                if(bIsFlag)
                {
                    ConRightColor = colorright;
                    bIsFlag = false;
                }
                bIsContinue = false;
            }
            Count++;
            ChessColor[colorright] = 0;
            colorright++;
        }else if(whiteArray.indexOf(date) != -1) // 这个坐标为白色
        {
            if(hiscolor == 1 && bIsContinue) // 同色
            {
                contiCount++;
            }else // 不同色则连续中断
            {
                if(bIsFlag)
                {
                    ConRightColor = colorright;
                    bIsFlag = false;
                }
                bIsContinue = false;
            }
            Count++;
            ChessColor[colorright] = 1;
            colorright++;
        }else // 为空
        {
            if(bIsFlag)
            {
                ConRightColor = colorright;
                bIsFlag = false;
            }
            bIsContinue = false;
            Count++;
            ChessColor[colorright] = -1;
            colorright++;
        }
    }
    return AnalysisOfChessShape(ChessColor, contiCount, ConLeftColor,
                                ConRightColor, color);
}

long CalcuScoreBetterRightSlant(const QPoint &point, int color,
                                const QList<DrawData>& blackArray,
                                const QList<DrawData>& whiteArray)
{
    // 取出以当前点为中心的一条线上的左右,上下,斜1和斜2四个直线两边各四个点信息,并判断棋型
    long score = 0;
    int iStartX = point.x();
    int iStartY = point.y();
    int Count = 0; // 找了多少次
    // -1表示空,0表示黑子,1表示白子,3表示没有坐标了,以所下点为中心横向或纵向或斜向
    // 各取四个点,包括要下的点一共9个点.用来判断棋形.
    int ChessColor[9] = {3,3,3,3,3,3,3,3,3};
    // 向左找四个点.
    int hiscolor = color; // 记录连续的颜色.
    int contiCount = 1; // 连续点的个数,至少下的这一点是个连续点
    int colorleft = 3; // 不连续后左端点,从4-1开始
    int colorright = 5; // 不连续后右端点,从4+1开始
    bool bIsContinue = true; // 是否连续
    int ConLeftColor = 3; // 左边第一个不连续的坐标
    int ConRightColor = 5; // 右边第一个不连续的坐标
    bool bIsFlag = true;
    // 从要下坐标向上找4个点的信息
    iStartX -= Length/14;
    iStartY += Length/14;
    while(iStartX >= 120 && iStartY <= 120+Length && Count<4)
    {
            QPoint point(iStartX, iStartY);
            DrawData date;
            date.point = point;
            if(blackArray.indexOf(date) != -1) // 这个坐标如果为黑色
            {
                if(hiscolor == 0 && bIsContinue) // 同色
                {
                    contiCount++;
                }else // 不同色则连续中断
                {
                    if(bIsFlag)
                    {
                        ConLeftColor = colorleft;
                        bIsFlag = false;
                    }
                    bIsContinue = false;
                }
                Count++;
                ChessColor[colorleft] = 0;
                colorleft--;
            }else if(whiteArray.indexOf(date) != -1) // 这个坐标为白色
            {
                if(hiscolor == 1 && bIsContinue) // 同色
                {
                    contiCount++;
                }else // 不同色则连续中断
                {
                    if(bIsFlag)
                    {
                        ConLeftColor = colorleft;
                        bIsFlag = false;
                    }
                    bIsContinue = false;
                }
                Count++;
                ChessColor[colorleft] = 1;
                colorleft--;
            }else // 为空
            {
                if(bIsFlag)
                {
                    ConLeftColor = colorleft;
                    bIsFlag = false;
                }
                bIsContinue = false;
                Count++;
                ChessColor[colorleft] = -1;
                colorleft--;
            }
            iStartX -= Length/14;
            iStartY += Length/14;
    }
    // 向右找四个点
    iStartX = point.x();
    iStartY = point.y();
    Count = 0;
    bIsContinue = true;
    bIsFlag = true;
    iStartX += Length/14;
    iStartY -= Length/14;
    while(iStartY >= 120 && iStartX <= 120+Length && Count<4)
    {
        QPoint point(iStartX, iStartY);
        DrawData date;
        date.point = point;
        if(blackArray.indexOf(date) != -1) // 这个坐标如果为黑色
        {
            if(hiscolor == 0 && bIsContinue) // 同色
            {
                contiCount++;
            }else // 不同色则连续中断
            {
                if(bIsFlag)
                {
                    ConRightColor = colorright;
                    bIsFlag = false;
                }
                bIsContinue = false;
            }
            Count++;
            ChessColor[colorright] = 0;
            colorright++;
        }else if(whiteArray.indexOf(date) != -1) // 这个坐标为白色
        {
            if(hiscolor == 1 && bIsContinue) // 同色
            {
                contiCount++;
            }else // 不同色则连续中断
            {
                if(bIsFlag)
                {
                    ConRightColor = colorright;
                    bIsFlag = false;
                }
                bIsContinue = false;
            }
            Count++;
            ChessColor[colorright] = 1;
            colorright++;
        }else // 为空
        {
            if(bIsFlag)
            {
                ConRightColor = colorright;
                bIsFlag = false;
            }
            bIsContinue = false;
            Count++;
            ChessColor[colorright] = -1;
            colorright++;
        }
        iStartX += Length/14;
        iStartY -= Length/14;
    }
    return AnalysisOfChessShape(ChessColor, contiCount, ConLeftColor,
                                ConRightColor, color);
}

long CalcuScoreBetterLeftSlant(const QPoint &point, int color,
                               const QList<DrawData>& blackArray,
                               const QList<DrawData>& whiteArray)
{
    // 取出以当前点为中心的一条线上的左右,上下,斜1和斜2四个直线两边各四个点信息,并判断棋型
    long score = 0;
    int iStartX = point.x();
    int iStartY = point.y();
    int Count = 0; // 找了多少次
    // -1表示空,0表示黑子,1表示白子,3表示没有坐标了,以所下点为中心横向或纵向或斜向
    // 各取四个点,包括要下的点一共9个点.用来判断棋形.
    int ChessColor[9] = {3,3,3,3,3,3,3,3,3};
    // 向左找四个点.
    int hiscolor = color; // 记录连续的颜色.
    int contiCount = 1; // 连续点的个数,至少下的这一点是个连续点
    int colorleft = 3; // 不连续后左端点,从4-1开始
    int colorright = 5; // 不连续后右端点,从4+1开始
    bool bIsContinue = true; // 是否连续
    int ConLeftColor = 3; // 左边第一个不连续的坐标
    int ConRightColor = 5; // 右边第一个不连续的坐标
    bool bIsFlag = true;
    // 从要下坐标向上找4个点的信息
    iStartX += Length/14;
    iStartY += Length/14;
    while(iStartX <= (120 + Length) && iStartY <= (120+Length) && Count<4)
    {
            QPoint point(iStartX, iStartY);
            DrawData date;
            date.point = point;
            if(blackArray.indexOf(date) != -1) // 这个坐标如果为黑色
            {
                if(hiscolor == 0 && bIsContinue) // 同色
                {
                    contiCount++;
                }else // 不同色则连续中断
                {
                    if(bIsFlag)
                    {
                        ConLeftColor = colorleft;
                        bIsFlag = false;
                    }
                    bIsContinue = false;
                }
                Count++;
                ChessColor[colorleft] = 0;
                colorleft--;
            }else if(whiteArray.indexOf(date) != -1) // 这个坐标为白色
            {
                if(hiscolor == 1 && bIsContinue) // 同色
                {
                    contiCount++;
                }else // 不同色则连续中断
                {
                    if(bIsFlag)
                    {
                        ConLeftColor = colorleft;
                        bIsFlag = false;
                    }
                    bIsContinue = false;
                }
                Count++;
                ChessColor[colorleft] = 1;
                colorleft--;
            }else // 为空
            {
                if(bIsFlag)
                {
                    ConLeftColor = colorleft;
                    bIsFlag = false;
                }
                bIsContinue = false;
                Count++;
                ChessColor[colorleft] = -1;
                colorleft--;
            }
            iStartX += Length/14;
            iStartY += Length/14;
    }
    // 向右找四个点
    iStartX = point.x();
    iStartY = point.y();
    Count = 0;
    bIsContinue = true;
    bIsFlag = true;
    iStartX -= Length/14;
    iStartY -= Length/14;
    while(iStartY >= 120 && iStartX >= 120 && Count<4)
    {
        QPoint point(iStartX, iStartY);
        DrawData date;
        date.point = point;
        if(blackArray.indexOf(date) != -1) // 这个坐标如果为黑色
        {
            if(hiscolor == 0 && bIsContinue) // 同色
            {
                contiCount++;
            }else // 不同色则连续中断
            {
                if(bIsFlag)
                {
                    ConRightColor = colorright;
                    bIsFlag = false;
                }
                bIsContinue = false;
            }
            Count++;
            ChessColor[colorright] = 0;
            colorright++;
        }else if(whiteArray.indexOf(date) != -1) // 这个坐标为白色
        {
            if(hiscolor == 1 && bIsContinue) // 同色
            {
                contiCount++;
            }else // 不同色则连续中断
            {
                if(bIsFlag)
                {
                    ConRightColor = colorright;
                    bIsFlag = false;
                }
                bIsContinue = false;
            }
            Count++;
            ChessColor[colorright] = 1;
            colorright++;
        }else // 为空
        {
            if(bIsFlag)
            {
                ConRightColor = colorright;
                bIsFlag = false;
            }
            bIsContinue = false;
            Count++;
            ChessColor[colorright] = -1;
            colorright++;
        }
        iStartX -= Length/14;
        iStartY -= Length/14;
    }

    return AnalysisOfChessShape(ChessColor, contiCount, ConLeftColor,
                                ConRightColor, color);
}

// 返回一个点的分值.
long EvaluateWithPoint(const QPoint &point, int color,
                       const QList<DrawData>& blackArray,
                       const QList<DrawData>& whiteArray)
{
    long totalScore = 0;
    //qDebug()<<point<<"------得分:-----";
    long scoreCross = CalcuScoreBetterCross(point, color, blackArray, whiteArray);
   // qDebug()<<"横向得分:"<<scoreCross;
    long scoreColum = CalcuScoreBetterColumn(point, color, blackArray, whiteArray);
    //qDebug()<<"纵向得分:"<<scoreColum;
    long scoreLeft = CalcuScoreBetterLeftSlant(point, color, blackArray, whiteArray);
    //qDebug()<<"斜左得分:"<<scoreLeft;
    long scoreRight = CalcuScoreBetterRightSlant(point, color, blackArray, whiteArray);
    //qDebug()<<"斜右得分:"<<scoreRight;
    if(scoreCross == WIN5 || scoreColum == WIN5 ||
            scoreLeft == WIN5 || scoreRight == WIN5)
    {
        return WIN5*10;
    }
    if(scoreCross == ALIVE4 || scoreColum == ALIVE4 ||
            scoreLeft == ALIVE4 || scoreRight == ALIVE4)
    {
        return ALIVE4;
    }
    int nCountDie4 = 0; // 死4的数目
    int nCountAlive3 = 0; // 活3的数目
    int nCountAlive2 = 0; // 活2的数目
    if(scoreCross == DIE4)
    {
        nCountDie4++;
    }else if(scoreCross == ALIVE3)
    {
        nCountAlive3++;
    }else if(scoreCross == ALIVE2)
    {
        nCountAlive2++;
    }else
    {

    }

    if(scoreColum == DIE4)
    {
        nCountDie4++;
    }else if(scoreColum == ALIVE3)
    {
        nCountAlive3++;
    }else if(scoreColum == ALIVE2)
    {
        nCountAlive2++;
    }else
    {

    }

    if(scoreLeft == DIE4)
    {
        nCountDie4++;
    }else if(scoreLeft == ALIVE3)
    {
        nCountAlive3++;
    }else if(scoreLeft == ALIVE2)
    {
        nCountAlive2++;
    }else
    {

    }

    if(scoreRight == DIE4)
    {
        nCountDie4++;
    }else if(scoreRight == ALIVE3)
    {
        nCountAlive3++;
    }else if(scoreRight == ALIVE2)
    {
        nCountAlive2++;
    }else
    {

    }

    // 都是必胜棋型,必堵
    if(nCountDie4 >= 2 || (nCountDie4 >=1 && nCountAlive3 >= 1) || (nCountAlive3 >= 2))
    {
        qDebug()<<"Two-----------Two";
        return ALIVE4*1.5;
    }

    totalScore = scoreCross + scoreColum + scoreLeft + scoreRight;
    if(nCountAlive2 >= 2) // 双活2提高分值
    {
        totalScore += 300;
    }
    qDebug()<<"totalScore"<<totalScore;
    return totalScore;
}

// 根据启发函数获取应该下的点的坐标
int GetPutPoint(const QList<QPoint> &canPlayArray, int color,
                const QList<DrawData>& blackArray,
                const QList<DrawData>& whiteArray)
{
    long MaxOwnScore = 0;
    long MaxSameWhiteScore = 0;
    long MaxSameBlackScore = 0;
    long MaxOtherScore = 0;
    int indexOwn = 0;
    int indexOther = 0;
    for(int i = 0; i<canPlayArray.size(); i++)
    {
        QPoint point = canPlayArray.at(i);
        // 返回己方分值
        long scoreOwn = EvaluateWithPoint(point, color, blackArray, whiteArray);
        // 返回他方在这点下的分值
        long scoreOther = EvaluateWithPoint(point, (color+1)%2, blackArray,
                                            whiteArray);
        qDebug()<<"点坐标:"<<point<<"我方得分:"<<scoreOwn<<"对方得分:"<<scoreOther;
        if(scoreOwn > MaxOwnScore)
        {
            MaxOwnScore = scoreOwn;
            MaxSameWhiteScore = scoreOther; // 记录最大得分位置的对方得分
            indexOwn = i;
        }else if(scoreOwn == MaxOwnScore) // 如果这个点分值和已经得出的最大值一样,看对方的分值
        {
            if(MaxSameWhiteScore < scoreOther)
            {
                MaxSameWhiteScore = scoreOther;
                indexOwn = i;
            }
        }else
        {
            // 不做处理
        }

        if(MaxOtherScore < scoreOther)
        {
            MaxOtherScore = scoreOther;
            MaxSameBlackScore = scoreOwn;
            indexOther = i;
        }else if(MaxOtherScore == scoreOther)
        {
            if(MaxSameBlackScore < scoreOwn)
            {
                indexOther = i;
                MaxSameBlackScore = scoreOwn;
            }
        }

    }

    if((MaxOwnScore+10) >= MaxOtherScore)
    {
        qDebug()<<"应下坐标:"<<canPlayArray.at(indexOwn)<<"得分:"<<MaxOwnScore;
        return indexOwn;
    }
            qDebug()<<"应下坐标:"<<canPlayArray.at(indexOther)<<"对方得分:"<<MaxOtherScore;
    return indexOther;
}

/* 极大极小值alpha-beta剪枝算法 */
/*int MaxMin_AlphaBeta(int player, int depth, QList<Point>& canPlayArray,
                   int alpha, int beta)
{
    if(depth <= 0) // 到达终端局面
    {

    }
    if(player == 0) // AI走
    {
        // 每种走法
        for(int i = 0; i<canPlayArray.size(); i++)
        {
            QPoint point;

            int val = MaxMin_AlphaBeta((player+1)%2, depth-1, canPlayArray, alpha, beta);

        }
    }
}*/
