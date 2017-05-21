#pragma once
#include "Common.h"

//リアルタイム処理できないけどどうしよ？
//そらもうアレよ、Listコンテナ引数にぶっ込むっしょ
//わあ…
//OutPutしたら用済みである。コンテナはdeleteしとけ
//OutPutしなかった場合のコンテナは？
//…

//これな、Update関数の最初にファイル作ってループ内で適宜書きこめば良くないか？
//結果とか最後に書くことになんべ？
//まあ…それは仕方ないと割り切るべきで、どうせ自分しか使わん。

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