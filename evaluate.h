/*
 * 提供对局势判断的函数
*/
#include <QPoint>
#include <QList>
#include "message.h"
#ifndef EVALUATE_H
#define EVALUATE_H

/*
 * 判断棋形的函数
ChessColor[]:以下子点为中心向左向右各延伸4个点的9位数组.
continueCout:在中心位置下子后出现的连珠数目
leftBreak:左边断开连珠的位置
rightBreak:右边断开连珠的位置
hiscolor:当前下子的颜色 0:黑色 1:白色
blackArray:当前所有的黑子坐标
whiteArray:当前所有的白子坐标
*/

long AnalysisOfChessShape(int ChessColor[], int continueCount,  int leftBreak,
                          int rightBreak, int hiscolor);
/* 四个方向上判断棋型得分的函数
point: 检测点的坐标
color:检测点的颜色 0:黑方 1:白方
black:当前所下黑子坐标集合
white:当前所下白子坐标集合
*/
// 返回横行的得分
long CalcuScoreBetterCross(const QPoint& point, int color,
                                 const QList<DrawData>& blackArray,
                                 const QList<DrawData>& whiteArray);
// 返回竖行的得分
 long CalcuScoreBetterColumn(const QPoint& point, int color,
                                   const QList<DrawData>& blackArray,
                                   const QList<DrawData>& whiteArray);
// 返回左斜行的得分
 long CalcuScoreBetterLeftSlant(const QPoint& point, int color,
                                      const QList<DrawData>& blackArray,
                                      const QList<DrawData>& whiteArray);
// 返回右斜行的得分
 long CalcuScoreBetterRightSlant(const QPoint& point, int color,
                                       const QList<DrawData>& blackArray,
                                       const QList<DrawData>& whiteArray);
// 返回一个点的估值,启发式函数
 long EvaluateWithPoint(const QPoint& point, int color,
                              const QList<DrawData>& blackArray,
                              const QList<DrawData>& whiteArray);

//
int GetPutPoint(const QList<QPoint>& canPlayArray, int color,
                       const QList<DrawData>& blackArray,
                       const QList<DrawData>& whiteArray);


/* 极大极小值alpha-beta剪枝算法 */
//int MaxMin_AlphaBeta(int player, int depth, QList<Point>& canPlayArray,
 //                  int alpha, int beta);

#endif // EVALUATE_H
