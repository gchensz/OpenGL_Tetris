/*
 *
 * - 已实现功能如下：
 * - 1) 绘制棋盘格
 * - 2) 键盘左/右/下键控制方块的移动，上键旋转方块
 * - 3) ‘L’、‘O’、‘I’、‘S’、‘Z’、‘J’、‘T’型方块
 * - 4) 随机生成不同形状的方块并赋上不同的颜色
 * - 5) 方块的自动向下移动
 * - 6) 方块之间、方块与边界之间的碰撞检测
 * - 7) 棋盘格中每一行填充满之后自动消除，并同时统计消除的得分
 * - 8) 空格键控制方块直接掉落到底部
 * - 9) 游戏结束的检测及结束后在游戏界面绘制红色"over"字样
 *  -10)‘r’键重新开始游戏
 *
 */

#include "Angel.h"

// #pragma comment(lib, "glew32.lib")

#include <cstdlib>
#include <iostream>
#include <string>
#include <time.h>
#include <vector>

int starttime;			// 控制方块向下移动时间
int rotation = 0;		// 控制当前窗口中的方块旋转
vec2 tile[4];			// 表示当前窗口中的方块
bool gameover = false;	// 游戏结束控制变量
int xsize = 400;		// 窗口大小（尽量不要变动窗口大小！）
int ysize = 720;
int speed;			//方块自动下落的速度阈值
int score=0;			//游戏得分

// 一个三维数组表示所有可能出现的方块和方向。
vec2 allRotationsLshape[7][4][4] ={
							{
							{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(-1,-1)},	//   "L"
							{vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, -1)},   //
							{vec2(1, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   //
							{vec2(-1,1), vec2(0, 1), vec2(0, 0), vec2(0, -1)}
							},
{
							{vec2(0,0),vec2(0,-1),vec2(-1,0),vec2(-1,-1)},		// "O"
							{vec2(0,0),vec2(0,-1),vec2(-1,0),vec2(-1,-1)},
							{vec2(0,0),vec2(0,-1),vec2(-1,0),vec2(-1,-1)},
							{vec2(0,0),vec2(0,-1),vec2(-1,0),vec2(-1,-1)}		
},
{
							{vec2(-2,0),vec2(-1,0),vec2(0,0),vec2(1,0)},			// "I"
							{vec2(0,1),vec2(0,0),vec2(0,-1),vec2(0,-2)},
							{vec2(-2,0),vec2(-1,0),vec2(0,0),vec2(1,0)},			
							{vec2(0,1),vec2(0,0),vec2(0,-1),vec2(0,-2)}
},
{
							{vec2(-1,-1),vec2(0,-1),vec2(0,0),vec2(1,0)},		// "S"
							{vec2(0,1),vec2(0,0),vec2(1,0),vec2(1,-1)},
							{vec2(-1,-1),vec2(0,-1),vec2(0,0),vec2(1,0)},		
							{vec2(0,1),vec2(0,0),vec2(1,0),vec2(1,-1)}
},
{
							{vec2(-1,0),vec2(0,0),vec2(0,-1),vec2(1,-1)},		// "Z"
							{vec2(0,-1),vec2(0,0),vec2(1,0),vec2(1,1)},
							{vec2(-1,0),vec2(0,0),vec2(0,-1),vec2(1,-1)},		
							{vec2(0,-1),vec2(0,0),vec2(1,0),vec2(1,1)}
},
{
							{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(1,-1)},	//   "J"
							{vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, 1)},   //
							{vec2(-1, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   //
							{vec2(-1,-1), vec2(0, 1), vec2(0, 0), vec2(0, -1)}						
},
{
							{vec2(0, 0), vec2(-1,0), vec2(1, 0), vec2(0,-1)},	//   "T"
							{vec2(0, 1), vec2(0, 0), vec2(0,-1), vec2(1, 0)},   //
							{vec2(0, 1), vec2(-1,0), vec2(0, 0), vec2(1,  0)},   //
							{vec2(-1,0), vec2(0, 1), vec2(0, 0), vec2(0, -1)}
}		
};	  
int currentShape=0;//记录当前新方块的形状

// 绘制窗口的颜色变量
vec4 orange = vec4(238.0/255.0, 118.0/255.0, 0.0, 1.0);
vec4 white  = vec4(1.0, 1.0, 1.0, 1.0);
vec4 black  = vec4(0.0, 0.0, 0.0, 1.0);
vec4 grey   = vec4(54.0/255.0,54.0/255.0,54.0/255.0,1.0);
vec4 darkred= vec4(1.0,0.0,0.0,1.0);
vec4 red    = vec4(205.0/255.0, 38.0/255.0, 38.0/255.0, 1.0);
vec4 yellow = vec4(1.0, 185.0/255.0, 15.0/255.0, 1.0);
vec4 pink   = vec4(238.0/255.0,99.0/255.0,99.0/255.0, 1.0);
vec4 blue   = vec4(0.0, 1.0, 127.0/255.0, 1.0);
vec4 Turquoise=vec4(0.0,197.0/255.0,205.0/255.0,1.0);
vec4 Aquamarine=vec4(102.0/255.0, 205.0/255.0, 170.0/255.0,1.0);
vec4 NavajoWhite=vec4(205.0/255.0, 179.0/255.0, 139.0/255.0,1.0);
vec4 VioletRed=vec4(238.0/255.0 ,58.0/255.0, 140.0/255.0,1.0);
vec4 SlateBlue=vec4(105.0/255.0, 89.0/255.0, 205.0/255.0,1.0);
vec4 LightGreen=vec4(144.0/255.0, 238.0/255.0, 144.0/255.0,1.0);
vec4 colorsSet[11]={orange,red,yellow,pink,blue,Turquoise,Aquamarine,NavajoWhite,VioletRed,SlateBlue,LightGreen};//保存十一种颜色的数组
int currentColor=0;//记录当前方块的颜色

// 当前方块的位置（以棋盘格的左下角为原点的坐标系）
vec2 tilepos = vec2(5, 19);

// 布尔数组表示棋盘格的某位置是否被方块填充，即board[x][y] = true表示(x,y)处格子被填充。
// （以棋盘格的左下角为原点的坐标系）
bool board[10][20];

// 当棋盘格某些位置被方块填充之后，记录这些位置上被填充的颜色
vec4 boardcolours[1200];

GLuint locxsize;
GLuint locysize;

GLuint vaoIDs[3];
GLuint vboIDs[6];

// 修改棋盘格在pos位置的颜色为colour，并且更新对应的VBO
void changecellcolour(vec2 pos, vec4 colour)
{
	// 每个格子是个正方形，包含两个三角形，总共6个定点，并在特定的位置赋上适当的颜色
	for (int i = 0; i < 6; i++)
		boardcolours[(int)(6*(10*pos.y + pos.x) + i)] = colour;

	vec4 newcolours[6] = {colour, colour, colour, colour, colour, colour};

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);

	// 计算偏移量，在适当的位置赋上颜色
	int offset = 6 * sizeof(vec4) * (int)(10*pos.y + pos.x);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// 当前方块移动或者旋转时，更新VBO
void updatetile()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);

	// 每个方块包含四个格子
	for (int i = 0; i < 4; i++)
	{
		// 计算格子的坐标值
		GLfloat x = tilepos.x + tile[i].x;
		GLfloat y = tilepos.y + tile[i].y;

		vec4 p1 = vec4(33.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p2 = vec4(33.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);
		vec4 p3 = vec4(66.0 + (x * 33.0), 33.0 + (y * 33.0), .4, 1);
		vec4 p4 = vec4(66.0 + (x * 33.0), 66.0 + (y * 33.0), .4, 1);

		// 每个格子包含两个三角形，所以有6个顶点坐标
		vec4 newpoints[6] = {p1, p2, p3, p2, p3, p4};
		glBufferSubData(GL_ARRAY_BUFFER, i*6*sizeof(vec4), 6*sizeof(vec4), newpoints);
	}
	#ifdef __APPLE__
		glBindVertexArrayAPPLE(0);
	#else
		glBindVertexArray(0);
	#endif
}

//游戏结束，棋盘中全部方块变为灰色，并绘制出红色"over"字样
void over()
{
	//全体已填充格子变为灰色
	for(int i=0;i<10;i++)
		for(int j=0;j<20;j++)
			if(board[i][j])	
				changecellcolour(vec2(i,j),grey);

	// 把最后一个方块变为灰色
	vec4 newcolours[24];
	for (int i = 0; i < 24; i++)
		newcolours[i] = grey;

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	#ifdef __APPLE__
		glBindVertexArrayAPPLE(0);
	#else
		glBindVertexArray(0);
	#endif	

	//绘制红色"over"字样
	//绘制"e r"
	for(int i=1;i<=4;i++)
		changecellcolour(vec2(i,8),darkred);
	changecellcolour(vec2(6,8),darkred);
	changecellcolour(vec2(1,9),darkred);
	changecellcolour(vec2(6,9),darkred);
	for(int i=1;i<=4;i++)
		changecellcolour(vec2(i,10),darkred);
	changecellcolour(vec2(6,10),darkred);
	for(int i=0;i<9;i++)
		if((i==1)||(i==4)||(i==6))
			changecellcolour(vec2(i,11),darkred);
	for(int i=1;i<=4;i++)
		changecellcolour(vec2(i,12),darkred);
	for(int i=6;i<=9;i++)
		changecellcolour(vec2(i,12),darkred);
	//绘制"o v"
	for(int i=1;i<=4;i++)
		changecellcolour(vec2(i,14),darkred);
	changecellcolour(vec2(7,14),darkred);
	changecellcolour(vec2(8,14),darkred);
	for(int i=0;i<=9;i++)
		if((i==1)||(i==4)||(i==6)||(i==9))
			changecellcolour(vec2(i,15),darkred);
	for(int i=0;i<=9;i++)
		if((i==1)||(i==4)||(i==6)||(i==9))
			changecellcolour(vec2(i,16),darkred);	
	for(int i=0;i<=9;i++)
		if((i==1)||(i==2)||(i==3)||(i==4)||(i==6)||(i==9))
			changecellcolour(vec2(i,17),darkred);	
}

// 设置当前方块为下一个即将出现的方块。在游戏开始的时候调用来创建一个初始的方块，
// 在游戏结束的时候判断，没有足够的空间来生成新的方块。
void newtile()
{
	//游戏结束则停止生成新方块
	if(gameover)
		return;	

	// 将新方块放于棋盘格的最上行中间位置并设置默认的旋转方向
	tilepos = vec2(5 , 19);
	rotation = 0;
	speed=40;	//设置初始速度阈值

	//随机选择方块的形状
	srand ( (unsigned) time (NULL) );
	currentShape=rand()%7;//调用随机函数选择新方块形状 并 记录新方块形状
	for (int i = 0; i < 4; i++)
	{
		tile[i] = allRotationsLshape[currentShape][rotation][i];
	}	
	
	updatetile();

	//随机选择当前新方块的颜色
	srand ( (unsigned) time (NULL) );
	currentColor=rand()%11;//调用随机函数选择11种颜色中一个 并 记录当前颜色的下标
	vec4 randomColor=colorsSet[currentColor];	

	// 给新方块赋上颜色
	vec4 newcolours[24];
	for (int i = 0; i < 24; i++)
		newcolours[i] = randomColor;

	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(newcolours), newcolours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	#ifdef __APPLE__
		glBindVertexArrayAPPLE(0);
	#else
		glBindVertexArray(0);
	#endif
}

// 游戏和OpenGL初始化
void init()
{
	// 11条竖线，21条横线
	// 初始化棋盘格，包含64个顶点坐标（总共32条线），并且每个顶点一个颜色值
	vec4 gridpoints[64];
	vec4 gridcolours[64];

	// 纵向线
	for (int i = 0; i < 11; i++)
	{
		gridpoints[2*i] = vec4((33.0 + (33.0 * i)), 33.0, 0, 1);
		gridpoints[2*i + 1] = vec4((33.0 + (33.0 * i)), 693.0, 0, 1);
	}

	// 水平线
	for (int i = 0; i < 21; i++)
	{
		gridpoints[22 + 2*i] = vec4(33.0, (33.0 + (33.0 * i)), 0, 1);
		gridpoints[22 + 2*i + 1] = vec4(363.0, (33.0 + (33.0 * i)), 0, 1);
	}

	// 将所有线赋成白色
	for (int i = 0; i < 64; i++)
		gridcolours[i] = white;

	// 初始化棋盘格，并将没有被填充的格子设置成黑色
	vec4 boardpoints[1200];
	for (int i = 0; i < 1200; i++)
		boardcolours[i] = black;

	// 对每个格子，初始化6个顶点，表示两个三角形，绘制一个正方形格子
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			vec4 p1 = vec4(33.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p2 = vec4(33.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);
			vec4 p3 = vec4(66.0 + (j * 33.0), 33.0 + (i * 33.0), .5, 1);
			vec4 p4 = vec4(66.0 + (j * 33.0), 66.0 + (i * 33.0), .5, 1);
			boardpoints[6*(10*i + j)    ] = p1;
			boardpoints[6*(10*i + j) + 1] = p2;
			boardpoints[6*(10*i + j) + 2] = p3;
			boardpoints[6*(10*i + j) + 3] = p2;
			boardpoints[6*(10*i + j) + 4] = p3;
			boardpoints[6*(10*i + j) + 5] = p4;
		}

	// 将棋盘格所有位置的填充与否都设置为false（没有被填充）
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
			board[i][j] = false;

	// 载入着色器
	std::string vshader, fshader;
	#ifdef __APPLE__	// for MacOS
		vshader = "shaders/vshader_mac.glsl";
		fshader = "shaders/fshader_mac.glsl";
	#else				// for Windows
		vshader = "shaders/vshader_win.glsl";
		fshader = "shaders/fshader_win.glsl";
	#endif
	GLuint program = InitShader(vshader.c_str(), fshader.c_str());
	glUseProgram(program);

	locxsize = glGetUniformLocation(program, "xsize");
	locysize = glGetUniformLocation(program, "ysize");

	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	GLuint vColor = glGetAttribLocation(program, "vColor");

	#ifdef __APPLE__
		glGenVertexArraysAPPLE(3, &vaoIDs[0]);
		glBindVertexArrayAPPLE(vaoIDs[0]);	// 棋盘格顶点
	#else
		glGenVertexArrays(3, &vaoIDs[0]);
		glBindVertexArray(vaoIDs[0]);		// 棋盘格顶点
	#endif
	glGenBuffers(2, vboIDs);

	// 棋盘格顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, 64*sizeof(vec4), gridcolours, GL_STATIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	#ifdef __APPLE__
		glBindVertexArrayAPPLE(vaoIDs[1]);	// 棋盘格每个格子
	#else
		glBindVertexArray(vaoIDs[1]);		// 棋盘格每个格子
	#endif
	glGenBuffers(2, &vboIDs[2]);

	// 棋盘格每个格子顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardpoints, GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 棋盘格每个格子顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[3]);
	glBufferData(GL_ARRAY_BUFFER, 1200*sizeof(vec4), boardcolours, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	#ifdef __APPLE__
		glBindVertexArrayAPPLE(vaoIDs[2]);	// 当前方块
	#else
		glBindVertexArray(vaoIDs[2]);		// 当前方块
	#endif
	glGenBuffers(2, &vboIDs[4]);

	// 当前方块顶点位置
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[4]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	// 当前方块顶点颜色
	glBindBuffer(GL_ARRAY_BUFFER, vboIDs[5]);
	glBufferData(GL_ARRAY_BUFFER, 24*sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

	#ifdef __APPLE__
		glBindVertexArrayAPPLE(0);
	#else
		glBindVertexArray(0);
	#endif
	glClearColor(0, 0, 0, 0);

	// 游戏初始化
	//游戏状态变量初始化
	gameover=false;		//游戏结束状态重置
	score=0;		//游戏分数置0
	speed=40;		//速度阈值重置为40
	newtile();
	starttime = glutGet(GLUT_ELAPSED_TIME);
}

// 检查在cellpos位置的格子是否被填充或者是否在棋盘格的边界范围内
bool checkvalid(vec2 cellpos)
{
	if((cellpos.x >=0) && (cellpos.x < 10) && (cellpos.y >= 0) && (cellpos.y <20) && (!board[(int)cellpos.x][(int)cellpos.y]) )
		return true;
	else
		return false;
}

// 在棋盘上有足够空间的情况下旋转当前方块
void rotate()
{
	// 计算得到下一个旋转方向
	int nextrotation = (rotation + 1) % 4;

	// 检查当前旋转之后的位置的有效性
	if (checkvalid((allRotationsLshape[currentShape][nextrotation][0]) + tilepos)
		&& checkvalid((allRotationsLshape[currentShape][nextrotation][1]) + tilepos)
		&& checkvalid((allRotationsLshape[currentShape][nextrotation][2]) + tilepos)
		&& checkvalid((allRotationsLshape[currentShape][nextrotation][3]) + tilepos))
	{
		// 更新旋转，将当前方块设置为旋转之后的方块
		rotation = nextrotation;
		for (int i = 0; i < 4; i++)
			tile[i] = allRotationsLshape[currentShape][rotation][i];

		updatetile();
	}
}

// 检查棋盘格在row行有没有被填充满
bool checkfullrow(int row)
{
	//判断在row行是否全满，有空格子则结束判断并退出
	for(int i=0;i<10;i++)
	{
		if(!board[i][row])
			return false;
	}
	//row行全满，该行上方整体方块向下移动
	for(int i=0;i<10;i++)
	{
		for(int j=row;j<19;j++)
		{
			board[i][j]=board[i][j+1];
			changecellcolour(vec2(i,j),boardcolours[6*(10*(j+1)+i)]);//获取正上方格子顶点的颜色并修改
		}
	}
	std::cout<<"YOUR SCORE: "<<++score<<std::endl;
	return true;
}

// 放置当前方块，并且更新棋盘格对应位置顶点的颜色VBO
void settile()
{
	// 每个格子
	for (int i = 0; i < 4; i++)
	{
		// 获取格子在棋盘格上的坐标
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;

		// 将格子对应在棋盘格上的位置设置为填充
		board[x][y] = true;
		// 并将相应位置的颜色修改
		changecellcolour(vec2(x,y), colorsSet[currentColor]);
	}

	//判断j行是否填满需要消除
	int i=0;
	for(int j=0;j<20 && i<4 ;j++)
	{
		if(checkfullrow(j))
		{
			i++;
			j--;
		}
	}

	//判断放置当前方块后游戏是否结束
	for (int i = 0; i < 4; i++)
	{
		// 获取格子在棋盘格上的坐标
		int x = (tile[i] + tilepos).x;
		int y = (tile[i] + tilepos).y;
		//方块堆到棋盘顶部边界则游戏结束
		if(y==19)
		{
			gameover=true;
			std::cout<<"GAME OVER! YOUR SCORE: "<<score<<std::endl;
			over();
			return ;
		}
	}
}

// 给定位置(x,y)，移动方块。有效的移动值为(-1,0)，(1,0)，(0,-1)，分别对应于向
// 左，向下和向右移动。如果移动成功，返回值为true，反之为false
bool movetile(vec2 direction)
{
	// 计算移动之后的方块的位置坐标
	vec2 newtilepos[4];
	for (int i = 0; i < 4; i++)
		newtilepos[i] = tile[i] + tilepos + direction;

	// 检查移动之后的有效性
	if (checkvalid(newtilepos[0])
		&& checkvalid(newtilepos[1])
		&& checkvalid(newtilepos[2])
		&& checkvalid(newtilepos[3]))
		{
			// 有效：移动该方块
			tilepos.x = tilepos.x + direction.x;
			tilepos.y = tilepos.y + direction.y;

			updatetile();

			return true;
		}

	return false;
}

// 重新启动游戏
void restart()
{
	init();
}

// 游戏渲染部分
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUniform1i(locxsize, xsize);
	glUniform1i(locysize, ysize);

	#ifdef __APPLE__	// for MacOS
		glBindVertexArrayAPPLE(vaoIDs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 1200); // 绘制棋盘格 (10*20*2 = 400 个三角形)
		glBindVertexArrayAPPLE(vaoIDs[2]);
		glDrawArrays(GL_TRIANGLES, 0, 24);	 // 绘制当前方块 (8 个三角形)
		glBindVertexArrayAPPLE(vaoIDs[0]);
		glDrawArrays(GL_LINES, 0, 64);		 // 绘制棋盘格的线
	#else				// for Windows
		glBindVertexArray(vaoIDs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 1200); // 绘制棋盘格 (10*20*2 = 400 个三角形)
		glBindVertexArray(vaoIDs[2]);
		glDrawArrays(GL_TRIANGLES, 0, 24);	 // 绘制当前方块 (8 个三角形)
		glBindVertexArray(vaoIDs[0]);
		glDrawArrays(GL_LINES, 0, 64);		 // 绘制棋盘格的线
	#endif

	glutSwapBuffers();
}

// 在窗口被拉伸的时候，控制棋盘格的大小，使之保持固定的比例。
void reshape(GLsizei w, GLsizei h)
{
	xsize = w;
	ysize = h;
	glViewport(0, 0, w, h);
}

// 键盘响应事件中的特殊按键响应
void special(int key, int x, int y)
{
	if(!gameover)
	{
		switch(key)
		{
			case GLUT_KEY_UP:	// 向上按键旋转方块
				rotate();
				break;
			case GLUT_KEY_DOWN: // 向下按键移动方块
				if (!movetile(vec2(0, -1)))
				{
					settile();
					newtile();
				}
				break;
			case GLUT_KEY_LEFT:  // 向左按键移动方块
				movetile(vec2(-1, 0));
				break;
			case GLUT_KEY_RIGHT: // 向右按键移动方块
				movetile(vec2(1, 0));
				break;
		}
	}
}

// 键盘响应时间中的普通按键响应
void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 033: // ESC键 和 'q' 键退出游戏
			exit(EXIT_SUCCESS);
			break;
		case 'q':
			exit (EXIT_SUCCESS);
			break;
		case 'r': // 'r' 键重启游戏
			restart();
			break;
		case 040://空格键使方块直接下落到底部
			while(movetile(vec2(0, -1)))
			{}
			settile();
			newtile();
			break;
	}
	glutPostRedisplay();
}

//空闲回调函数，自动使方块向下移动
void idle(void)
{
	//方块自动下落
	static int time=0;
	if(time++ ==speed)
	{
		special(GLUT_KEY_DOWN,0,0);
		time=0;
	}

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(xsize, ysize);
	glutInitWindowPosition(680, 130);
	glutCreateWindow("2017153028_陈钢_期中大作业");
	
	#ifdef WIN32
		glewInit();
	#endif
	
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
