#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>
#include <QPixmap>
#include "message.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    // 处理窗口重绘事件
    void paintEvent(QPaintEvent *event);
    // 处理鼠标点击事件
    void mousePressEvent(QMouseEvent *event);
    // 游戏初始化
    void InitGame();
    // 实现悔棋功能
    void RetractChess();
    /*
    判断是否获胜的函数,因为获胜时必须是当前下过一步,所以从当前点开始寻找横行,竖行,斜行是否有
    连续的五个同类颜色的棋子.
    参数:QPoint& point, 当前所下点的坐标
    color:当前所下棋子的颜色
    返回值:true:获胜 false:没有获胜.
    */
    bool isWin(const QPoint& point, int color);
    // 判断同一横行或者同一列是否五连,
    // flag = 0 横行  flag = 1 列
    bool JudgeCrossOrColumnIsWin(const QPoint& point, int color, int flag);
    // 判断左斜线是否五连
    bool JudgeLeftSlantIsWin(const QPoint& point, int color);
    // 判断右斜线是否五连
    bool JudgeRightSlantIsWin(const QPoint& point, int color);
    /*
    电脑下子
    */

    /*极大极小值算法*/
    /* 局势评价函数, 相对于传入的player的评价值*/
    long Evaluate(int player = 0);
   // /*极大极小值算法的Alpha-Beta搜索*/
    long MaxMin_AlphaBeta(int depth,  int alpha, int beta);
    // 优化查找,如果一点附件两个点无点位,不搜索
    void GeneratorCanPlay(QList<QPoint>& canPlayArray);
    /*极大值*/
    long Max(int depth, int alpha, int beta);
    long Min(int depth, int alpha, int beta);
    //void GeneratorCanPlay();
    void AIPlay();
private slots:
    void on_action_New_triggered();
    void on_action_Exit_triggered();
    void on_action_Retract_triggered();
    void on_action_FuPan_triggered();
    void on_action_First_triggered(bool checked);

    void on_action_Last_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    // 棋盘的边长,是正方形
    int Length;
    // 标记是否有获胜的变量
    static int Win;
    // 标记当前下子的一方,true为AI,false为人
    bool bWhoPlay;
    bool isInit; // 是否初始化了新游戏
    // 所有下过点的坐标集合,用于每次绘制时使用
    QList<DrawData> m_pointArray;
    // 存储所有未下过点的坐标,由AI在其中寻找可下点
    QList<QPoint> m_pointCanPlayArray;
    // 提供复盘功能的List
    QList<DrawData> m_pointTemp;
    // 黑子下过点的坐标集合
    QList<DrawData> m_BlackArray;
    // 白子下过点的坐标集合
    QList<DrawData> m_WhiteArray;
private:
    QPixmap image1;
    bool bIsPlayMus; // 是否播放音效
    int m_MinMaxDepth; // 搜索深度
};

#endif // MAINWINDOW_H
