//*********************************************
//               connection.h
//
//        2019年12月22日 北田 章伍 最終更新
//      サーバとクライアント間の接続処理に関する宣言
//
//     備考: CHaserOnlineのサンプルプログラムより
//*********************************************

// 関数動作の正常・異常を判断する際に使用
#define SUCCESS  -1
#define FAILURE  -2

#define MAP_SIZE        61          // 構造体 map_datas における一列の要素数
#define GR_ARR_SIZE     9           // GetReady に用いる配列の要素数
#define MAX_CMD_LEN     10          // サーバに送信する最長の String 型コマンド
#define SENDCMD_MAXLEN  20          // サーバに送信する 「commandN=」を含めた最長コマンド
                                    // 必要数 17 「commandN=pru0wld」


// sendCommand に使用するキーワード
#define KEY_GETREADY "GetReadyCheck"
#define KEY_ACTION   "CommandCheck"
#define KEY_END      "EndCommandCheck"

/**
 * @brief ReturnNumber に含まれる数値の数を取得する
 * 
 * @return int ReturnNumber に含まれる数値の数
 */
int  retDataCount();

/**
 * @brief 取得した周囲情報 (GR情報) を取得する
 * 
 * @param copyTo 保存先の配列
 */
void copyRawgr(int copyTo[]);

/**
 * @brief サーバとの接続を確立する
 * 
 * @param argc main() の argc
 * @param argv main() の argv 
 * @param proxyAddress プロキシ (学校からだったら "proxy.spec.ed.jp")
 */
void establishConnection(int argc, char* argv[], char proxyAddress[]);

/**
 * @brief コマンドをサーバに送信する
 * 
 * @param mode KEY_GETREADY, KEY_ACTION, KEY_END のいずれか
 * @param cmd 送信するコマンド (gr, wu など)
 */
void sendCommand(const char mode[], const char cmd[]);