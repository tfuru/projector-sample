#include <Arduino.h>
#include <VarSpeedServo.h> 

#include "main.h"

//仕様メモ
// サーボ 移動スピードを 5秒 程度まで遅く
// 360サーボの初期値 設定は マイクロスイッチ割り込み
// 移動処理 切れ目で 1秒 停止

VarSpeedServo servo180;
VarSpeedServo servo360;

int angles[] = {180,90,0,90};
int currentAngleIndex = 0;
#define ANGLE_INDEX_MAX 4

//初期値設定 SW 割り込み
void interruptZeroSw(){
    int v = digitalRead(SERVO360_ZERO_SW_PIN);
    Serial.print("interruptZeroSw "); Serial.println(v);
    Serial.print("currentAngleIndex "); Serial.println(currentAngleIndex);
    if(v == HIGH) return;
    if(status == INIT){
        status = SERVO360_ZERO_SUCCESS;
    }
    else if(status == RUN){
        currentAngleIndex++;
        if(ANGLE_INDEX_MAX == currentAngleIndex) currentAngleIndex = 0;
        status = SERVO360_ANGLE_90;
    }
}

void setup() {
    Serial.begin(9600);

    //ステッピングモーター初期値 割り込み設定 
    pinMode(SERVO360_ZERO_SW_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SERVO360_ZERO_SW_PIN), interruptZeroSw, CHANGE);

    //サーボ 初期値
    servo180.attach(SERVO180_PIN);
    servo180.write(90, SERVO_SPEED_MIN, false);
    
    servo360.attach(SERVO360_PIN);
    servo360.write(90, SERVO_SPEED_MIN, false);
}

int speedRead(){
    int a = 0;
    for(int i=0;i<100;i++){
        int v = analogRead(VOLUME_PIN);
        a += map(v, 0, 1023, SERVO_SPEED_MIN, SERVO_SPEED_MAX);
    }
    int s = a/100;
    Serial.print("s "); Serial.println(s);
    return s;
}

void loopInit() {
    Serial.println("loopInit ");
    //初期値を設定するために割り込みがあるまで 360度サーボは 回転を続ける    
}

//割り込みがあって 初期値設定が終わった
void loopServo360ZeroSuccess() {
    Serial.println("loopServo360ZeroSuccess");   
    //360サーボを停止する
    servo360.detach();    
    delay(5000);
    //通常の遷移へ
    status = RUN;
}

void setServoAngle(int angle){
    servo360.attach(SERVO360_PIN);    
    servo360.write(90, speedRead(), false);
    servo180.write(angle, speedRead(), false);
}

void loopRun() {
    Serial.println("loopRun");
    setServoAngle(angles[currentAngleIndex]);
}

void loopServo360Angle90(){
    Serial.println("loopServo360Angle90");
    servo360.detach();
    delay(2000);
    //通常の遷移へ
    status = RUN;   
}

void loop() {
    switch (status){
        case INIT:
            loopInit();
            break;
        case SERVO360_ZERO_SUCCESS:
            loopServo360ZeroSuccess();
            break;
        case RUN:
            loopRun();
            break;
        case SERVO360_ANGLE_90:
            loopServo360Angle90();
            break;            
    }

}