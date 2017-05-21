#pragma once
#include "Common.h"
class Calculator{
private:
public:
	int get(){};
	int epsilonGreedy(double EPSILON, double DIRECTION_NUM, double ***q_num, int now_player_point_x, int now_player_point_y){
		int seed = rand() % 100 + 1;	//ƒ‰ƒ“ƒ_ƒ€‚És“®‚·‚é‚©”Û‚©‚ðŒˆ’è‚·‚é•Ï”
		int move_direction = 0;			//ˆÚ“®•ûŒüŠi”[•Ï”
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