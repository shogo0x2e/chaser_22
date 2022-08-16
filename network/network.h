//*********************************************
//               connection.h
//
//        2019年12月22日 北田 章伍 最終更新
//      サーバとクライアント間の接続処理に関する宣言
//
//     備考: CHaserOnlineのサンプルプログラムより
//*********************************************

#include "../others/std_headers.h"

// 学校から接続する場合は以下のプロキシを設定
#ifdef _IN_SCHOOL_
#define PROXY "proxy.spec.ed.jp"
#else
#define PROXY ""
#endif

#define KEY_GETREADY "GetReadyCheck"
#define KEY_ACTION   "CommandCheck"
#define KEY_END      "EndCommandCheck"

typedef enum  send_mode_ {
    SendGetready,
    SendAction,
    SendTurnEnd
} send_mode;

int  retDataCount();
void copyRawgr(int copyTo[]);
void establishConnection(int argc, char* argv[], char proxyAddress[]);
void sendCommand(send_mode smode, ecmd cmd);

void expSendCommand(const char mode[], const char cmd[]);