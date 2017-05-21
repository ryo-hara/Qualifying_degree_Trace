#include "OutputFileSystem.h"
#include <time.h>
#include <String>

void OutputFileSystem::makeOFSFile(){

	time_t mt;
	struct tm *t;
	time(&mt);
	t = localtime(&mt);
	char str[100];
	sprintf(str, "%d", t->tm_mon+1);
	strcpy(address, str);
	sprintf(str, "%d", t->tm_mday);
	strcat(address, str);
	sprintf(str, "%d", t->tm_hour);
	strcat(address, str);
	sprintf(str, "%d", t->tm_min);
	strcat(address, str);
	sprintf(str, "%d", t->tm_sec);
	strcat(address, str);
	strcat(address,".txt");

	if ((f = fopen(address, "w+")) == NULL){
		exit(1);
	}
	return;
}

void OutputFileSystem::wrtiteFile(int num,int successflag, int movements, std::list<Cordinate*> L , double one_story_Q){
	if (f == NULL)exit(0);
	fprintf(f, "Num,%d,Success,%d,movements,%d,Q,%f", num, successflag, movements, one_story_Q);
	for (std::list<Cordinate*>::iterator itr = L.begin(); itr != L.end(); itr++){
		//fprintf(f,"%d-%d,",(*itr)->x,(*itr)->y);
	}
	fprintf(f,"\n");
}


void OutputFileSystem::terminateOFS(int SceneNum, int successNum, int firstSuccessNum, int MinNum){//terminate関数、処理を終わらせる。←引数部にリザルト数値追加なー

	fprintf(f,"Success: %d / %d ,,FirstSccess:,%d,,MinSccess:,%d",successNum,SceneNum,firstSuccessNum,MinNum);

	fclose(f);
}

void OutputFileSystem::writeQ(double ***q, int maxx, int maxy, int nowcount){
	char ad[200];
	strcpy(ad, "");
	strcat(ad, "Q_num\\");
	char add[100];
	sprintf(add, "%d", nowcount);
	strcat(ad,add);
	strcat(ad, ".csv");

	FILE *f_1;
	if ((f_1 = fopen(ad, "w+")) == NULL){
		exit(1);
	}

	for (int y = 0; y < maxy; y++){
		for (int x = 0; x < maxx; x++){
			for (int d = 0; d < 4; d++ ){
				fprintf(f_1, "%f,", q[y][x][d]);
			}
			fprintf(f_1,",");
		}
		fprintf(f_1, "\n");
	}


	fclose(f_1);

}