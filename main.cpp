//起動パラメータの書式
//	./CHaserOnlineClient009-2Proxy.o ターゲットurl [-x プロキシアドレス:プロキシポート -u ユーザID -p パスワード -r ルーム番号]
//	パラメータの順番、有無は任意でよい

#include <string>

/**
 * @brief ターンの状態を示す
 * 
 */
enum class Turn {
    GETREADY,
    ACTION,
    END
};

/**
 * @brief CHaser ゲームのエントリポイントになるクラス
 * 
 */
class Game {
public:
    /**
     * @brief サーバに接続する。
     * 
     * @param hostUrl CHaser サーバへの URL (ex) "http://localhost:8080/CHaserOnline003/user/"
     * @param userId  ユーザ ID
     * @param userPw  ユーザパスワード
     */
    void connect(std::string hostUrl, std::string userId, std::string userPw);
    
    /**
     * @brief サーバにコマンドを送信する。
     * 
     * 備考:
     * 異常系に対応するために Turn 列挙子を引数に取った。サーバとの通信の過程で
     * 2度同じコマンドパターンが求められた場合に対応できる。
     * 
     * sendGetReady(), sendAction() sendEnd() として個別にメソッドを
     * 定義するのも考えたが、先に述べたようなシチュエーションに対応できなくなるため、
     * Turn の switch 文で多少関数の記述が長くなる事はトレードオフだろうと考えた。
     * 
     * @param now 現在のターン
     * @param cmdStr 送信するコマンドの文字列
     */
    void send(Turn now, std::string cmdStr);

    /**
     * @brief ゲームが続行するかを取得する。
     * 
     * @return true 続行
     * @return false 終了
     */
    bool continues() const noexcept;

    /**
     * @brief ターンの状態を取得する
     * 
     * @return Turn 現在のターン (send() の実行を境目に変わる)
     */
    Turn getTurn() const noexcept;
};

