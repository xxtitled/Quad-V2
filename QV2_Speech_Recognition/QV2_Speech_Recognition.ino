#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <BluetoothSerial.h> // 블루투스 연결

BluetoothSerial SerialBT;

// ESP32 I2C 핀 정의
#define SDA_PIN    21
#define SCL_PIN    22

// PCA9685 16채널 서보 드라이버
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN   150
#define SERVOMAX   600
#define SERVO_FREQ 50

// [] 그룹에 묶어서 제어하는 다리 수
// Front Leg -->FLEG
const uint8_t FLEG_SIZE = 1;

// A: Front Left Leg (힙, 무릎, 발목) 서브 서보 -> Ankle // 메인 서보 --> Knee // Roll 축 서보 --> Hip
const uint8_t HIP_A[FLEG_SIZE] = {2};
const uint8_t KNEE_A[FLEG_SIZE] = {1};
const uint8_t ANKLE_A[FLEG_SIZE] = {0};

// B: Back Left Leg (힙, 무릎, 발목)
const uint8_t HIP_B[FLEG_SIZE] = {6};
const uint8_t KNEE_B[FLEG_SIZE] = {5};
const uint8_t ANKLE_B[FLEG_SIZE] = {4};

/* ------------- UPPER ------------- */

// [] 그룹에 묶어서 제어하는 다리 수
// Back Leg --> BLEG
const uint8_t BLEG_SIZE = 1;

// C: Front Right Leg (힙, 무릎, 발목)
const uint8_t HIP_C[BLEG_SIZE] = {10};
const uint8_t KNEE_C[BLEG_SIZE] = {9};
const uint8_t ANKLE_C[BLEG_SIZE] = {8};

// D: Back Right Leg (힙, 무릎, 발목)
const uint8_t HIP_D[BLEG_SIZE] = {14};
const uint8_t KNEE_D[BLEG_SIZE] = {13};
const uint8_t ANKLE_D[BLEG_SIZE] = {12};


// PWM 서보 각도 설정
void setAngle(uint8_t ch, float angle) {
  angle = constrain(angle, 0, 180);
  uint16_t pulse = map((int)angle, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(ch, 0, pulse);
}

// ============= FRONT ============= 
void setGroupA(float hip, float knee, float ankle) {    // Left
  setAngle(HIP_A[0], hip);
  setAngle(KNEE_A[0], knee);
  setAngle(ANKLE_A[0], ankle);
}

void setGroupB(float hip, float knee, float ankle) {    // Right
  setAngle(HIP_B[0], hip);
  setAngle(KNEE_B[0], knee);
  setAngle(ANKLE_B[0], ankle);
}

// ============= BACK ============= 
void setGroupC(float hip, float knee, float ankle) {    // Legt
  setAngle(HIP_C[0], hip);
  setAngle(KNEE_C[0], knee);
  setAngle(ANKLE_C[0], ankle);
}
void setGroupD(float hip, float knee, float ankle) {    // Right
  setAngle(HIP_D[0], hip);
  setAngle(KNEE_D[0], knee);
  setAngle(ANKLE_D[0], ankle);
}


void setup() {
  Serial.begin(115200);
  SerialBT.begin("VIN_ESP32");
  Serial.println("Bluetooth Started! Device name: VIN_ESP32");
  delay(100);

  Wire.begin(SDA_PIN, SCL_PIN);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  Serial.println("QV2 Control Ready");

  // 초기 포즈: 일어난 상태
  setGroupA(90, 140, 110);
  setGroupB(90, 140, 110);
  setGroupC(90, 40, 70);
  setGroupD(90, 40, 70);
}


void standup() { // 일어나기
  setGroupA(90, 140, 110);
  setGroupB(90, 140, 110);  
  setGroupC(90, 40, 70);
  setGroupD(90, 40, 70);      
  Serial.println("일어나기");
}

void trot() { // 제자리 Trot Gait --> 준비
  setGroupA(90, 140, 110);
  setGroupB(90, 140, 110);
  setGroupC(90, 40, 70);
  setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
  delay(200);
  setGroupA(105, 150, 95); //F_L 발 들기
  setGroupD(75, 30, 85);  //B_R 발 들기
  delay(200);
  setGroupA(95, 150, 120); //F_L 발 밀기
  setGroupD(85, 30, 60);  //B_R 발 밀기
  delay(200);
  setGroupA(90, 140, 110); //F_L 발 원상태
  setGroupD(90, 40, 65);  //B_R 발 원상태
  delay(200);
  setGroupB(105, 150, 95);  //F_R 발 들기
  setGroupC(75, 30, 85);  //B_L 발 들기
  delay(200);
  setGroupB(95, 150, 120);  //F_R 발 밀기
  setGroupC(85, 30, 60);  //B_L 발 밀기
  delay(200);
  setGroupB(90, 140, 110);  //F_R 원상태
  setGroupC(90, 40, 70);  //B_L 원상태
  delay(250); // ===== 초기화 =====
  setGroupA(90, 140, 110);
  setGroupB(90, 140, 110);
  setGroupC(90, 40, 70);
  setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
  delay(200);
  setGroupA(105, 150, 95); //F_L 발 들기
  setGroupD(75, 30, 85);  //B_R 발 들기
  delay(200);
  setGroupA(95, 150, 120); //F_L 발 밀기
  setGroupD(85, 30, 60);  //B_R 발 밀기
  delay(200);
  setGroupA(90, 140, 110); //F_L 발 원상태
  setGroupD(90, 40, 65);  //B_R 발 원상태
  delay(200);
  setGroupB(105, 150, 95);  //F_R 발 들기
  setGroupC(75, 30, 85);  //B_L 발 들기
  delay(200);
  setGroupB(95, 150, 120);  //F_R 발 밀기
  setGroupC(85, 30, 60);  //B_L 발 밀기
  delay(200);
  setGroupB(90, 140, 110);  //F_R 원상태
  setGroupC(90, 40, 70);  //B_L 원상태
  Serial.println("제자리"); // 앞으로 가기
}

void go() { // 전진 Trot Gait
  setGroupA(90, 140, 110);
  setGroupB(90, 140, 110);
  setGroupC(90, 40, 70);
  setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
  delay(200);
  setGroupA(90, 150, 95); //F_L 발 들기
  setGroupD(90, 30, 85);  //B_R 발 들기
  delay(200);
  setGroupA(90, 130, 90); //F_L 발 밀기
  setGroupD(90, 50, 90);  //B_R 발 밀기
  delay(200);
  setGroupA(90, 140, 110); //F_L 발 원상태
  setGroupD(90, 40, 65);  //B_R 발 원상태
  delay(200);
  setGroupB(90, 150, 95);  //F_R 발 들기
  setGroupC(90, 30, 85);  //B_L 발 들기
  delay(200);
  setGroupB(90, 130, 90);  //F_R 발 밀기
  setGroupC(90, 50, 90);  //B_L 발 밀기
  delay(200);
  setGroupB(90, 140, 110);  //F_R 원상태
  setGroupC(90, 40, 70);  //B_L 원상태
  delay(250); // ===== 초기화 =====
  setGroupA(90, 140, 110);
  setGroupB(90, 140, 110);
  setGroupC(90, 40, 70);
  setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
  delay(200);
  setGroupA(90, 150, 95); //F_L 발 들기
  setGroupD(90, 30, 85);  //B_R 발 들기
  delay(200);
  setGroupA(90, 130, 90); //F_L 발 밀기
  setGroupD(90, 50, 90);  //B_R 발 밀기
  delay(200);
  setGroupA(90, 140, 110); //F_L 발 원상태
  setGroupD(90, 40, 65);  //B_R 발 원상태
  delay(200);
  setGroupB(90, 150, 95);  //F_R 발 들기
  setGroupC(90, 30, 85);  //B_L 발 들기
  delay(200);
  setGroupB(90, 130, 90);  //F_R 발 밀기
  setGroupC(90, 50, 90);  //B_L 발 밀기
  delay(200);
  setGroupB(90, 140, 110);  //F_R 원상태
  setGroupC(90, 40, 70);  //B_L 원상태
  Serial.println("전진");
}

void sitdown() { // 앉기
  setGroupA(90, 180, 90);
  setGroupB(90, 180, 90); 
  setGroupC(90, 0, 90);
  setGroupD(90, 0, 90);
  Serial.println("앉기");
}

void leftroll() { // Left Roll
  setGroupA(70, 140, 110); // --> Roll 좌 움직임
  setGroupB(110, 140, 110);  
  setGroupC(70, 40, 70);
  setGroupD(110, 40, 70);      
  Serial.println("left Roll");
}

void rightroll() { // Right Roll
  setGroupA(110, 140, 110); // --> Roll 우 움직임
  setGroupB(70, 140, 110);  
  setGroupC(110, 40, 70);
  setGroupD(70, 40, 70); 
  Serial.println("Right Roll");
}

void frontpitch() { // Front Pitch 
  setGroupA(90, 140, 110);  // FL / --> 앞으로 숙이기
  setGroupB(90, 120, 110);  // BL
  setGroupC(90, 40, 70);  // FR
  setGroupD(90, 60, 70);  // BR
  Serial.println("Front Pitch");
}

void backpitch() { // Back Pitch 
  setGroupA(90, 120, 110);  // FL / --> 뒤로 숙이기
  setGroupB(90, 140, 110);  // BL
  setGroupC(90, 60, 70);  // FR
  setGroupD(90, 40, 70);  // BR
  Serial.println("Back Pitch");
}

void loop() {
  if (SerialBT.available()) {
    String cmd = SerialBT.readStringUntil('\n');
    cmd.trim();
    Serial.print("CMD> "); Serial.println(cmd);

    if (cmd.equalsIgnoreCase("up") || cmd.equals("일어나") || cmd.equals("멈춰")) {
      standup();
    }
    else if (cmd.equalsIgnoreCase("trot") || cmd.equals("준비")) {
      trot();
    }
    else if (cmd.equalsIgnoreCase("go") || cmd.equals("전진") || cmd.equals("앞으로가")) {
      go();
    }
    else if (cmd.equalsIgnoreCase("down") || cmd.equals("앉아") || cmd.equals("아래")) {
      sitdown();
    }
    else if (cmd.equalsIgnoreCase("leftroll") || cmd.equals("왼쪽")) {
      leftroll();
    }
    else if (cmd.equalsIgnoreCase("rightroll") || cmd.equals("오른쪽")) {
      rightroll();
    }
    else if (cmd.equalsIgnoreCase("frontpitch") || cmd.equals("앞쪽")) {
      frontpitch();
    }
    else if (cmd.equalsIgnoreCase("backpitch") || cmd.equals("뒤쪽")) {
      backpitch();
    }

    else {
      Serial.println("Unknown command");
    }
  }
  delay(20); // 과부화 방지
}