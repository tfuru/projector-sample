
//ボリューム スピードコントロール
#define VOLUME_PIN A0

//180 SERVO
#define SERVO180_PIN 9
//360 SERVO 
#define SERVO360_PIN 8

//サーボのスピード設定
#define SERVO_SPEED_MIN 6
#define SERVO_SPEED_MAX 60

//SW ステッピングモーター初期値 判定
#define SERVO360_ZERO_SW_PIN 2

//ステータス
enum STATUS{
    INIT,
    SERVO360_ZERO_SUCCESS,
    RUN,    
    SERVO360_ANGLE_90
};
STATUS status = INIT;