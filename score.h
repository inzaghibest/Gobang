/* 棋型表示 */

/*分值定义
可以成五:100000;
活4 100000；
冲4 1000；
死3活3 1000;
死4 500;
低级死4 400;
单活3 100;
跳活3 90;
双活2 50;
活2 10;
低级活2 9;
死3 5;
死2 2;
其他 1;
*/

const long WIN5 = 100000; // 可以成五
const long ALIVE4 = 10000; // 活4
const long DOUBLEALIVE3 = 10000; // 两个以上的活3应该具有和活4一样的分值
const long DIE4 = 9000; // 死4
const long DOUBLEDIE4 = 10000; // 两个以上的死4应该具有单活4一样的分值
const long DIE4ALIVE3 = 10000; // 死4活3也是很高的分值
const long LOWDIE4 =  1000; // 低级死4
const long ALIVE3 = 800; // 活3
const long JUMPLIVE3 = 700; // 跳活3
const long ALIVE2 = 200;  // 活2
const long DOUBLEALIVE2 = 400; // 双活2比单活2要高
const long LOWALIVE2 = 100; // 低级活2
const long DIE3 = 50; // 死3
const long DIE2 = 20;  // 死2
const long ALIVE1 = 10; // 活1
const long DIE1 = 1; // 死1
const long NOTHING = 0;

//

#ifndef SCORE_H
#define SCORE_H

#endif // SCORE_H
