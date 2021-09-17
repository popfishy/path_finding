#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include "A_Star.h"

//声明各种变量
int open_list_count = 0;
int close_list_count = 0;
AStarNode map_nodes[ROWS][COLS];
AStarNode *start_node;
AStarNode *end_node;
AStarNode *curr_node;
AStarNode *open_list[ROWS*COLS];
AStarNode *close_list[ROWS*COLS];
AStarNode *final_path[ROWS*COLS];
int top;
int step_count;

int main()
{
	int flag = 0;
	top = 0;
	step_count = 0;
	//地图基础数据
	int map[][COLS]= {
						{ 0,0,0,3,0,3,0,0,0,0 },
						{ 0,0,0,0,0,3,0,3,2,3 },
						{ 3,0,3,0,0,3,3,3,0,3 },
						{ 3,0,3,0,0,0,0,0,0,3 },
						{ 3,0,3,0,0,3,0,0,0,3 },
						{ 3,0,3,3,0,0,0,3,0,3 },
						{ 3,1,3,0,0,3,3,0,0,0 },
						{ 0,3,0,0,0,0,0,0,0,0 },
						{ 3,3,3,0,0,3,0,3,0,3 },
						{ 3,0,0,0,0,3,3,3,0,3 },
	};
	//初始化地图基础属性
	for (int i = 0; i < ROWS; ++i) {
		for (int j = 0; j < COLS; ++j) {
			map_nodes[i][j].g = 0;
			map_nodes[i][j].h = 0;
			map_nodes[i][j].is_in_closetable = 0;
			map_nodes[i][j].is_in_opentable = 0;
			map_nodes[i][j].style = map[i][j];
			map_nodes[i][j].x = i;
			map_nodes[i][j].y = j;
			map_nodes[i][j].parent = NULL;

			if (map_nodes[i][j].style == STARTPOINT)  // ���
			{
				start_node = &(map_nodes[i][j]);
			}
			else if (map_nodes[i][j].style == ENDPOINT)    // �յ�
			{
				end_node = &(map_nodes[i][j]);
			}
			printf("%d", map[i][j]);
		}
		printf("\n");
	}

	//初始化起点
	open_list[open_list_count++] = start_node;
	start_node->is_in_opentable = 1;
	start_node->g = 0;
	start_node->h = abs(end_node->x - start_node->x) + abs(end_node->y - start_node->y);
	start_node->parent = NULL;

	//判断是否到达终点
	if (start_node->x == end_node->x && start_node->y == end_node->y)
	{
		printf("已到达终点\n");
		return 0;
	}
	flag = 0;

	//
	while (1)
	{
		curr_node = open_list[0];		// open表中最小值得点
		open_list[0] = open_list[--open_list_count];  // 进行堆排序
		Adjust_heap(0);

		close_list[close_list_count++] = curr_node;    // 当前点加入close表中
		curr_node->is_in_closetable = 1;

		if (curr_node->x == end_node->x && curr_node->y == end_node->y)// 如果终点在close表中
		{
			flag = 1;
			break;
		}

		Find_neighbors(curr_node, end_node);           // 找当前点的邻居点

		if (open_list_count == 0)             // 没有路径
		{
			flag = 0;
			break;
		}
	}

	//如果找到了路径
	if (flag == 1)
	{
		curr_node = end_node;
		while (curr_node != NULL)
		{
			final_path[++top] = curr_node;
			curr_node = curr_node->parent;
		}
		step_count = top - 1;
		for (int k = top-1; k >= 1; k--) {
			system("cls");
			if (k != 1)
			{
				map[final_path[k]->x][final_path[k]->y] = ROBOT;
			}
			Show(map);
			Sleep(200);
			if (k == 1)
			{
				system("PAUSE");
			}
		}
	}
	else if(flag == 0)
	{
		printf("\n");
		printf("没有路径到达");
	}

	return 0;
}

