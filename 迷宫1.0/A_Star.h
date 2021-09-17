
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>

#define ROAD		 0		//可走的点
#define STARTPOINT   1		//起点
#define ENDPOINT     2		//终点
#define BARRIER      3		//障碍物
#define ROBOT        4		//表示机器人
#define ROWS 10				//迷宫长度
#define COLS 10				//迷宫宽度

typedef struct AStarNode
{
	int x;    // 坐标(最终输出路径需要)
	int y;
	int g;    // 起点到此点的距离( 由g和h可以得到f，f=g+h ，用来衡量路径中的最优点)
	int h;    // 启发函数预测的此点到终点的距离
	int style;// 结点类型：起点，终点，障碍物
	struct AStarNode *parent;    // 父节点
	int is_in_closetable;     // 是否在close表中
	int is_in_opentable;      // 是否在open表中
}AStarNode,*pAStarNode;

extern AStarNode map_nodes[][COLS];		//迷宫地图

//声明各种变量
extern int open_list_count;			//open表中节点数量
extern int close_list_count;		//close表中节点数量
extern AStarNode map_nodes[][COLS];	//地图节点
extern AStarNode *start_node;
extern AStarNode *end_node;
extern AStarNode *curr_node;		//当前点
extern AStarNode *open_list[];		//open表
extern AStarNode *close_list[];		//close表
extern int map[][COLS];				//迷宫地图
extern int step_count;
void Swap(int idx1, int idx2);

void Adjust_heap(int nIndex);

void Find_neighbors(AStarNode *curr_node, AStarNode *end_node);

void Add_to_open_list(int x, int y, AStarNode *curr_node, AStarNode *end_node, int w);

void Show(int map[][10]);




