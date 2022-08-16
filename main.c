//起動パラメータの書式
//	./CHaserOnlineClient009-2Proxy.o ターゲットurl [-x プロキシアドレス:プロキシポート -u ユーザID -p パスワード -r ルーム番号]
//	パラメータの順番、有無は任意でよい

#include "network.h"

//*******************************************************************
//							main 関数
//*******************************************************************

int main(int argc, char *argv[]){

	int  i = 0;
	int  getReady[GR_ARR_SIZE];

    establishConnection(argc, argv, "");

	while ( 1 ) {

		/****** GetReadyの発行 ******/
		sendCommand(KEY_GETREADY, "gr");

		/******  Actionの発行  ******/
		sendCommand(KEY_ACTION, "wu");

		/******   ターン終了   ******/
		sendCommand(KEY_END, "#");
	}

    return 0;
}