#pragma once
#include "Common.h"

//���A���^�C�������ł��Ȃ����ǂǂ�����H
//��������A����AList�R���e�i�����ɂԂ����ނ�����
//�킠�c
//OutPut������p�ς݂ł���B�R���e�i��delete���Ƃ�
//OutPut���Ȃ������ꍇ�̃R���e�i�́H
//�c

//����ȁAUpdate�֐��̍ŏ��Ƀt�@�C������ă��[�v���œK�X�������߂Ηǂ��Ȃ����H
//���ʂƂ��Ō�ɏ������ƂɂȂ�ׁH
//�܂��c����͎d���Ȃ��Ɗ���؂�ׂ��ŁA�ǂ������������g���B

class OutputFileSystem{
private:
	char address[100];
	FILE *f;
public:

	OutputFileSystem(){ f = NULL; }

	void makeOFSFile();

	void wrtiteFile(int num, int successflag, int movements, std::list<Cordinate*> L, double one_story_Q);
	void terminateOFS(int SceneNum,int successNum,int firstSuccessNum,int MinNum);

	void writeQ(double ***q,int maxx,int maxy,int nowcount);

};