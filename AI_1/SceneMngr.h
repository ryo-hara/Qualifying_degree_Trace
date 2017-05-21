#pragma once

using namespace std;
#include <list>

enum Session_State{
	SESSIONSTATE_DROPDAWN_HOLE,
	SESSIONSTATE_TIMEOUT,
	SESSIONSTATE_GOAL,
	SESSIONSTATE_NORMAL,
	SESSIONSTATE_HIT_WALL,
};

class SceneMngr{
private:
	int success_num;
	int first_success_num;
	bool scene_refresh_flag;
	int session_state;
	int min_success_num;
	double one_Story_Q_num;
	SceneMngr(){
		this->initialize();
	}
public:
	struct Counter{
		int number_of_trials;
	};
	static SceneMngr* getInstance(){//このままだと正しくメモリ解放されないっぽいのでスマートポインタ噛ませるべきでして
		static SceneMngr * scenemngr = new SceneMngr();
		return scenemngr;
	}
	Counter *counter;
	list<Counter*> Counter_List;

	void initialize(){
		one_Story_Q_num = 0;
		success_num = 0;
		first_success_num = -1;
		scene_refresh_flag = false;
		session_state = SESSIONSTATE_NORMAL;
		min_success_num = 100000;
	}

	void setRefreshFlag(bool flag){ this->scene_refresh_flag = flag; }
	bool getRefreshFlag(){ return this->scene_refresh_flag; }
	void setSessionState(int state){ this->session_state = state; }
	int	 getSessionState(){ return this->session_state; }
	void setFirstSuccess(int num){ if (first_success_num == -1)first_success_num = num; }
	int getFirstSuccess(){ return this->first_success_num; }

	void addStoryRewards(double a){ one_Story_Q_num += a; }
	double getStoryRewards(){ return this->one_Story_Q_num; }
	void initializeStoryWards(){ this->one_Story_Q_num = 0; }

	void setMinSucess(int n){ if(n < min_success_num)min_success_num = n; }
	int getMinSccess(){ return min_success_num; }

	void countSuccess(){ success_num++; }
	int getSuccessNum(){ return this->success_num; }
};