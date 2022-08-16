//起動パラメータの書式
//	./CHaserOnlineClient009-2Proxy.o ターゲットurl [-x プロキシアドレス:プロキシポート -u ユーザID -p パスワード -r ルーム番号]
//	パラメータの順番、有無は任意でよい

#include "others/std_headers.h"

// 学校から接続する場合は下の define を有効化
// #define  _IN_SCHOOL_

#include "network/network.h"

//*******************************************************************
//							main 関数
//*******************************************************************


int main(int argc, char *argv[]){

	int  i = 0;
	int  getReady[GR_ARR_SIZE];

    establishConnection(argc, argv, PROXY);

	while ( 1 ) {

		/****** GetReadyの発行 ******/
		expSendCommand(KEY_GETREADY, "command1=gr");
		//sendCommand(SendGetready, gr_get);

		/******  Actionの発行  ******/
		expSendCommand(KEY_ACTION, "command2=wu");
		// sendCommand(SendAction, walk_up);

		/******   ターン終了   ******/
		expSendCommand(KEY_END, "command3=#");

		//「 0 」の部分は意味なし, SendTurnEnd 処理時に第二引数は使用しない
		// sendCommand(SendTurnEnd, 0);
	}

    return 0;
}