#include "Scene.h"

Scene::Scene(){
	srand((unsigned)time(NULL));
	this->setConstant();//config系定数系の読み込み
	this->initializeQNum();
	this->map_size_x = 0;
	this->map_size_y = 0;
	this->now_movement_num = 0;
}

void Scene::draw(){

	/*マップ描画部*/
	for (int y = 0; y < this->map_size_y;y++){
		for (int x = 0; x < this->map_size_x; x++){
			if (map[y][x] < 10)printf(" ");
			printf("%d,",map[y][x]);
		}
		printf("\n");
	}
}



void Scene::update(){
	this->draw();
	this->OFS.makeOFSFile();

	for (int scenenum = 0; scenenum < MAX_SCENE_TRIALNUM;scenenum++){
		this->initialize();
		//Listコンテナに現在の座標を格納
		cordinate = new Cordinate;
		cordinate->x = now_player_point_x; cordinate->y = now_player_point_y; cordinate->q = 0.0f;
		Cordinate_List.push_back(cordinate);
		SM->initializeStoryWards();

		/*イプシロングリーディー法*/
		int move_direction = Cal.epsilonGreedy(EPSILON,DIRECTION_NUM,q_num,now_player_point_x,now_player_point_y);


		for (int move=0; move < 1000;move++){
			/*行動aを行った先のrとS'を確認*/
			double reward = 0;
			int vector_x = 0, vector_y = 0;
			switch (move_direction){
			case UP:
				vector_y = -1;
				break;
			case DOWN:
				vector_y = 1;
				break;
			case LEFT:
				vector_x = -1;
				break;
			case RIGHT:
				vector_x = 1;
				break;
			default:
				exit(0);
			}
			this->next_player_point_x = this->now_player_point_x + vector_x;
			this->next_player_point_y = this->now_player_point_y + vector_y;
			if (next_player_point_x < 0 || next_player_point_x >= this->map_size_x || next_player_point_y < 0 || next_player_point_y >= map_size_y){
				printf("\n\n--ERROR--");
				return;
			}
						//r値確定
			if (map[next_player_point_y][next_player_point_x] == MCA_WALL){			//移動先が壁の場合
				vector_x = 0; vector_y = 0;
				next_player_point_x = now_player_point_x;				
				next_player_point_y = now_player_point_y;
				reward += PENALTY_HIT_WALL;
				SM->setSessionState(SESSIONSTATE_HIT_WALL);
			}
			else
			if (map[next_player_point_y][next_player_point_x] == MCA_HOLE){			//移動先が穴の場合
				reward += PENALTY_DROPDAWN_HOLE;
				this->next_player_point_x = this->default_start_point_x;//
				this->next_player_point_y = this->default_start_point_y;//
				SM->setSessionState(SESSIONSTATE_DROPDAWN_HOLE);
			}
			else
			if (map[next_player_point_y][next_player_point_x] == MCA_PLAYER_GOAL){	//移動先がゴールの場合
				reward += REWARDS_GOAL;
				SM->setSessionState(SESSIONSTATE_GOAL);
			}
			else{																	//それ以外の場合
				SM->setSessionState(SESSIONSTATE_NORMAL);
			}
			//ゴール以外ならマイナス
			if (SM->getSessionState() != SESSIONSTATE_GOAL)reward += PENALTY_ONE_STEP;
			SM->addStoryRewards(reward);



			/*Qs',aを基に行動a'を選択。*/
			int q_max = rand()%4;
			for (int n = 0; n < DIRECTION_NUM; n++){//Q(St+1,at+1)の実装
				if (q_num[next_player_point_y][next_player_point_x][q_max] < q_num[next_player_point_y][next_player_point_x][n])
					q_max = n;
				//this?
			}

			double ee = updateE(move, now_player_point_x, now_player_point_y, move_direction);

			/*Q値計算*/
			q_num[now_player_point_y][now_player_point_x][move_direction] =
				q_num[now_player_point_y][now_player_point_x][move_direction] + ee*ALPHA*(reward + GAMMA*q_num[next_player_point_y][next_player_point_x][q_max]
				- q_num[now_player_point_y][now_player_point_x][move_direction]);

			//q_num[now_player_point_y][now_player_point_x][move_direction] += ee;

			/*s<-s',a<-a'*/
			now_player_point_x += vector_x;
			now_player_point_y += vector_y;
			move_direction = q_max;

			/*sが終端なら終了*/
			int se = SM->getSessionState();
			if (se == SESSIONSTATE_DROPDAWN_HOLE){
				this->now_player_point_x = this->default_start_point_x;
				this->now_player_point_y = this->default_start_point_y;
				/*イプシロングリーディー法*/
				move_direction = Cal.epsilonGreedy(EPSILON, DIRECTION_NUM, q_num, now_player_point_x, now_player_point_y);

			}
			else
			if (se == SESSIONSTATE_GOAL){
				SM->setMinSucess(move);
				SM->setFirstSuccess(scenenum);
				SM->countSuccess();
				OFS.wrtiteFile(scenenum, 1, move, Cordinate_List, SM->getStoryRewards());
				break;
			}

		}
		if (scenenum % 10000 == 0){
			OFS.writeQ(q_num, map_size_x, map_size_y, scenenum);
		}
		Cordinate_List.clear();
		//printf("\n");
	}

	printf("\n\n First Success : %d",SM->getFirstSuccess());
	printf("\n SuccessNum : %d / %d\n", SM->getSuccessNum(), MAX_SCENE_TRIALNUM);

	this->OFS.terminateOFS(MAX_SCENE_TRIALNUM, SM->getSuccessNum(), SM->getFirstSuccess(), SM->getMinSccess());
	printf("\n-----END------");
}

void Scene::setScene(char *c){

	this->readMap(c);

	for (int y = 0; y < map_size_y; y++){
		for (int x = 0; x < map_size_x; x++){
			if (map[y][x] == MCA_PLAYER_START){
				this->default_start_point_x = x; this->default_start_point_y = y;
			}else
			if (map[y][x] == MCA_PLAYER_GOAL){
				this->goal_point_x = x; this->goal_point_y = y;
			}
		}
	}

	//Q値格納変数の動的確保
	this->q_num = new double**[map_size_y];
	for (int y = 0; y < map_size_y; y++){
		q_num[y] = new double*[map_size_x];
		for (int x = 0; x < map_size_x; x++){
			q_num[y][x] = new double[DIRECTION_NUM];
		}
	}

	this->e_num = new double**[map_size_y];
	for (int y = 0; y < map_size_y; y++){
		e_num[y] = new double*[map_size_x];
		for (int x = 0; x < map_size_x; x++){
			e_num[y][x] = new double[DIRECTION_NUM];
		}
	}

	this->initializeQNum();
	this->initializeENum();
}

void Scene::initializeQNum(){

	for (int y = 0; y < map_size_y;y++)
	for (int x = 0; x < map_size_x;x++)
	for (int d = 0; d < DIRECTION_NUM; d++)
		q_num[y][x][d] = 0;//rand() % (INITIALIZE_Q_MAX + 1);
	return;
}

void Scene::initializeENum(){
	for (int y = 0; y < map_size_y; y++)
	for (int x = 0; x < map_size_x; x++)
	for (int d = 0; d < DIRECTION_NUM; d++)
	e_num[y][x][d] = 0;//rand() % (INITIALIZE_Q_MAX + 1);
	return;
}

double Scene::updateE(int time,int x, int y,int d){
	if (time == 0)return 0.0;
	double result = 0.0;

	if (time == 1)result = 1.0*Lamda*GAMMA+1;
	else
		result = Lamda*GAMMA*pow(e_num[y][x][d], time - 1)+1;

	return result;
}

void Scene::readMap(char *c){
	char c_[1000];
	FILE *f;
	if ((f = fopen(c, "r")) == NULL){
		exit(1);
	}
	/*マップの縦横のマス数を取得*/
	fgets(c_, 5, f);
	map_size_x = atoi(c_);
	fgets(c_, 5, f);
	map_size_y = atoi(c_);
	/*動的に二次元配列を確保*/
	map = (int**)malloc(sizeof(int)* map_size_y);
	for (int n = 0; n < map_size_y; n++)map[n] = (int *)malloc(sizeof(int)* map_size_x);
	/*各マスに割り振られた数値を取得*/
	char *ss = " ,";
	char *tok;
	for (int y = 0; y < map_size_y; y++){
		fgets(c_, 1000, f);
		tok = strtok(c_, ss);
		for (int x = 0; x < map_size_x; x++){
			map[y][x] = atoi(tok);
			tok = strtok(NULL, ss);
			if (tok == NULL)
				break;
		}
	}
	/*マップの縦横の長さを格納*/
	fclose(f);
	//this->DecimatingLine();//LINEの間引き
	return;
}

void Scene::initialize(){
	this->now_movement_num = 0;
	this->now_player_point_x = this->default_start_point_x;
	this->now_player_point_y = this->default_start_point_y;
}

void Scene::setConstant(){

	CFS.setAXSS("config.txt");

	for (int n = 0; n < CFS.getLineNum();n++){
		int i = CFS.getFunction(n);
		if (i == SET_EPSILON)
			EPSILON = atof(CFS.getPartOfLine(n,1));
		if (i == SET_ALPHA)
			ALPHA = atof(CFS.getPartOfLine(n, 1));
		if (i == SET_GAMMA)
			GAMMA = atof(CFS.getPartOfLine(n, 1));
		if (i == SET_INITIALIZE_Q_MAX)
			INITIALIZE_Q_MAX = atoi(CFS.getPartOfLine(n,1));
		if (i == SET_MAX_MOVEMENTS_NUM)
			MAX_NUMBER_OF_MOVEMENTS = atoi(CFS.getPartOfLine(n,1));
		if (i == SET_MAX_SCENE_TRIALNUM)
			MAX_SCENE_TRIALNUM = atoi(CFS.getPartOfLine(n,1));
	}

	return;
}