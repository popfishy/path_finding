#include <stdio.h>
#include <stdlib.h>
#include "A_Star.h"

//两个数互换
void Swap(int idx1, int idx2)
{
	AStarNode *temp;
	temp = open_list[idx1];
	open_list[idx1] = open_list[idx2];
	open_list[idx2] = temp;
}

// 二叉堆调整（最小堆）
void Adjust_heap(int nIndex)
{
	int curr = nIndex;
	int child = curr * 2 + 1;   // 得到左节点idx( 二叉堆下标从0开始，所有左节点下标为curr*2+1 )
	int parent = (curr - 1) / 2;  // 得到父节点idx

	if (nIndex < 0 || nIndex >= open_list_count)	return;

	// 调整index大于curr的点的顺序 
	while (child < open_list_count)
	{
		// 最小堆是父节点的值小于子节点的值
		if ((child + 1 < open_list_count && open_list[child]->g + open_list[child]->h) > (open_list[child + 1]->g + open_list[child + 1]->h))
		{
			++child;						// 判断左右子节点大小
		}

		if ((open_list[curr]->g + open_list[curr]->h) <= (open_list[child]->g + open_list[child]->h))
		{
			break;							//节点顺序调整完毕，跳出循环
		}
		else
		{
			Swap(child, curr);              // 交换节点
			curr = child;                   //重复循环调整节点顺序
			child = curr * 2 + 1;           
		}
	}

	//调整index小于curr的点的顺序
	while (curr != 0)
	{
		if ((open_list[curr]->g + open_list[curr]->h) >= (open_list[parent]->g + open_list[parent]->h))
		{
			break;							// 节点顺序调整完毕，跳出循环
		}
		else
		{
			Swap(curr, parent);				// 交换节点
			curr = parent;					//重复循环调整节点顺序
			parent = (curr - 1) / 2;
		}
	}
}

//首先判断当前点周围的点能否加入open_list，能加入open_list的点则进行一系列处理
//w表示左右相邻点之间的距离，默认是10；斜对角线相邻格子之间的距离是14
void Add_to_open_list(int x, int y, AStarNode *curr_node, AStarNode *end_node, int w) {
	int i = 0;
	if (map_nodes[x][y].style != BARRIER)        // 不是障碍物
	{
		if (!map_nodes[x][y].is_in_closetable)   // 不在闭表中
		{
			if (map_nodes[x][y].is_in_opentable) // 在open表中
			{
				// 需要判断是否是一条更优化的路径
				//
				if (map_nodes[x][y].g > curr_node->g + w)    // 如果更优化
				{
					map_nodes[x][y].g = curr_node->g + w;
					map_nodes[x][y].parent = curr_node;

					for (i = 0; i < open_list_count; ++i)
					{
						if (open_list[i]->x == map_nodes[x][y].x && open_list[i]->y == map_nodes[x][y].y)
						{
							break;
						}
					}

					Adjust_heap(i);                   // 下面调整点
				}
			}
			else                                    // 不在open中
			{
				map_nodes[x][y].g = curr_node->g + w;
				map_nodes[x][y].h = abs(end_node->x - x) + abs(end_node->y - y);
				map_nodes[x][y].parent = curr_node;
				map_nodes[x][y].is_in_opentable = 1;
				open_list[open_list_count++] = &(map_nodes[x][y]);
			}
		}
	}
}

//找到周围4个相邻的点，并进行处理
//左右相邻点之间的距离默认是10,斜对角线相邻点之间的距离默认是14
void Find_neighbors(AStarNode *curr_node, AStarNode *end_node) {
	int x = curr_node->x;
	int y = curr_node->y;
	if ((x + 1) >= 0 && (x + 1) < ROWS && y >= 0 && y < COLS)
	{
		Add_to_open_list(x + 1, y, curr_node, end_node, 10);
	}

	if ((x - 1) >= 0 && (x - 1) < ROWS && y >= 0 && y < COLS)
	{
		Add_to_open_list(x - 1, y, curr_node, end_node, 10);
	}

	if (x >= 0 && x < ROWS && (y + 1) >= 0 && (y + 1) < COLS)
	{
		Add_to_open_list(x, y + 1, curr_node, end_node, 10);
	}

	if (x >= 0 && x < ROWS && (y - 1) >= 0 && (y - 1) < COLS)
	{
		Add_to_open_list(x, y - 1, curr_node, end_node, 10);
	}
	/*if ((x + 1) >= 0 && (x + 1) < ROWS && (y + 1) >= 0 && (y + 1) < COLS)
	{
		Add_to_open_list(x + 1, y + 1, curr_node, end_node, 14);
	}

	if ((x + 1) >= 0 && (x + 1) < ROWS && (y - 1) >= 0 && (y - 1) < COLS)
	{
		Add_to_open_list(x + 1, y - 1, curr_node, end_node, 14);
	}

	if ((x - 1) >= 0 && (x - 1) < ROWS && (y + 1) >= 0 && (y + 1) < COLS)
	{
		Add_to_open_list(x - 1, y + 1, curr_node, end_node, 14);
	}

	if ((x - 1) >= 0 && (x - 1) < ROWS && (y - 1) >= 0 && (y - 1) < COLS)
	{
		Add_to_open_list(x - 1, y - 1, curr_node, end_node, 14);
	}*/
}

//打印地图
void Show(int map[][10]) {
	int i, j;
	for (i = 0; i < 10; ++i) {
		for (j = 0; j < 10; ++j) {
			if (map[i][j] == ROAD)				printf(" ");	//显示可通行的道路
			else if (map[i][j] == STARTPOINT)   printf("1");	//显示起点
			else if (map[i][j] == ENDPOINT)	    printf("2");	//显示终点
			else if (map[i][j] == BARRIER)		printf("*");	//显示障碍
			else if (map[i][j] == ROBOT)		printf("o");	//显示机器人
		}
		printf("\n");
	}
	printf("\n");
	printf("起点的坐标是(%d,%d)，终点的坐标是(%d,%d),最短路径需要走%d步\n",
		start_node->x, start_node->y, end_node->x, end_node->y, step_count);
}


