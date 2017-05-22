#include "Scene.h"

Scene::Scene(){
	srand((unsigned)time(NULL));
	this->setConstant();//config�n�萔�n�̓ǂݍ���
	this->initializeQNum();
	this->map_size_x = 0;
	this->map_size_y = 0;
	this->now_movement_num = 0;
}

void Scene::draw(){

	/*�}�b�v�`�敔*/
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
		//List�R���e�i�Ɍ��݂̍��W���i�[
		cordinate = new Cordinate;
		cordinate->x = now_player_point_x; cordinate->y = now_player_point_y; cordinate->q = 0.0f;
		Cordinate_List.push_back(cordinate);
		SM->initializeStoryWards();

		/*�C�v�V�����O���[�f�B�[�@*/
		int move_direction = Cal.epsilonGreedy(EPSILON,DIRECTION_NUM,q_num,now_player_point_x,now_player_point_y);


		for (int move=0; move < 1000;move++){
			/*�s��a���s�������r��S'���m�F*/
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
						//r�l�m��
			if (map[next_player_point_y][next_player_point_x] == MCA_WALL){			//�ړ��悪�ǂ̏ꍇ
				vector_x = 0; vector_y = 0;
				next_player_point_x = now_player_point_x;				
				next_player_point_y = now_player_point_y;
				reward += PENALTY_HIT_WALL;
				SM->setSessionState(SESSIONSTATE_HIT_WALL);
			}
			else
			if (map[next_player_point_y][next_player_point_x] == MCA_HOLE){			//�ړ��悪���̏ꍇ
				reward += PENALTY_DROPDAWN_HOLE;
				this->next_player_point_x = this->default_start_point_x;//
				this->next_player_point_y = this->default_start_point_y;//
				SM->setSessionState(SESSIONSTATE_DROPDAWN_HOLE);
			}
			else
			if (map[next_player_point_y][next_player_point_x] == MCA_PLAYER_GOAL){	//�ړ��悪�S�[���̏ꍇ
				reward += REWARDS_GOAL;
				SM->setSessionState(SESSIONSTATE_GOAL);
			}
			else{																	//����ȊO�̏ꍇ
				SM->setSessionState(SESSIONSTATE_NORMAL);
			}
			//�S�[���ȊO�Ȃ�}�C�i�X
			if (SM->getSessionState() != SESSIONSTATE_GOAL)reward += PENALTY_ONE_STEP;
			SM->addStoryRewards(reward);



			/*Qs',a����ɍs��a'��I���B*/
			int q_max = rand()%4;
			for (int n = 0; n < DIRECTION_NUM; n++){//Q(St+1,at+1)�̎���
				if (q_num[next_player_point_y][next_player_point_x][q_max] < q_num[next_player_point_y][next_player_point_x][n])
					q_max = n;
				//this?
			}

			double ee = updateE(move, now_player_point_x, now_player_point_y, move_direction);

			/*Q�l�v�Z*/
			q_num[now_player_point_y][now_player_point_x][move_direction] =
				q_num[now_player_point_y][now_player_point_x][move_direction] + ee*ALPHA*(reward + GAMMA*q_num[next_player_point_y][next_player_point_x][q_max]
				- q_num[now_player_point_y][now_player_point_x][move_direction]);

			//q_num[now_player_point_y][now_player_point_x][move_direction] += ee;

			/*s<-s',a<-a'*/
			now_player_point_x += vector_x;
			now_player_point_y += vector_y;
			move_direction = q_max;

			/*s���I�[�Ȃ�I��*/
			int se = SM->getSessionState();
			if (se == SESSIONSTATE_DROPDAWN_HOLE){
				this->now_player_point_x = this->default_start_point_x;
				this->now_player_point_y = this->default_start_point_y;
				/*�C�v�V�����O���[�f�B�[�@*/
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

	//Q�l�i�[�ϐ��̓��I�m��
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
	/*�}�b�v�̏c���̃}�X�����擾*/
	fgets(c_, 5, f);
	map_size_x = atoi(c_);
	fgets(c_, 5, f);
	map_size_y = atoi(c_);
	/*���I�ɓ񎟌��z����m��*/
	map = (int**)malloc(sizeof(int)* map_size_y);
	for (int n = 0; n < map_size_y; n++)map[n] = (int *)malloc(sizeof(int)* map_size_x);
	/*�e�}�X�Ɋ���U��ꂽ���l���擾*/
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
	/*�}�b�v�̏c���̒������i�[*/
	fclose(f);
	//this->DecimatingLine();//LINE�̊Ԉ���
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