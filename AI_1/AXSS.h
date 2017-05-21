#pragma once
#include "Common.h"

#define MAX_STRINGLINE_NUM 400
#define MAX_ONELINE_STRING_NUM 200
#define MAX_BINARY MAX_STRINGLINE_NUM*MAX_ONELINE_STRING_NUM*2

#define ARGUMENT_NUM 5//���ߕ��{�������̐�

using namespace std;

#define CFS_EPSILON "@EPSILON"
#define CFS_ALPHA	"@ALPHA"
#define CFS_GAMMA	"@GAMMA"
#define	CFS_INITIALIZE_Q_MAX "@INITIALIZE_Q_MAX"
#define CFS_MAX_MOVEMENTS_NUM "@MAX_MOVEMENTS_NUM"
#define CFS_MAX_SCENE_TRIALNUM "@MAX_SCENE_TRIALNUM"
#define CFS_FILE_OUTPUT_FLAG   "@FILE_OUTPUT_FLAG"
#define CFS_END				   "@END"

enum ORDER_SET{
	SET_EPSILON,
	SET_ALPHA,
	SET_GAMMA,
	SET_INITIALIZE_Q_MAX,
	SET_MAX_MOVEMENTS_NUM,
	SET_MAX_SCENE_TRIALNUM,
	SET_OUTPUT_FLAG,
	SET_END,
};


class FileLeader{
private:
	int binary_num;
	char binaryData[MAX_BINARY];
	char getString[MAX_STRINGLINE_NUM][MAX_ONELINE_STRING_NUM];
	char address[100];
	int Linenum;
	FILE *f;
public:

	void setAddress(char *a);
	void readFile();
	void changeBinaryToString();
	void print(){
		printf("%s\n", getString[0]);
	}

	char *getStr(int n){ return this->getString[n]; };
	int getLineNum(){ return this->Linenum; }
};


class AXSS : public FileLeader{
private:
	int order;					//���ߊi�[�ϐ�
	int argument[ARGUMENT_NUM];	//�����i�[�ϐ�
	std::string Str[MAX_STRINGLINE_NUM][ARGUMENT_NUM];


	void setStrToArgument();

public:
	AXSS(){};
	~AXSS(){};

	int setAXSS(char *a);
	char* getPartOfLine(int linenum,int argument);//���̍s�̈ꕔ��؂���//argument��0�Ŗ��ߕ��A1�ő������A2�ő��������w�肷��

	int getFunction(int linenum);

	int checkLineNum(int n);			//���̍s�����݂��邩�ǂ������m���߂�֐�
};