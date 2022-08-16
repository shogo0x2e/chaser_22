//**************************************************
//                numeric_defines.h
//
//        2019年1月3日 北田 章伍 最終更新
//     ディレクティブによる処理に関する各種数値の定義
//**************************************************

/*--------------- 処理系 --------------*/

// 関数動作の正常・異常を判断する際に使用
#define SUCCESS  -1
#define FAILURE  -2

#define MAP_SIZE        61          // 構造体 map_datas における一列の要素数
#define GR_ARR_SIZE     9           // GetReady に用いる配列の要素数
#define MAX_CMD_LEN     10          // サーバに送信する最長の String 型コマンド
#define SENDCMD_MAXLEN  20          // サーバに送信する 「commandN=」を含めた最長コマンド
                                    // 必要数 17 「commandN=pru0wld」

/*---------- キャラクタ・動作系 ----------*/

//*************************************
//     enumによるキャラクタの定義
//
//  備考: 昇順に整列する意味は特にない
//*************************************

typedef enum map_chara_ {
    outside         = -30000,
    white           = -29000,
    unknown         = -28000,   // 4 個の空きに意味はない
    warp            = -24000,
    warp10_up,
    warp10_right,
    warp10_down,
    warp10_left,
    warp5_up,      // -24005
    warp5_right,
    warp5_down,
    warp5_left,
    black_lu,                   // black系の末尾2文字はマップ中央からみた位置を示す
    black_ru,      // -24010
    black_ld,
    black_rd,
    dirt            = -21000,
    hole            = -20000,
    enemy           = -15000,
    bedrock         = -5000,
    myself          = -1000,
    item            =  10000,
    target          =  50000,
    trilobite       =  50001,
    fossil          =  150000,
} map_chara;

//*********************************************
//          enumによるcommandの定義
//
//  備考: 時計回りに整列してるけど特に意味はない
//     checks.c の *commands[][]　と順番が
//             対応してればそれでOK
//*********************************************

typedef int  ecmd;  //enum 型の command が対象であることを明示

typedef enum get_ready_  {      // 指定方向に移動し、自機を中心として周囲情報を取得
    gr_get,                     // 周囲情報を取得するだけ
    gr_get_up,
    gr_get_right,
    gr_get_down,
    gr_get_left
} get_ready;

typedef enum act_walk_ {       // 指定方向に1マス移動
    walk_up = 10,
    walk_right,
    walk_down,
    walk_left
} act_walk;

typedef enum act_search_ {
    put_search_up = 20,     //アイテムを上に置き、右に1マス移動し、上の9マス(一直線)の情報を取得
    put_search_right,       //アイテムを右に置き、下に1マス移動し、右の9マス(一直線)の情報を取得
    put_search_down,        //アイテムを下に置き、左に1マス移動し、下の9マス(一直線)の情報を取得
    put_search_left         //アイテムを左に置き、上に1マス移動し、左の9マス(一直線)の情報を取得
} act_search;

typedef enum act_look_ {
    put_look_up = 30,		//アイテムを上に置き、右に1マス移動し、上の9マスの情報を取得
    put_look_right,		    //アイテムを右に置き、下に1マス移動し、右の9マスの情報を取得
    put_look_down,		    //アイテムを下に置き、右に1マス移動し、下の9マスの情報を取得
    put_look_left		    //アイテムを左に置き、上に1マス移動し、左の9マスの情報を取得
} act_look;

typedef enum act_putwalk_ {     // 指定方向に土を置き、反対へ移動
    put_walk_up = 40,
    put_walk_right,
    put_walk_down,
    put_walk_left,
    put_walk_right_up,
    put_walk_right_down,
    put_walk_left_down,
    put_walk_left_up
} act_putwalk;

typedef enum act_keima_ {       // 指定方向へ桂馬の動きをする
    keima_right_up = 50,
    keima_right_down,
    keima_left_down,
    keima_left_up
} act_keima;

typedef enum act_break_ {       // 指定方向にある土を破壊
    break_up = 60,
    break_right,
    break_down,
    break_left
} act_break;

typedef enum act_break_walk_ {  // 指定方向にあるキャラクタを破壊し、反対へ逃げる
    break_walk_up = 70,
    break_walk_right,
    break_walk_down,
    break_walk_left,
    break_walk_right_up,
    break_walk_left_up,
    break_walk_right_down,
    break_walk_left_down
} act_break_walk;

typedef enum act_dig_ {         // 指定方向に動いて自機の上9マスの情報を取得
    dig_up = 80,
    dig_right,
    dig_down,
    dig_left,
    dig_right_up,
    dig_right_down,
    dig_left_down,
    dig_left_up
} act_dig;
