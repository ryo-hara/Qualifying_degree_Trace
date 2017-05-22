//�Q�l���T�C�g/*http://nkdkccmbr.hateblo.jp/entry/2016/10/08/205107*/

#pragma once
#include "OutputFileSystem.h"
#include "SceneMngr.h"
#include "AXSS.h"
#include "Common.h"
#include "Calculate.h"
//#define EPSILON	0.30		//��-greedy�@�̃ÁA���̊����Ń����_���Ɉړ�
//#define ALPHA	0.10		//�w�K����
//#define GAMMA	0.90		//��������
#define Lamda	0.10


enum Move_DIRECTION{//�ړ�����
	UP		=	0,
	DOWN	=	1,
	RIGHT	=	2,
	LEFT	=	3,
};

enum MapChipAssignment{
	MCA_WALL = 1,
	MCA_HOLE = 9,
	MCA_PLAYER_START =  50,
	MCA_PLAYER_GOAL = 60,
};

enum Penalty{
	PENALTY_HIT_WALL = 0,//-100,
	PENALTY_DROPDAWN_HOLE = -100,
	//PENALTY_ONE_STEP = -0,
};
#define PENALTY_ONE_STEP  -1;
enum Rewards{
	REWARDS_GOAL	=	12,//500,
};

#define DIRECTION_NUM 4//������
//#define INITIALIZE_Q_MAX 30
//#define MAX_NUMBER_OF_MOVEMENTS	50//�s���ł���ő��
//#define MAX_SCENE_TRIALNUM		100000

class Scene{
private:
	int **map;		//��̔z�u�i�[�ϐ�
	double ***q_num;	//Q�l�i�[�ϐ�
	double ***e_num;
	int map_size_x, map_size_y;
	int now_movement_num;

	double EPSILON,GAMMA,ALPHA;
	int INITIALIZE_Q_MAX, MAX_NUMBER_OF_MOVEMENTS, MAX_SCENE_TRIALNUM;

	int now_player_point_x, now_player_point_y;
	int next_player_point_x, next_player_point_y;
	int default_start_point_x, default_start_point_y;
	int goal_point_x, goal_point_y;
	void readMap(char *c);
public:

	Cordinate *cordinate;
	list<Cordinate*> Cordinate_List;
	SceneMngr *SM = SceneMngr::getInstance();
	AXSS CFS;
	OutputFileSystem OFS;
	Calculator Cal;
	Scene();

	void initialize();
	virtual void finalize(){}
	void update();
	void draw();

	void initializeQNum();
	void initializeENum();
	double updateE(int time, int x, int y, int d);
	void setScene(char *c);
	void setConstant();
};


class Scene_1 : public Scene{
private:

public:

	Scene_1(){
		//setScene("Map\\Scene1.txt");
		setScene("Map\\Scene_kougiyou.txt");
	}

};