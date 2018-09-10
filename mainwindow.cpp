#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QHash>
#include <QMediaPlayer>
#include "evaluate.h"


#include "score.h" // 提供分值定义

QPoint g_curPoint;
int MainWindow::Win = 0;
int g_Count = 0;
long MaxMinCount = 0;
// 先走的优势分
const int ADVANCED_VALUE = 3;
// 游戏初始化
void MainWindow::InitGame()
{
    // 清空所有下过点的坐标
    m_pointArray.clear();
    // 初始化可以下棋点的坐标
    m_pointCanPlayArray.clear();
    for(int row = 120; row<=120+Length; row+=Length/14)
    {
        for(int col = 120; col<=120+Length; col+=Length/14)
        {
            QPoint point(row, col);
            m_pointCanPlayArray.append(point);
        }
    }
    qDebug()<<"size:"<<m_pointCanPlayArray.size();
    // 初始化人先下
    bWhoPlay = false;
    // 清空黑子下过点的坐标
    m_BlackArray.clear();
    m_WhiteArray.clear();
    // 初始化无获胜
    Win = 0;
    isInit = true;
    g_Count = 0;
}

void MainWindow::RetractChess()
{
    if(Win != 0)
    {
        Win = 0;
    }
    QPoint pointLast1(0,0);
    QPoint pointLast2(0,0);
    // 黑白棋各去掉最后一个点
    if(!m_BlackArray.empty())
    {
        pointLast1 = m_BlackArray.at(m_BlackArray.size() - 1).point;
        m_BlackArray.removeLast();
    }
    if(!m_WhiteArray.empty())
    {
        pointLast2 = m_WhiteArray.at(m_WhiteArray.size() - 1).point;
        m_WhiteArray.removeLast();
    }
    // 删除已下点坐标后两个
    if(!m_pointArray.empty())
    {
        m_pointArray.removeLast();
    }
    if(!m_pointArray.empty())
    {
        m_pointArray.removeLast();
    }
    if(pointLast1 != QPoint(0,0))
    {
        m_pointCanPlayArray.append(pointLast1);
    }
    if(pointLast2 != QPoint(0,0))
    {
        m_pointCanPlayArray.append(pointLast2);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Length = 420;
    // 设置窗口固定大小
    setFixedSize(1000,600);
    image1.load(":image/images/bk2.jfif");
    m_MinMaxDepth = 2;
    InitGame();
}

// 窗口重绘
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(700,80, 300, 680, image1);
    int row = Length/14;
    int col = Length/14;
    QPen pen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawRect(QRect(120,120,Length,Length));
    QPen pen1(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen1);
    // 绘制棋盘
    for(int i = 0; i<14; i++)
    {
        painter.drawLine(QPoint(120,120+row), QPoint(120+Length,120+row));
        row += Length/14;
    }
    for(int j = 0; j<14; j++)
    {
        painter.drawLine(QPoint(120+col, 120), QPoint(120+col, 120+Length));
        col += Length/14;
    }
    // 绘制当前棋局,从存储所有下过点的坐标集合中取出点进行绘制.
    for(int count = 0; count<m_pointArray.size(); count++)
    {
        if(m_pointArray.at(count).color == 0)
        {
            QBrush brush(Qt::black, Qt::SolidPattern);
            painter.setBrush(brush);
            painter.drawEllipse(m_pointArray.at(count).point, 13,13);
        }else
        {
            QBrush brush(Qt::white, Qt::SolidPattern);
            painter.setBrush(brush);
            painter.drawEllipse(m_pointArray.at(count).point, 13,13);
        }
        if(Win != 0) // 如果Win不等于0,说明有获胜,绘制结果.
        {
            QFont font;
            font.setPointSize(30);
            font.setFamily("Microsoft YaHei");
            font.setLetterSpacing(QFont::AbsoluteSpacing,0);
            painter.setPen(Qt::yellow);
            painter.setFont(font);
            if(1 == Win)
            {
                painter.drawText(300,300, "黑方胜!");
            }else
            {
                painter.drawText(300,300, "白方胜!");
            }
        }
    }
    if(bWhoPlay && Win == 0) // 电脑在无获胜情况下下子
    {
        AIPlay();
        bWhoPlay = false;
        for(int count = 0; count<m_pointArray.size(); count++)
        {
             if(m_pointArray.at(count).color == 0)
             {
                  QBrush brush(Qt::black, Qt::SolidPattern);
                  painter.setBrush(brush);
                  painter.drawEllipse(m_pointArray.at(count).point, 13,13);
             }else
             {
                  QBrush brush(Qt::white, Qt::SolidPattern);
                  painter.setBrush(brush);
                  painter.drawEllipse(m_pointArray.at(count).point, 13,13);
             }
             if(Win != 0) // 如果Win不等于0,说明有获胜,绘制结果.
             {
                    QFont font;
                    font.setPointSize(30);
                    font.setFamily("Microsoft YaHei");
                    font.setLetterSpacing(QFont::AbsoluteSpacing,0);
                    painter.setPen(Qt::yellow);
                    painter.setFont(font);
                    if(1 == Win)
                    {
                        painter.drawText(300,300, "黑方胜!");
                    }else
                    {
                        painter.drawText(300,300, "白方胜!");
                    }
                }
            }

        }
}

void MainWindow::AIPlay()
{

    if(g_Count < 3)
    {
        int index = GetPutPoint(m_pointCanPlayArray, 0, m_BlackArray, m_WhiteArray);
        DrawData date;
        date.point = m_pointCanPlayArray.at(index);
        date.color = 0;
        m_BlackArray.append(date);
        if(isWin(date.point, 0))
        {
            Win = 1;
        }
        m_pointCanPlayArray.removeAt(index);
        m_pointArray.append(date);
        g_Count++;
    }else
    {
        MaxMin_AlphaBeta(m_MinMaxDepth, -99999999, 99999999);
        qDebug()<<"本次计算"<<MaxMinCount<<"次!";
        MaxMinCount = 0;
        DrawData date;
        date.point = g_curPoint;
        date.color = 0;
        m_BlackArray.append(date);
        if(isWin(date.point, 0))
        {
            Win = 1;
        }
        qDebug()<<"移除坐标:"<<g_curPoint;
        m_pointCanPlayArray.removeAll(g_curPoint);
        m_pointArray.append(date);
    }

}



// 判断横行五连或者竖向五连
bool MainWindow::JudgeCrossOrColumnIsWin(const QPoint& point, int color, int flag)
{
    int x = point.x();
    int y = point.y();
    int nStartX = x;
    int nStartY = y;
    int start = 0;
    if(0 == flag) // 横行
    {
        start = nStartX;
    }else if(1 == flag) // 列行
    {
        start = nStartY;
    }else
    {
        return false;
    }
    int Count = 1;
    bool bFind = false;

    for(start = (start-Length/14); start>=120; start-=Length/14)
    {
        if(0 == flag)
        {
            nStartX = start;
        }else
        {
            nStartY = start;
        }
        if(0 == color) // 如果当前下的是黑子,则在黑子中寻找
        {
            for(int i = 0; i<m_BlackArray.size(); i++)
            {
                if(m_BlackArray.at(i).point == QPoint(nStartX, nStartY))
                {
                    Count++;
                    bFind = true;
                    break;
                }
            }
            if(bFind)// 找到,Count+1后继续寻找
            {
                bFind = false;
                continue;
            }else // 没有找到,退出左行循环,并开始右行继续寻找
            {
                break;
            }
        }else // 在白色棋子中寻找
        {
            for(int i = 0; i<m_WhiteArray.size(); i++)
            {
                if(m_WhiteArray.at(i).point == QPoint(nStartX, nStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }
    }

    if(Count >= 5) // 如果找到了5个连续的,表示获胜了
    {
        return true;
    }
    nStartX = x;
    nStartY = y;
    start = 0;
    if(0 == flag) // 横行
    {
        start = nStartX;
    }else if(1 == flag) // 列行
    {
        start = nStartY;
    }else
    {
        return false;
    }
    bFind = false;
    for(start = (start+Length/14); start<=120+Length; start+=Length/14)
    {
        if(0 == flag)
        {
            nStartX = start;
        }else
        {
            nStartY = start;
        }
        if(0 == color)
        {
            for(int i = 0; i<m_BlackArray.size(); i++)
            {
                if(m_BlackArray.at(i).point == QPoint(nStartX, nStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }else
        {
            for(int i = 0; i<m_WhiteArray.size(); i++)
            {
                if(m_WhiteArray.at(i).point == QPoint(nStartX, nStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }
    }
    if(Count >= 5) // 如果找到了5个连续的,表示获胜了
    {
        return true;
    }
    qDebug()<<"Count:"<<Count;
    return false;
}

// 判断左斜列是否五连
bool MainWindow::JudgeLeftSlantIsWin(const QPoint &point, int color)
{
    int Count = 1;
    int iStartX = point.x();
    int iStartY = point.y();
    bool bFind = false;
    while(iStartY <= 120+Length && iStartX <= 120+Length)
    {
        iStartX += Length/14;
        iStartY += Length/14;
        if(0 == color)
        {
            for(int i = 0; i<m_BlackArray.size(); i++)
            {
                if(m_BlackArray.at(i).point == QPoint(iStartX, iStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }else
        {
            for(int i = 0; i<m_WhiteArray.size(); i++)
            {
                if(m_WhiteArray.at(i).point == QPoint(iStartX, iStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }
    }
    if(Count >= 5) // 如果找到了5个连续的,表示获胜了
    {
        return true;
    }

    iStartX = point.x();
    iStartY = point.y();
    bFind = false;
    while(iStartX >= 120 && iStartY >= 120)
    {
        iStartX -= Length/14;
        iStartY -= Length/14;
        if(0 == color)
        {
            for(int i = 0; i<m_BlackArray.size(); i++)
            {
                if(m_BlackArray.at(i).point == QPoint(iStartX, iStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }else
        {
            for(int i = 0; i<m_WhiteArray.size(); i++)
            {
                if(m_WhiteArray.at(i).point == QPoint(iStartX, iStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }
    }
    if(Count >= 5) // 如果找到了5个连续的,表示获胜了
    {
        return true;
    }
    return false;
}

// 判断右斜线是否五连
bool MainWindow::JudgeRightSlantIsWin(const QPoint &point, int color)
{
    int iStartX = point.x();
    int iStartY = point.y();
    int Count = 1;
    bool bFind = false;
    while(iStartX >= 120 && iStartY <= 120+Length)
    {
        iStartX -= Length/14;
        iStartY += Length/14;
        if(0 == color)
        {
            for(int i = 0; i<m_BlackArray.size(); i++)
            {
                if(m_BlackArray.at(i).point == QPoint(iStartX, iStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }else
        {
            for(int i = 0; i<m_WhiteArray.size(); i++)
            {
                if(m_WhiteArray.at(i).point == QPoint(iStartX, iStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }
    }
    if(Count >= 5) // 如果找到了5个连续的,表示获胜了
    {
        return true;
    }

    iStartX = point.x();
    iStartY = point.y();
    bFind = false;
    while(iStartY >= 120 && iStartX <= 120+Length)
    {
        iStartX += Length/14;
        iStartY -= Length/14;
        if(0 == color)
        {
            for(int i = 0; i<m_BlackArray.size(); i++)
            {
                if(m_BlackArray.at(i).point == QPoint(iStartX, iStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }else
        {
            for(int i = 0; i<m_WhiteArray.size(); i++)
            {
                if(m_WhiteArray.at(i).point == QPoint(iStartX, iStartY))
                {
                    Count++;
                    bFind = true;
                    continue;
                }
            }
            if(bFind)
            {
                bFind = false;
                continue;
            }else
            {
                break;
            }
        }
    }
    if(Count >= 5) // 如果找到了5个连续的,表示获胜了
    {
        return true;
    }
    return false;
}


// 判断是否获胜的函数
bool MainWindow::isWin(const QPoint& point, int color)
{
    if(JudgeCrossOrColumnIsWin(point, color, 0) || // 横行
       JudgeCrossOrColumnIsWin(point, color, 1) || // 竖行
       JudgeLeftSlantIsWin(point, color) ||        // 左斜行
       JudgeRightSlantIsWin(point, color))         // 右斜行
    {
        return true;
    }
    return false;
}

// 鼠标事件处理
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(Win != 0 || bWhoPlay) // 有一方获胜不再响应鼠标事件.在电脑下棋时不响应鼠标事件
        return;
    QPoint point = event->pos();
    qDebug()<<point;
    QRect rect(120, 120, Length, Length);
    if(rect.contains(point)) // 在棋盘上单击才处理
    {
        int x = point.x();
        int y = point.y();
        int value = Length/14;
        if(x % value != 0 || y % value != 0) // 找到绘制的交点
        {
            int modX = x%value;
            if(modX == 0)
            {
                x = x;
            }else if(modX < value/2)
            {
                x = x - x%value;
            }else
            {
                x = (x - x%value + value);
            }
            int modY = y%value;
            if(modY == 0)
            {
                y = y;
            }else if(modY < value/2)
            {
                y = y - y%value;
            }else
            {
                y = (y - y%value + value);
            }
        }
        DrawData date;
        date.point = QPoint(x, y);
        date.isUse = true;
        bool bHave = false;
        // 当前点如果已经下过子,则不再处理.
        for(int i = 0; i<m_pointArray.size(); i++)
        {
            if(m_pointArray.at(i).point == date.point)
            {
                bHave = true;
                break;
            }
        }
        if(!bHave) // 当前没有下过,则需要记录
        {
            date.color = 1;
            m_WhiteArray.append(date);
            m_pointArray.append(date);
            qDebug()<<"人方下的点:"<<date.point;
            m_pointCanPlayArray.removeAll(date.point);
            // 从可下点坐标移除
            qDebug()<<"移除坐标"<<date.point;
            // 每下一步,判断胜负
            if(isWin(date.point, 1) == true)
            {
                 bWhoPlay = false;// 电脑不再下
                 Win = 2;
            }else
            {
                // 标记为电脑下,这里一定要是人方下了之后才能标记.
                bWhoPlay = true;
            }
        }
    }else // 不在区域内,否则不做处理
    {

    }
    this->repaint(); // 重绘
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 新游戏菜单相应
void MainWindow::on_action_New_triggered()
{
    // 游戏初始化
    InitGame();
    this->repaint();
}

void MainWindow::on_action_Exit_triggered()
{
    close();
}

// 实现悔棋功能
void MainWindow::on_action_Retract_triggered()
{
    RetractChess();
    this->repaint();
}

// 实现复盘功能
/*
    用静态变量存储所有下过的点,然后将绘制集合清空,每次添加一个点,绘制出来
    当绘制完成后,将所有静态变量置回初值.
*/
void MainWindow::on_action_FuPan_triggered()
{
    static QList<DrawData> m_pointTempS = m_pointArray;
    static int count = 0;
    static bool bClear = false;
    if(isInit)
    {
        m_pointTempS = m_pointArray;
        count = 0;
        bClear = false;
        isInit = false;
    }
    if(!bClear)
    {
        m_pointArray.clear();
        bClear = true;
    }
    if(Win != 0)
    {
        Win = 0;
    }

    if(count < m_pointTempS.size())
    {
        m_pointArray.append(m_pointTempS.at(count));
        this->repaint();
        count++;
    }else
    {
        m_pointTempS = m_pointArray;
        count = 0;
        bClear = false;
    }

}

/*极大极小值算法*/

// 评估函数
long MainWindow::Evaluate(int player)
{
    long whiteScore = 0;
    long blackScore = 0;
    long EvaScore = 0;
    {
        // 白方估分
        for(int i = 0; i<m_WhiteArray.size(); i++)
        {
            QPoint point = m_WhiteArray.at(i).point;
            int Score = CalcuScoreBetterCross(point, 1, m_BlackArray, m_WhiteArray);
            if(Score == ALIVE4)
            {
                whiteScore += 1000000; // 必堵棋形
            }else if(Score == DIE4 || Score == LOWDIE4||
                     Score == JUMPLIVE3 || Score == ALIVE3)
            {
                whiteScore += 1000;
            }else
            {
                 whiteScore += Score;
            }
            Score = CalcuScoreBetterRightSlant(point, 1, m_BlackArray, m_WhiteArray);
            if(Score == ALIVE4)
            {
                whiteScore += 1000000; // 必堵棋形
            }else if(Score == DIE4 || Score == LOWDIE4||
                     Score == JUMPLIVE3 || Score == ALIVE3)
            {
                whiteScore += 1000;
            }else
            {
                 whiteScore += Score;
            }
            Score = CalcuScoreBetterColumn(point, 1, m_BlackArray, m_WhiteArray);
            if(Score == ALIVE4)
            {
                whiteScore += 1000000; // 必堵棋形
            }else if(Score == DIE4 || Score == LOWDIE4||
                     Score == JUMPLIVE3 || Score == ALIVE3)
            {
               // whiteScore += 1000;
            }else
            {
                 whiteScore += Score;
            }
            Score = CalcuScoreBetterLeftSlant(point, 1, m_BlackArray, m_WhiteArray);
            if(Score == ALIVE4)
            {
                whiteScore += 1000000; // 必堵棋形
            }else if(Score == DIE4 || Score == LOWDIE4||
                     Score == JUMPLIVE3 || Score == ALIVE3)
            {
                whiteScore += 1000;
            }else
            {
                 whiteScore += Score;
            }
            //whiteScore += EvaluateWithPoint(point, 1, m_BlackArray, m_WhiteArray);
            //if(whiteScore >= ALIVE4 || whiteScore >= DIE4 || whiteScore >= ALIVE3 || whiteScore >= JUMPLIVE3)
            //    whiteScore = 100000;
            whiteScore += ADVANCED_VALUE; // 加上先走的优势分
        }
        qDebug()<<tr("白方局势分:")<<whiteScore;
        // 黑方估分
        for(int j = 0; j<m_BlackArray.size(); j++)
        {
            QPoint point = m_BlackArray.at(j).point;
            blackScore += EvaluateWithPoint(point, 0, m_BlackArray, m_WhiteArray);
        }
        qDebug()<<tr("黑方局势分:")<<blackScore;
        EvaScore = whiteScore - blackScore;
    }
    if(player == 0)
        return EvaScore;
    else
        return 0-EvaScore;
}

void MainWindow::GeneratorCanPlay(QList<QPoint>& canPlayArray)
{
    for(int i = 0; i<m_pointCanPlayArray.size(); i++)
    {
        QPoint point = m_pointCanPlayArray.at(i);
        bool bHave = false;
        int Count = 0;
        // 向左
        int x = point.x();
        int y = point.y();
        for(x = x-Length/14; (x>=120 && Count<2); x-=Length/14)
        {
            DrawData date;
            date.point = QPoint(x, y);
            date.color = 0;
            if(m_BlackArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave =  true;
                break;
            }
            date.color = 1;
            if(m_WhiteArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            Count++;
        }
        if(bHave)
            continue;
        // 向右
        Count = 0;
        x = point.x();
        y = point.y();
        for(x = x+Length/14; x<=120+Length && Count<2; x+=Length/14)
        {
            DrawData date;
            date.point = QPoint(x, y);
            date.color = 0;
            if(m_BlackArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            date.color = 1;
            if(m_WhiteArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            Count++;
        }
        if(bHave)
            continue;

        // 向上
        Count = 0;
        x = point.x();
        y = point.y();
        for(y = y-Length/14; y>=120 && Count<2; y-=Length/14)
        {
            DrawData date;
            date.point = QPoint(x, y);
            date.color = 0;
            if(m_BlackArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            date.color = 1;
            if(m_WhiteArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            Count++;
        }
        if(bHave)
            continue;

        // 向下
        Count = 0;
        x = point.x();
        y = point.y();
        for(y = y+Length/14; y<=120+Length && Count<2; y+=Length/14)
        {
            DrawData date;
            date.point = QPoint(x, y);
            date.color = 0;
            if(m_BlackArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            date.color = 1;
            if(m_WhiteArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            Count++;
        }
        if(bHave)
            continue;

        // 斜右下
        Count = 0;
        x = point.x();
        y = point.y();
        x += Length/14;
        y += Length/14;
        while( x <= 120+Length && y <= 120+Length && Count<2)
        {
            DrawData date;
            date.point = QPoint(x, y);
            date.color = 0;
            if(m_BlackArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            date.color = 1;
            if(m_WhiteArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            x += Length/14;
            y += Length/14;
            Count++;
        }
        if(bHave)
            continue;

        // 斜左上
        Count = 0;
        x = point.x();
        y = point.y();
        x -= Length/14;
        y -= Length/14;
        while( x >= 120 && y >=  120 && Count<2)
        {
            DrawData date;
            date.point = QPoint(x, y);
            date.color = 0;
            if(m_BlackArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            date.color = 1;
            if(m_WhiteArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            x -= Length/14;
            y -= Length/14;
            Count++;
        }
        if(bHave)
            continue;

        // 斜左下
        Count = 0;
        x = point.x();
        y = point.y();
        x -= Length/14;
        y += Length/14;
        while(x >= 120 && y <= 120+Length && Count<2)
        {

            DrawData date;
            date.point = QPoint(x, y);
            date.color = 0;
            if(m_BlackArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            date.color = 1;
            if(m_WhiteArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            x -= Length/14;
            y += Length/14;
            Count++;
        }
        if(bHave)
            continue;

        // 斜右上
        Count = 0;
        x = point.x();
        y = point.y();
        x += Length/14;
        y -= Length/14;
        while(x <= 120+Length && y >= 120 && Count<2)
        {
            DrawData date;
            date.point = QPoint(x, y);
            date.color = 0;
            if(m_BlackArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            date.color = 1;
            if(m_WhiteArray.indexOf(date) != -1)
            {
                canPlayArray.append(point);
                bHave = true;
                break;
            }
            x += Length/14;
            y -= Length/14;
            Count++;
        }
        if(bHave)
            continue;
    }
}
// 人方的极大值计算
long MainWindow::Min(int depth, int alpha, int beta)
{
    if(depth <= 0)
    {
        MaxMinCount++;
        return Evaluate(0);
    }
    // 所有可能下点的坐标
    QList<QPoint> canPlayArray;
    GeneratorCanPlay(canPlayArray);
    for(int i = 0; i<canPlayArray.size(); i++)
    {
        MaxMinCount++;
        DrawData date;
        date.point = canPlayArray.at(i);
        date.color = 0;
        qDebug()<<"---------黑可下点坐标:----------"<<date.point;
        m_BlackArray.append(date);
        qDebug()<<"黑子个数:"<<m_BlackArray.size();
        m_pointCanPlayArray.removeAll(date.point);
        // 极小值计算时,beta发生变化,alpha不发生变化.此时的alpha下一层算出的极大值.
        int val = Max(depth-1,  alpha, beta);
        qDebug()<<"黑方在此点下子后得分"<<val;
        m_pointCanPlayArray.append(date.point);
        m_BlackArray.removeAll(date);
        qDebug()<<"黑子个数"<<m_BlackArray.size();
        // 极小值搜索,beta变化,alpha不变
        if(val <= beta)
        {
            if(depth == m_MinMaxDepth)
            {
                qDebug()<<tr("Point")<<date.point<<tr("Score:")<<val<<"beta"<<beta;
                g_curPoint = date.point;
                beta = val;
            }
        }
        // 算出的极小值比上层算出的一个极大值要小,意味着极大层不会选择这条分支得到
        // 一个更小的值，所以直接返回.
        if(alpha > beta)
            return beta;
    }
    return beta;
}

// 极大值计算
long MainWindow::Max(int depth, int alpha, int beta)
{
    if(depth <= 0)
    {
        MaxMinCount++;
        return Evaluate(0); // 对于人方的估值
    }
    // 所有可能下点的坐标
    QList<QPoint> canPlayArray;
    GeneratorCanPlay(canPlayArray);
    for(int i = 0; i<canPlayArray.size(); i++)
    {
        MaxMinCount++;
        DrawData date;
        date.point = canPlayArray.at(i);
        date.color = 0;
        m_WhiteArray.append(date);
        // beta是子节点算出的极小值,而alpha是上层算出的极大值.
        m_pointCanPlayArray.removeAll(date.point);
        int val = Min(depth-1,  alpha, beta);
        m_pointCanPlayArray.append(date.point);
        m_WhiteArray.removeAll(date);
        // 极大层搜索,alpha值在变化,beta不变.
        if(val >= alpha)
        {
            //g_curPoint = date.point;
            qDebug()<<"白方下子点:"<<date.point<<"极大值:"<<val<<"alpha:"<<alpha;
            alpha = val;
        }
        // 在极大值层算出的极大值比上一层算出的极小值要大,上层极小层不会选择此分支,返回.
        if(alpha > beta)
            return alpha;
    }
    return alpha;
}

long MainWindow::MaxMin_AlphaBeta(int depth, int alpha, int beta)
{
        Min(depth, alpha, beta);
}

void MainWindow::on_action_First_triggered(bool checked)
{
    if(checked == true)
    {
        qDebug()<<"checked";
    }
}

void MainWindow::on_action_Last_triggered(bool checked)
{
    if(checked == true)
    {
    }
}
