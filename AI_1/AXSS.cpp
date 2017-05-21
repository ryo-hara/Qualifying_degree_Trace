#include "AXSS.h"



void FileLeader::setAddress(char * a){
	strcpy(this->address,a);
}

void FileLeader::readFile(){
	if ((f = fopen(address, "rb")) == NULL){
		exit(1);
	}
	char value;
	int z = 0;
	binary_num = 0;
	while ((value = getc(f)) != EOF) {
		binaryData[z] = value;
		z++;
	}
	binary_num = z;
	fclose(this->f);
	return;
}

void FileLeader::changeBinaryToString(){
	char c = '\0';
	char b = ';';
	char d = ' ';
	char e = '\n';
	char f = '\r';
	char g = '	';
	int x = 0;
	int string_x = 0;
	int string_y = 0;
	while (binaryData[x] != '\0'){
		if (memchr(&binaryData[x], b, sizeof(char)) != NULL){
			this->getString[string_y][string_x] = '\0';
			string_x = 0;
			string_y++;
		}else
		if (memchr(&binaryData[x], d, sizeof(char)) != NULL || memchr(&binaryData[x], e, sizeof(char)) != NULL || memchr(&binaryData[x], f, sizeof(char)) != NULL || memchr(&binaryData[x], g, sizeof(char)) != NULL){
		}else{
			this->getString[string_y][string_x] = this->binaryData[x];
			string_x++;
		}
		x++;
		if (binary_num <= x)break;
	}
	Linenum = string_y+1;
	return;
}


int AXSS::checkLineNum(int n){
	if (n > getLineNum())return NULL;
	
	return 1;
}


char* AXSS::getPartOfLine(int linenum, int argument){
	char *tok = new char[Str[linenum][argument].length() + 1];
	memcpy(tok, Str[linenum][argument].c_str(), Str[linenum][argument].length() + 1);
	return tok;
}

int AXSS::setAXSS(char *a){
	setAddress(a);
	readFile();
	changeBinaryToString();
	setStrToArgument();
	return 0;
}


int AXSS::getFunction(int linenum){
	if (this->checkLineNum(linenum)!= 1)return -1;

	if (strcmp(this->getPartOfLine(linenum, 0), CFS_EPSILON) == 0){
		return SET_EPSILON;
	}else
	if (strcmp(this->getPartOfLine(linenum, 0), CFS_ALPHA) == 0){
		return SET_ALPHA;
	}else
	if (strcmp(this->getPartOfLine(linenum, 0), CFS_GAMMA) == 0){
		return SET_GAMMA;
	}else
	if (strcmp(this->getPartOfLine(linenum, 0), CFS_INITIALIZE_Q_MAX) == 0){
		return SET_INITIALIZE_Q_MAX;
	}else
	if (strcmp(this->getPartOfLine(linenum, 0), CFS_MAX_MOVEMENTS_NUM) == 0){
		return SET_MAX_MOVEMENTS_NUM;
	}else
	if (strcmp(this->getPartOfLine(linenum, 0), CFS_MAX_SCENE_TRIALNUM) == 0){
		return SET_MAX_SCENE_TRIALNUM;
	}else
	if (strcmp(this->getPartOfLine(linenum, 0), CFS_FILE_OUTPUT_FLAG) == 0){
		return SET_OUTPUT_FLAG;
	}else
	if (strcmp(this->getPartOfLine(linenum, 0), CFS_END) == 0){
	}
	return -1;
}


void AXSS::setStrToArgument(){
	const char *ss = ",;";
	char *tok = NULL;
	for (int n = 0; n <= getLineNum(); n++){
		char *getarg = this->getStr(n);
		for (int i = 0; i < ARGUMENT_NUM; i++){
			if (i == 0){
				tok = strtok(getarg, ss);
			}
			else{
				tok = strtok(NULL, ss);
			}
			if (tok == NULL)tok = " \0";
			this->Str[n][i] = tok;
		}
	}
}