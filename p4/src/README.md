## 主要函数（wushx）

##### game

##### main

## 辅助函数1（wushx）

##### 阅读按键

## 辅助函数2（xuyt）

##### food0

- 不太丑即可
- 命名为food.S（作为接口）
- 参数为x,y
- 第零种食物 加长 绿色空心圆圈


##### food1

- 不太丑即可
- 命名为food.S（作为接口）
- 参数为x,y
- 第一种食物 变短 黄色实心圆圈


##### food2

- 不太丑即可
- 命名为food.S（作为接口）
- 参数为x,y
- 第零种食物 死亡 红色实心圆圈

##### snakehead

- 不太丑即可，可以画一个奇怪的圈之类的
- 命名为snakehead.S（作为接口）
- 参数为x,y，坐标

##### sankebody

- 不太丑即可，一节蛇身，可以搞一个节之类的
- 命名为snakebody.S（作为接口）
- 参数为x,y，坐标

## 界面（xuyt）

##### 初始界面(start menu)

- 不需要完成play和help的选择，后期完成
- 命名为startmenu.S（作为接口）

##### 游戏指南(help menu)

- 不太丑即可
- 命名为helpmenu.S（作为接口）

##### 玩游戏(gameplay)

- 后期完成

##### 游戏失败（打印分数）(scoreboard)

- 不需要完成retry和quit的选择
- 命名为scoreboard.S（作为接口）
- 参数为a0，分数

#### 宗旨：只做美工

## 另
- difficulty.S（作为接口）
- easy
- normal
- hard
- hell


## 另
- otherchoice.S（作为接口）
- 第一个参数 0 1 2 3 代表上下四个
- 第二个参数 0擦除 1写入