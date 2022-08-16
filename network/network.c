//*******************************************************************
//                				connection.c
//
//        			2019年12月22日 北田 章伍 最終更新
//      		サーバとクライアント間の接続処理に関する定義
//
//    			 備考: CHaserOnlineのサンプルプログラムより
//*******************************************************************

#include "network.h"

#define BUF_LEN     512                  /* バッファのサイズ */
#define MAX_KEYWORD	30					 /*キーワードの最大数*/

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                            変数定義
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct sockaddr_in server;           /* ソケットを扱うための構造体 */

char host[BUF_LEN] = "localhost";    /* 接続するホスト名 */
char path[BUF_LEN] = "/";            /* 要求するパス */
unsigned short port = 0;             /* 接続するポート番号 */

char Param[BUF_LEN];                //サーバーへ送るパラメータを格納
char ReturnCode[BUF_LEN];           //サーバーからの返り値を格納
int  ReturnNumber[GR_ARR_SIZE];		//サーバーからの返り値 int バージョン

int  ReturnCount;					// サーバからの返り値の個数

// 要素数指定しないとコンパイルできない
const char *commands[9][MAX_CMD_LEN] = {
	//get_ready      (GetReadyMove)
	{ "gr", "gru", "grr", "grd", "grl" },
	//act_walk       (walk系)
	{ "wu", "wr", "wd", "wl" },
	//act_search     (put & search系)
	{ "pu3su", "pr3sr", "pd3sd", "pl3sl" },
	//act_look       (put & look)
	{ "pu3lu", "pr3lr", "pd3ld", "pl3ll" },
	//act_putwalk    (put & walk)
	{ "pu2wd", "pr2wl", "pd2wu", "pl2wr", "pru2wld", "prd2wlu", "pld2wru", "plu2wrd" },
	//act_keima      (kei系)
	{ "keiru", "keird", "keild", "keilu" },
	//act_break      (put系)
	{ "pu0", "pr0", "pd0", "pl0" },
	//act_break_walk (砕く動作)
	{ "pu0wd", "pr0wl", "pd0wu", "pl0wr", "pru0wld", "prd0wlu", "pld0wru", "prd0wlu" },
	//act_dig        (dig系)
	{ "du", "dr", "dd", "dl", "dru", "drd", "dlu", "dld" }
};


//-------------------------------------------------------------------
//					static な関数のプロトタイプ宣言
//-------------------------------------------------------------------

static int   Init(int argc, char *argv[], char ProxyAddress[], int ProxyPort);
static int   send_cmd(const char[], char[]);
static int   ReturnCode_to_ReturnNumber();
static char* enum2strcmd(ecmd cmd);


//*******************************************************************
//                             関数定義
//*******************************************************************

int  retDataCount() {
	return ReturnCount;
}

void copyRawgr(int copyTo[]) {
	int i = 0;

	for (i = 0; i < GR_ARR_SIZE; i++) {
		copyTo[i] = ReturnNumber[i];
	}
}

void establishConnection(int argc, char *argv[], char cmp_ProxyAddress[]) {

    int  i;
	int	 RoomNumber = -1;
	int  ProxyPort;
	char UserName[20];
	char PassWord[20];
    char *pivo;
	char ProxyAddress[256];
	char buff[10];

	strcpy(ProxyAddress, cmp_ProxyAddress);

	/*-----------------------
	 パラメータの読み込み
	 * 2013.4.3
	 ------------------------*/
	i = 2;
	while(argv[i] != NULL){
		if(argv[i][0] == '-'){	//パラメータ指定子だったら
			switch(argv[i][1]){
				case	'x':	//プロキシアドレス:プロキシポート
					i++;
					pivo = strchr(argv[i], ':');	//アドレスとポートを切り離す
					*pivo = '\0';
					strcpy(ProxyAddress, argv[i]);
					ProxyPort = atoi((char *)(++pivo));
					break;

				case	'u':	//ユーザID
					i++;
					strcpy(UserName, argv[i]);
					break;

				case	'p':	//パスワード
					i++;
					strcpy(PassWord, argv[i]);
					break;

				case	'r':	//ルーム番号
					i++;
					RoomNumber = atoi(argv[i]);
					break;

				default:
					break;
			}
		}

		i++;
	}

	/*-----------------------
		ネットワーク接続
	-------------------------*/
	if(Init(argc, argv, ProxyAddress, ProxyPort)!=0){
		exit(EXIT_FAILURE);
	}

	/*-----------------------
	ユーザ名とパスワードをセットする
	-------------------------*/
	do{
		if(strcmp(UserName, "")==0){	//ユーザ名パラメータがなければ
			printf("UserName=");
			scanf("%s",UserName);
		}
		strcpy(Param, "user=");
		strcat(Param, UserName);
		if(strcmp(PassWord, "")==0){	//パスワードパラメータがなければ
			printf("PassWord=");
			scanf("%s",PassWord);
		}
		strcat(Param, "&pass=");
		strcat(Param, PassWord);
		send_cmd("UserCheck", Param);


		if(strcmp(ReturnCode, "roomNumber=")==0){		//ユーザ名が受け付けられたら
		}
		else{
			//強制終了
			printf("User CheckNG\n");
			exit(EXIT_FAILURE);
		}
	}while(strcmp(ReturnCode, "roomNumber=")!=0);		//ユーザ名が受け付けられるまでループ

	/*--------------------
	ルーム番号を指定する
	--------------------*/
	do{
		if(RoomNumber < 0){	//ルーム番号パラメータがなければ
			printf("RoomNumber=");
			scanf("%d",&RoomNumber);
		}
		printf("RoomNumber=%d\n", RoomNumber);
		strcpy(Param, "roomNumber=");
		sprintf(buff, "%d", RoomNumber);
		strcat(Param, buff);
		send_cmd("RoomNumberCheck", Param);

		if(strcmp(ReturnCode, "command1=")==0){		//ルーム番号が受け付けられたら
		}
		else{
			//強制終了
			printf("RoomCheckNG\n");
			exit(EXIT_FAILURE);
		}
	}while(strcmp(ReturnCode, "command1=")!=0);	//ルーム番号が受け付けられるまでループ
	
	printf("\n\n-----------------------\n");
	printf("  Connected to server\n");			//なぜかこれないと進まない
	printf("-----------------------\n\n");
}

void expSendCommand(const char mode[], const char cmd[]) {

	// 同じコマンドを繰り返し送った回数
	int repeat = 0;
	// commandX=YY の X の数値
	int key_num;
	// サーバ文字列のバッファ
	char buf_cmd[MAX_KEYWORD];

	if      (strcmp(mode, KEY_GETREADY) == 0) key_num = 1;
	else if (strcmp(mode, KEY_ACTION)   == 0) key_num = 2;
	else if (strcmp(mode, KEY_END)      == 0) key_num = 3;

	if (strcmp(mode, KEY_END) != 0) {

		sprintf(buf_cmd, "command%d=%s", key_num, cmd);

		do {
			send_cmd(mode, buf_cmd);
		} while (strchr(ReturnCode, ',') == NULL && strcmp(ReturnCode, "user=") != 0);
	}
	else {

		do {
			// server_keyword[smode] は "EndCommandCheck" にも置き換えられる
			send_cmd(mode, "command3=%23");

			if (strcmp(ReturnCode, "user=") == 0 || repeat++ > 5) {
				exit(EXIT_SUCCESS); //ゲーム終了
			}

		} while (strcmp(ReturnCode, "command1=") != 0 && strcmp(ReturnCode, "user=") != 0);
	}

} 
//-------------------------------------------------------------------
//							static な関数
//-------------------------------------------------------------------

static int  Init(int argc, char *argv[], char ProxyAddress[], int ProxyPort) {
    struct hostent *servhost;            /* ホスト名と IP アドレスを扱うための構造体 */
    struct servent *service;             /* サービス (http など) を扱うための構造体 */

	if ( argc > 1 ){                     /* URLが指定されていたら */
		char host_path[BUF_LEN];

		if ( strlen(argv[1]) > BUF_LEN-1 ){
            fprintf(stderr, "URL が長すぎます。\n");
            return 1;
        }
                                         /* http:// から始まる文字列で */
                                         /* sscanf が成功して */
                                         /* http:// の後に何か文字列が存在するなら */
        if ( strstr(argv[1], "http://") &&
             sscanf(argv[1], "http://%s", host_path) &&
             strcmp(argv[1], "http://" ) ){

            char *p;
            p = strchr(host_path, '/');  /* ホストとパスの区切り "/" を調べる */
            if ( p != NULL ){
                strcpy(path, p);        /* "/"以降の文字列を path にコピー */
                *p = '\0';
                strcpy(host, host_path); /* "/"より前の文字列を host にコピー */
            } else {                     /* "/"がないなら＝http://host という引数なら */
                strcpy(host, host_path); /* 文字列全体を host にコピー */
            }

            p = strchr(host, ':');       /* ホスト名の部分に ":" が含まれていたら */
            if ( p != NULL ){
                port = atoi(p+1);        /* ポート番号を取得 */
                if ( port <= 0 ){        /* 数字でない (atoi が失敗) か、0 だったら */
                    port = 80;         /* ポート番号は 80 に決め打ち */
                }
                *p = '\0';
            }
        } else {
            fprintf(stderr, "URL は http://host/path の形式で指定してください。\n");
            return 1;
        }
    }

    printf("\nhttp://%s%s の取得... ", host, path);

	/* ホストの情報(IPアドレスなど)を取得 */
	if(strcmp(ProxyAddress, "") == 0){
		servhost = gethostbyname(host);
		if ( servhost == NULL ){
			fprintf(stderr, "\n[%s] から IP アドレスへの変換に失敗しました。\n", host);
			return 0;
		}
	}
	else{
		servhost = gethostbyname(ProxyAddress);	//プロキシの設定
		if ( servhost == NULL ){
			fprintf(stderr, "\n[%s] から IP アドレスへの変換に失敗しました。\n", ProxyAddress);
			return 0;
		}
	}

    bzero(&server, sizeof(server));            /* 構造体をゼロクリア */
	
    server.sin_family = AF_INET;

                                               /* IPアドレスを示す構造体をコピー */
    bcopy(servhost->h_addr, &server.sin_addr, servhost->h_length);

	if(strcmp(ProxyAddress, "") == 0){
		if ( port != 0 ){                          /* 引数でポート番号が指定されていたら */
			server.sin_port = htons(port);
		} else {                                   /* そうでないなら getservbyname でポート番号を取得 */
			service = getservbyname("http", "tcp");
			if ( service != NULL ){                /* 成功したらポート番号をコピー */
				server.sin_port = service->s_port;
			} else {                               /* 失敗したら 80 番に決め打ち */
				server.sin_port = htons(80);
			}
		}
	}
	else{
		server.sin_port = htons(ProxyPort);		//プロキシのポートを設定する
	}
	
	puts("OK");
	return 0;
}

static int  send_cmd(const char command[], char parameter[]){
    char buf[BUF_LEN];					//サーバ返答
    char WebBuf[BUF_LEN*40];
    int s;                               /* ソケットのためのファイルディスクリプタ */
    char send_buf[BUF_LEN];              /* サーバに送る HTTP プロトコル用バッファ */

    static char SessionID[100];					//セッションID
    char *SessionIDstart;				//セッションID記入開始位置
    char *SessionIDend;					//セッションID記入終了位置
	int	 SessionIDlength;				//セッションIDの長さ

	char ReturnBuf[BUF_LEN];
	char *ReturnBufStart;
	char *ReturnBufEnd;
	int  ReturnBufLength;

	char keyword[MAX_KEYWORD][30]=
		{
			"user=", "<input",
			"command1=", "<input",
			"GetReady ReturnCode=", "\n",
			"command2=", "<input",
			"Action ReturnCode=", "\n",
			"command3=", "<input",
			"roomNumber=", "<input"
		};


	int  i;

	ReturnCode[0] = '\0';

	/* ソケット生成 */
    if ( ( s = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		fprintf(stderr, "socket の生成に失敗しました。\n");	//カタカナの「そ」はワーニングが出る
        return 1;
    }

    /* サーバに接続 */
    if ( connect(s, (struct sockaddr *)&server, sizeof(server)) == -1 ){
        fprintf(stderr, "connect に失敗しました。\n");
        return 1;
    }

    /* HTTP プロトコル生成 & サーバに送信 */
	if(strcmp(SessionID, "")==0){
		sprintf(send_buf, 
			"GET http://%s/CHaserOnline003/user/%s?%s HTTP/1.1\r\n",
			host, command, parameter);
	}
	else{
		sprintf(send_buf, 
			"GET http://%s/CHaserOnline003/user/%s;jsessionid=%s?%s HTTP/1.1\r\n",
			host, command, SessionID, parameter);
	}

	printf("\nparameter=%s\n", parameter);


	write(s, send_buf, strlen(send_buf));
    sprintf(send_buf, "Host: %s:%d\r\n", host, port);
    write(s, send_buf, strlen(send_buf));
    sprintf(send_buf, "\r\n");
    write(s, send_buf, strlen(send_buf));

	/* あとは受信して、表示するだけ */
        int read_size;
        read_size = read(s, buf, BUF_LEN);
		buf[read_size] = '\0';

        if ( read_size > 0 ){

			strcpy(WebBuf, buf);

			do{
				read_size = read(s, buf, BUF_LEN);
				buf[read_size] = '\0';

				strcat(WebBuf, buf);
			}while(read_size >= BUF_LEN);

			if(SessionID[0] == '\0'){
				//サーバ返答からセッションＩＤを取り出す
				SessionIDstart = strstr(WebBuf, "JSESSIONID=");
				if(SessionIDstart != NULL){
					SessionIDend = strchr(SessionIDstart, ';');
					if(SessionIDend != NULL){
						SessionIDlength = SessionIDend - SessionIDstart - 11; 
						strncpy(SessionID, SessionIDstart+11, SessionIDlength);
						SessionID[SessionIDlength] = '\0';	//文字列終端の補正
					}
					else{
					}
				}
				else{
				}
			}

			for(i = 0; i<=MAX_KEYWORD; i=i+2){
				if(keyword[i][0]=='\0'){
					break;
				}
				//サーバ返答からReturnCodeを取り出す
				ReturnBufStart = strstr(WebBuf, keyword[i]);
				if(ReturnBufStart != NULL){
					ReturnBufEnd = strstr(ReturnBufStart, keyword[i+1]);
					if(ReturnBufEnd != NULL){
						ReturnBufLength = ReturnBufEnd - ReturnBufStart - strlen(keyword[i]); 
						if(ReturnBufLength == 0){
							strcpy(ReturnBuf, keyword[i]);
						}
						else{
							strncpy(ReturnBuf, ReturnBufStart+strlen(keyword[i]), ReturnBufLength);
							ReturnBuf[ReturnBufLength] = '\0';	//文字列終端の補正
						}
						//2016.2.8
						if(strlen(ReturnBuf)==1){	//もしもリターンコードが1文字だったらカンマを追加する
							strcat(ReturnBuf, ",");
						}

						strcpy(ReturnCode, ReturnBuf);
						i = MAX_KEYWORD + 1;

						close(s);
						return 0;
					}
				}
			}
		}
	/* 後始末 */
    close(s);
	return 0;
}

static int  ReturnCode_to_ReturnNumber() {

	int	 i = 0;
	int  count = 0;

	char *buf;
	char *endp;

	printf("ReturnCode = %s\n", ReturnCode);	//テスト用(XXX)

	buf = strtok(ReturnCode, ",");
	if (buf != NULL) {	// , ( カンマ ) が ReturnCode 内になかったら
		count++;
		ReturnNumber[0] = (int)strtol(buf, &endp, 10);

		if (*endp != 0){	// 数字以外の文字が含まれていたら

			return FAILURE;
		}
		else {

			for(i = 1; i < 9; i++){
				buf = strtok(NULL, ",");
				if (buf != NULL){
					count++;
					ReturnNumber[i] = atoi(buf);
				}
				else {
					break;
				}
			}
		}
		return count;
	}
	else {
		return FAILURE;
	}
}

static char* enum2strcmd(ecmd cmd) {
	int mode;
	int direction;
	static char cmd_str[MAX_CMD_LEN];

	mode = cmd / MAX_CMD_LEN;
	direction = cmd % MAX_CMD_LEN;

	strcpy(cmd_str, commands[mode][direction]);

	return cmd_str;
}

