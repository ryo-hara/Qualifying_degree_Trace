#pragma once
#include "Common.h"
class Calculator{
private:
public:
	int get(){};
	int epsilonGreedy(double EPSILON, double DIRECTION_NUM, double ***q_num, int now_player_point_x, int now_player_point_y){
		int seed = rand() % 100 + 1;	//�����_���ɍs�����邩�ۂ������肷��ϐ�
		int move_direction = 0;			//�ړ������i�[�ϐ�
		if (seed > EPSILON * 100){
			move_direction = rand() % 4;
		}
		else{
			move_direction = rand() % 4;
			for (int d = 0; d < DIRECTION_NUM; d++){
				if (q_num[now_player_point_y][now_player_point_x][d] > q_num[now_player_point_y][now_player_point_x][move_direction]){
					move_direction = d;
				}
			}
		}
		return move_direction;
	}
};