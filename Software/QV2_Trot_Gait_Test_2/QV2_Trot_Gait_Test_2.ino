#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// ESP32 I2C 핀 정의
#define SDA_PIN    21
#define SCL_PIN    22

// PCA9685 16채널 서보 드라이버
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN   150
#define SERVOMAX   600
#define SERVO_FREQ 50

/* ------------- LOWER ------------- */

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

// ============= BACk ============= 
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
  delay(100);
  Wire.begin(SDA_PIN, SCL_PIN);
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);

  Serial.println("QV2 Control Ready");

  // 초기 포즈: 다리 모두 중립(90°)
  setGroupA(90, 180, 90);
  setGroupB(90, 180, 90);
  setGroupC(90, 0, 90);
  setGroupD(90, 0, 90);
}

void loop() {
  if (!Serial.available()) return;
  char cmd = Serial.read();
  switch (cmd) {

    /* ------------- LOWER ------------- */

    case 'q': case 'Q': // --> 힙 무릎 발목 / 완전히 수축 
      setGroupA(90, 180, 90); // --> 초기 설정        // L
      setGroupB(90, 180, 90); 
      setGroupC(90, 0, 90);
      setGroupD(90, 0, 90);    
      Serial.println("Q: ");
      break;

    case 'w': case 'W': // --> 일어나기 확정
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);  
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 70);      
      Serial.println("W: ");
      break;

    case 'e': case 'E': // --> 일어난 상태에서 약간 앞으로 발 뻣기 
      setGroupA(90, 130, 90);
      setGroupB(90, 130, 90);
      setGroupC(90, 50, 90);
      setGroupD(90, 50, 90);
      Serial.println("E: ");
      break;

    case 'r': case 'R': // --> 보행 준비
      setGroupA(90, 160, 120); // 서보혼 더 앞으로 당기기 즉, 발목 펴기 --> 
      setGroupB(90, 160, 120);
      setGroupC(90, 20, 60);
      setGroupD(90, 20, 60);
      Serial.println("R: ");
      break;

    case 't': case 'T': // --> 제자리 Trot Gait 1 -- 약하게
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
      delay(250);
      setGroupA(90, 145, 95); //F_L 발 들기
      setGroupD(90, 35, 85);  //B_R 발 들기
      delay(250);
      setGroupA(90, 140, 110); //F_L
      setGroupD(90, 40, 65);  //B_R
      delay(250);
      setGroupB(90, 145, 95);  //F_R
      setGroupC(90, 35, 85);  //B_L
      delay(250);
      setGroupB(90, 140, 110);  //F_R
      setGroupC(90, 40, 70);  //B_L
      Serial.println("T: ");
      break;

    case 'o': case 'O': // --> 제자리 Trot Gait 2 -- 강하게
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
      delay(250);
      setGroupA(105, 150, 95); //F_L 발 들기
      setGroupD(75, 30, 85);  //B_R 발 들기
      delay(250);
      setGroupA(90, 140, 110); //F_L
      setGroupD(90, 40, 65);  //B_R
      delay(250);
      setGroupB(105, 150, 95);  //F_R 발 들기
      setGroupC(75, 30, 85);  //B_L 발 들기
      delay(250);
      setGroupB(90, 140, 110);  //F_R
      setGroupC(90, 40, 70);  //B_L
      Serial.println("O: ");
      break;


    case 'p': case 'P': // --> 제자리 Trot Gait 3 
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
      Serial.println("P: ");
      break;

    //================================

    case 'b': case 'B': // --> 전진 Trot Gait 1 (대각선 발 들때 몸 안쪽으로 끌고오기)
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
      delay(200);
      setGroupA(105, 150, 95); //F_L 발 들기
      setGroupD(75, 30, 85);  //B_R 발 들기
      delay(200);
      setGroupA(95, 130, 90); //F_L 발 밀기
      setGroupD(85, 50, 90);  //B_R 발 밀기
      delay(200);
      setGroupA(90, 140, 110); //F_L 발 원상태
      setGroupD(90, 40, 65);  //B_R 발 원상태
      delay(200);
      setGroupB(105, 150, 95);  //F_R 발 들기
      setGroupC(75, 30, 85);  //B_L 발 들기
      delay(200);
      setGroupB(95, 130, 90);  //F_R 발 밀기
      setGroupC(85, 50, 90);  //B_L 발 밀기
      delay(200);
      setGroupB(90, 140, 110);  //F_R 원상태
      setGroupC(90, 40, 70);  //B_L 원상태
      Serial.println("B: ");
      break;


    case 'n': case 'N': // --> 전진 Trot Gait 2 (Z 축으로만 발 들기)
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
      Serial.println("N: ");
      break;


    case 'y': case 'Y': // --> 제자리 발 구르기 길게
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 70); // 발 위치 맞추기
      delay(250);
      setGroupA(90, 145, 95); //F_L 발 들기
      setGroupD(90, 35, 85);  //B_R 발 들기
      delay(250);
      setGroupA(90, 140, 110); //F_L
      setGroupD(90, 40, 70);  //B_R
      delay(250);
      setGroupB(90, 145, 95);  //F_R
      setGroupC(90, 35, 85);  //B_L
      delay(250);
      setGroupB(90, 140, 110);  //F_R
      setGroupC(90, 40, 70);  //B_L -- 초기 상태
      delay(250);
      setGroupA(90, 145, 95); //F_L 발 들기
      setGroupD(90, 35, 85);  //B_R 발 들기
      delay(250);
      setGroupA(90, 140, 110); //F_L
      setGroupD(90, 40, 70);  //B_R
      delay(250);
      setGroupB(90, 145, 95);  //F_R
      setGroupC(90, 35, 85);  //B_L
      delay(250);
      setGroupB(90, 140, 110);  //F_R
      setGroupC(90, 40, 70);  //B_L
      Serial.println("T: ");
      break;

    case '0': // --> 전진 Trot Gait
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
      Serial.println("0: 전진");
      break;

    case '9': // --> 전진 Trot Gait 더 빠르게
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
      delay(150);
      setGroupA(90, 150, 95); //F_L 발 들기
      setGroupD(90, 30, 85);  //B_R 발 들기
      delay(150);
      setGroupA(90, 130, 90); //F_L 발 밀기
      setGroupD(90, 50, 90);  //B_R 발 밀기
      delay(150);
      setGroupA(90, 140, 110); //F_L 발 원상태
      setGroupD(90, 40, 65);  //B_R 발 원상태
      delay(150);
      setGroupB(90, 150, 95);  //F_R 발 들기
      setGroupC(90, 30, 85);  //B_L 발 들기
      delay(150);
      setGroupB(90, 130, 90);  //F_R 발 밀기
      setGroupC(90, 50, 90);  //B_L 발 밀기
      delay(150);
      setGroupB(90, 140, 110);  //F_R 원상태
      setGroupC(90, 40, 70);  //B_L 원상태
      delay(150); // ===== 초기화 =====
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
      delay(150);
      setGroupA(90, 150, 95); //F_L 발 들기
      setGroupD(90, 30, 85);  //B_R 발 들기
      delay(150);
      setGroupA(90, 130, 90); //F_L 발 밀기
      setGroupD(90, 50, 90);  //B_R 발 밀기
      delay(150);
      setGroupA(90, 140, 110); //F_L 발 원상태
      setGroupD(90, 40, 65);  //B_R 발 원상태
      delay(150);
      setGroupB(90, 150, 95);  //F_R 발 들기
      setGroupC(90, 30, 85);  //B_L 발 들기
      delay(150);
      setGroupB(90, 130, 90);  //F_R 발 밀기
      setGroupC(90, 50, 90);  //B_L 발 밀기
      delay(150);
      setGroupB(90, 140, 110);  //F_R 원상태
      setGroupC(90, 40, 70);  //B_L 원상태
      delay(150); // ===== 초기화 =====
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
      delay(150);
      setGroupA(90, 150, 95); //F_L 발 들기
      setGroupD(90, 30, 85);  //B_R 발 들기
      delay(150);
      setGroupA(90, 130, 90); //F_L 발 밀기
      setGroupD(90, 50, 90);  //B_R 발 밀기
      delay(150);
      setGroupA(90, 140, 110); //F_L 발 원상태
      setGroupD(90, 40, 65);  //B_R 발 원상태
      delay(150);
      setGroupB(90, 150, 95);  //F_R 발 들기
      setGroupC(90, 30, 85);  //B_L 발 들기
      delay(150);
      setGroupB(90, 130, 90);  //F_R 발 밀기
      setGroupC(90, 50, 90);  //B_L 발 밀기
      delay(150);
      setGroupB(90, 140, 110);  //F_R 원상태
      setGroupC(90, 40, 70);  //B_L 원상태
      delay(150); // ===== 초기화 =====
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
      delay(150);
      setGroupA(90, 150, 95); //F_L 발 들기
      setGroupD(90, 30, 85);  //B_R 발 들기
      delay(150);
      setGroupA(90, 130, 90); //F_L 발 밀기
      setGroupD(90, 50, 90);  //B_R 발 밀기
      delay(150);
      setGroupA(90, 140, 110); //F_L 발 원상태
      setGroupD(90, 40, 65);  //B_R 발 원상태
      delay(150);
      setGroupB(90, 150, 95);  //F_R 발 들기
      setGroupC(90, 30, 85);  //B_L 발 들기
      delay(150);
      setGroupB(90, 130, 90);  //F_R 발 밀기
      setGroupC(90, 50, 90);  //B_L 발 밀기
      delay(150);
      setGroupB(90, 140, 110);  //F_R 원상태
      setGroupC(90, 40, 70);  //B_L 원상태
      delay(150); // ===== 초기화 =====
      setGroupA(90, 140, 110);
      setGroupB(90, 140, 110);
      setGroupC(90, 40, 70);
      setGroupD(90, 40, 65); // 발 위치 맞추기 --> 서브 서보 수정 전
      delay(150);
      setGroupA(90, 150, 95); //F_L 발 들기
      setGroupD(90, 30, 85);  //B_R 발 들기
      delay(150);
      setGroupA(90, 130, 90); //F_L 발 밀기
      setGroupD(90, 50, 90);  //B_R 발 밀기
      delay(150);
      setGroupA(90, 140, 110); //F_L 발 원상태
      setGroupD(90, 40, 65);  //B_R 발 원상태
      delay(150);
      setGroupB(90, 150, 95);  //F_R 발 들기
      setGroupC(90, 30, 85);  //B_L 발 들기
      delay(150);
      setGroupB(90, 130, 90);  //F_R 발 밀기
      setGroupC(90, 50, 90);  //B_L 발 밀기
      delay(150);
      setGroupB(90, 140, 110);  //F_R 원상태
      setGroupC(90, 40, 70);  //B_L 원상태
      Serial.println("9: 전진");
      break;



    //============= 보행 =============

    case 'a': case 'A': // --> 낮은 보행
      setGroupA(90, 160, 120);  // F_L - B_R
      setGroupB(90, 150, 100);  // B_L
      setGroupC(90, 30, 80);    // F_R
      setGroupD(90, 20, 60);    // B_R - F_L
      Serial.println("A: ");
      break;

    case 'z': case 'Z': // --> 보행1 --> 오 왼 당기고 펴기
      setGroupA(90, 120, 110);  // F_L - B_R
      setGroupB(90, 130, 125);  // B_L
      setGroupC(90, 50, 55);    // F_R
      setGroupD(90, 60, 65);    // B_R - F_L
      delay(500);
      setGroupC(90, 40, 70);    // F_R
      setGroupB(90, 140, 110);  // B_L
      Serial.println("Z: ");
      break;


    case 's': case 'S': // --> 낮은 보행
      setGroupA(90, 150, 100);  // F_L
      setGroupB(90, 160, 120);  // B_L - F_R
      setGroupC(90, 20, 60);    // F_R - B_L
      setGroupD(90, 30, 80);    // B_R 
      Serial.println("S: ");
      break;

    case 'x': case 'X': // --> 보행1
      setGroupA(90, 130, 125);  // F_L
      setGroupB(90, 120, 110);  // B_L - F_R
      setGroupC(90, 60, 70);    // F_R - B_L
      setGroupD(90, 50, 55);    // B_R
      delay(500);
      setGroupA(90, 130, 115);  // F_L
      setGroupD(90, 50, 65);    // B_R
      Serial.println("X: ");
      break;

    //=========================================

    case 'c': case 'C': // --> 보행 2--> 오 왼 당기고 펴기
      setGroupA(100, 120, 120);  // F_L - B_R
      setGroupB(100, 130, 125);  // B_L
      setGroupC(100, 50, 55);    // F_R
      setGroupD(100, 60, 60);    // B_R - F_L
      delay(500);
      setGroupA(90, 120, 120);  // F_L
      setGroupB(90, 130, 125);
      setGroupC(90, 50, 55);    // F_R
      setGroupD(90, 60, 60);  // B_L
      Serial.println("C: ");
      break;


    case 'v': case 'V': // --> 보행 2
      setGroupA(80, 130, 125);  // F_L
      setGroupB(80, 120, 110);  // B_L - F_R
      setGroupC(80, 60, 70);    // F_R - B_L
      setGroupD(80, 50, 55);    // B_R
      delay(500);
      setGroupA(90, 130, 125);  // F_L
      setGroupB(90, 120, 110);
      setGroupC(90, 60, 70);    // F_R
      setGroupD(90, 50, 55);  // B_L
      Serial.println("V: ");
      break;



    //==========================
    case 'd': case 'D': // --> 포복
      setGroupA(90, 170, 110);
      setGroupB(90, 170, 110);
      setGroupC(90, 10, 70);
      setGroupD(90, 10, 70);
      Serial.println("D: ");
      break;

    // -------------- Pitch -------------- 

    case 'f': case 'F': // --> 앞으로 숙이기
      setGroupA(90, 140, 110);  // FL
      setGroupB(90, 120, 110);  // BL
      setGroupC(90, 40, 70);  // FR
      setGroupD(90, 60, 70);  // BR
      Serial.println("F: ");
      break;

    case 'g': case 'G': // --> 뒤로 숙이기
      setGroupA(90, 120, 110);  // FL
      setGroupB(90, 140, 110);  // BL
      setGroupC(90, 60, 70);  // FR
      setGroupD(90, 40, 70);  // BR
      Serial.println("G: ");
      break;
   
    
    // -------------- Roll -------------- 

    case 'h': case 'H': // --> 완전히 앉은 상태에서 +Z 방향으로 Roll 회전
      setGroupA(160, 180, 90);
      setGroupB(20, 180, 90);
      setGroupC(20, 0, 90);  
      setGroupD(160, 0, 90); 
      Serial.println("H: ");
      break;

    case 'j': case 'J': // --> 완전히 앉은 상태에서 -Z 방향으로 Roll 회전
      setGroupA(40, 180, 90);
      setGroupB(140, 180, 90);
      setGroupC(140, 0, 90);  
      setGroupD(40, 0, 90); 
      Serial.println("J: ");
      break;

    case 'k': case 'K': // --> 완전히 앉은 상태에서 Front +Z 방향으로 Roll 회전, Back -Z 방향으로 Roll 회전 --> 전방 & 후방 반대로 회전
      setGroupA(150, 180, 90);
      setGroupB(150, 180, 90);
      setGroupC(30, 0, 90);  
      setGroupD(30, 0, 90); 
      Serial.println("K: ");
      break;

    case 'l': case 'L': // --> 완전히 앉은 상태에서 Front -Z 방향으로 Roll 회전, Back +Z 방향으로 Roll 회전 --> 전방 & 후방 반대로 회전
      setGroupA(30, 180, 90);
      setGroupB(30, 180, 90);
      setGroupC(150, 0, 90);  
      setGroupD(150, 0, 90); 
      Serial.println("L: ");
      break;

  case 'u': case 'U': // --> 일어난 상태 Roll  좌 움직임
      setGroupA(70, 140, 110);
      setGroupB(110, 140, 110);  
      setGroupC(70, 40, 70);
      setGroupD(110, 40, 70);      
      Serial.println("U: ");
      break;

   case 'i': case 'I': // --> 일어난 상태 Roll 우 움직임
      setGroupA(110, 140, 110);
      setGroupB(70, 140, 110);  
      setGroupC(110, 40, 70);
      setGroupD(70, 40, 70);      
      Serial.println("I: ");
      break;


    default:
      // 기타 입력 무시
      break;
  }
}