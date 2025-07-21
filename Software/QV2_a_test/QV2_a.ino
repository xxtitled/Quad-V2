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

// B: Front Right Leg (힙, 무릎, 발목)
const uint8_t HIP_B[FLEG_SIZE] = {6};
const uint8_t KNEE_B[FLEG_SIZE] = {5};
const uint8_t ANKLE_B[FLEG_SIZE] = {4};

/* ------------- UPPER ------------- */

// [] 그룹에 묶어서 제어하는 다리 수
// Back Leg --> BLEG
const uint8_t BLEG_SIZE = 1;

// C: Back Left Leg (힙, 무릎, 발목)
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
  setGroupB(90, 90, 90);
  setGroupC(90, 90, 90);
  setGroupD(90, 90, 90);
}

void loop() {
  if (!Serial.available()) return;
  char cmd = Serial.read();
  switch (cmd) {

    /* ------------- LOWER ------------- */

    case 'q': case 'Q': // --> 힙 무릎 발목 / 완전히 수축 
      setGroupA(90, 180, 90);     // --> 초기 설정        // R
      Serial.println("Q: ");
      break;

    case 'w': case 'W': // --> 일어나기
      setGroupA(90, 150, 110);       // R
      Serial.println("W: ");
      break;

    case 'e': case 'E': // --> 일어난 상태에서 약간 앞으로 발 뻣기 
      setGroupA(90, 90, 90);   // R
      Serial.println("E: ");
      break;

    case 'r': case 'R': // --> 적당히 일어나기
      setGroupA(90, 150, 90);
      Serial.println("R: ");
      break;

    case 'a': case 'A':
      setGroupA(90, 160, 120); // 서보혼 더 앞으로 당기기 즉, 발목 펴기
      Serial.println("A: ");
      break;

    case 's': case 'S': // 
      setGroupA(90, 150, 80);
      Serial.println("S: ");
      break;

    case 'd': case 'D': // 
      setGroupA(90, 160, 110);
      Serial.println("D: ");
      break;

    /* ------------ 보행 ------------ */
/*

case 'a': case 'A': // --> 전진
      setGroupA(90, 30, 120, 90);       // 오른발 앞으로
      setGroupB(90, 105, 60, 90);       // R
      setGroupC(105, 100, 105);         // L_ARM
      setGroupD(105, 80, 90);         
      setGroupE(95);
      delay(1000);
      setGroupA(90, 75, 110, 90);      // 왼발 앞으로
      setGroupB(90, 150, 60, 90);      // --> Hip & Knee 조금 더 뒤로 셋백
      setGroupC(75, 100, 90);          
      setGroupD(75, 80, 75);          // R_ARM
      setGroupE(85);
      delay(1000);
      setGroupA(90, 30, 120, 90);       // 오른발 앞으로
      setGroupB(90, 105, 60, 90);       // R
      setGroupC(105, 100, 105);         // L_ARM
      setGroupD(105, 80, 90);         
      setGroupE(95);
      delay(1000);
      setGroupA(90, 75, 110, 90);      // 왼발 앞으로
      setGroupB(90, 150, 60, 90);      // --> Hip & Knee 조금 더 뒤로 셋백
      setGroupC(75, 100, 90);          
      setGroupD(75, 80, 75);          // R_ARM
      setGroupE(85);
      Serial.println("A: Go Ahead");
      break;


    case '2'://  오른발 앞으로 = Right up --> # 발바닥 높이 차이가 중요. 높이 차에 따라 로봇이 회전. 허리 회전에 신경써야 한다. 허리 회전과 팔 회전에 따라 로봇 전체가 회전
      setGroupA(90, 30, 120, 90);
      setGroupB(90, 105, 60, 90);       // R
      setGroupC(105, 100, 105);         // L_ARM
      setGroupD(105, 80, 90);         
      setGroupE(95);
      Serial.println("Preset 2 applied");
      break;

    case '3': //  왼발 앞으로 = -->  Left up
      setGroupA(90, 75, 110, 90);      // L Knee 앞으로
      setGroupB(90, 150, 60, 90);      // --> Hip & Knee 조금 더 뒤로 셋백
      setGroupC(75, 100, 90);          
      setGroupD(75, 80, 75);          // R_ARM
      setGroupE(85);
      Serial.println("Preset 3 applied");
      break;


    //=============== More dynamic =================
    case '4'://  --> Right up --> 
      setGroupA(90, 30, 60, 90);
      setGroupB(90, 70, 80, 90);       // R
      setGroupC(105, 100, 105);        // L_ARM
      setGroupD(105, 80, 90);         
      setGroupE(95);
      Serial.println("Preset 4 applied");
      break;

    case '5': //  -->  Left up
      setGroupA(90, 110, 110, 90);      // L Knee 앞으로
      setGroupB(90, 150, 120, 90);      // 
      setGroupC(75, 100, 90);          
      setGroupD(75, 80, 75);           // R_ARM
      setGroupE(85);
      Serial.println("Preset 5 applied");
      break;

    case '0': // --> 보행 각도 세팅
      setGroupA(90, 55, 125, 90);
      setGroupB(90, 125, 55, 90);
      setGroupC(90, 100, 90);
      setGroupD(90, 80, 90);
      setGroupE(90);
      Serial.println("Preset 0 applied");
      break;

    // ------------------------------------ 

    
    case 'z': case 'Z': // --> R side step test
      setGroupA(65, 90, 90, 110);
      setGroupB(85, 90, 90, 70);
      setGroupC(90, 135, 85);  // L
      setGroupD(90, 45, 95);   // R
      setGroupE(90);
      Serial.println("Preset Z applied");
      break;

     case 'x': case 'X': // --> 다리 덜 벌리기
      setGroupA(70, 90, 90, 110);
      setGroupB(110, 90, 90, 70);
      setGroupC(90, 135, 85);  // L
      setGroupD(90, 45, 95);   // R
      setGroupE(90);
      Serial.println("Preset X applied");
      break;
    

    // ====== Y축 이동 ======

    case 't': case 'T': // --> Y축 왼쪽 이동 ====> Right Leg의 움직임이 중요 
      setGroupB(120, 90, 90, 80); // --> 골반; 오른쪽 다리 몸 밖으로 30도 이동 / 발목; 왼쪽으로  10도 --80
      setGroupA(95, 90, 90, 110); // --> 골반; 왼쪽 다리 몸 중심으로 5도 이동 / 발목; 왼쪽으로 20 -- 110
      setGroupC(90, 115, 85);  // L
      setGroupD(90, 65, 95);   // R
      setGroupE(90);
      delay(1000);
      setGroupA(90, 90, 90, 90);
      setGroupB(90, 90, 90, 90);
      setGroupC(90, 100, 90); // 
      setGroupD(90, 80, 90); // 
      setGroupE(90);
      Serial.println("T: Left side step");
      break;

    case 'y': case 'Y': // --> Y축 오른쪽 이동
      setGroupA(60, 90, 90, 100); // --> 골반; 왼쪽 다리 몸 밖으로 30도 이동 / 발목; 오른쪽으로  10도
      setGroupB(85, 90, 90, 70);  // --> 골반; 오른쪽 다리 몸 중심으로 5도 이동 / 발목; 오른쪽으로 20도
      setGroupC(90, 115, 85);  // L
      setGroupD(90, 65, 95);   // R
      setGroupE(90);
      delay(1000);
      setGroupA(90, 90, 90, 90);
      setGroupB(90, 90, 90, 90);
      setGroupC(90, 100, 90); // 
      setGroupD(90, 80, 90); // 
      setGroupE(90);
      Serial.println("Y: right side step");
      break;


    // ------------- UPPER ------------- 

    case 'd': case 'D': // --> 팔 X축과 평행하게 펴기
      setGroupC(180, 90, 80);  // L
      setGroupD(0, 90, 100);   // R
      setGroupE(90);
      Serial.println("X: arm extension along the X-axis");
      break;

    case 'f': case 'F': // --> 팔 Y축과 평행하게 펴기
      setGroupC(90, 180, 90);  // L
      setGroupD(90, 0, 90);    // R
      setGroupE(90);
      Serial.println("F: arm extension along the Y-axis");
      break;

    case 'c': case 'C': // --> 왼손 스트레이트
      setGroupC(180, 90, 80);  // L
      setGroupD(45, 90, 5);   // R --> 가드
      setGroupE(115);
      Serial.println("C: Straight Left");
      break;

    case 'v': case 'V': // --> 오른손 스트레이트
      setGroupC(135, 90, 175);  // L --> 가드
      setGroupD(0, 90, 100);    // R
      setGroupE(65);   //
      Serial.println("V: Straight Right");
      break;

    case 'b': case 'B': // --> 가드
      setGroupC(145, 70, 175);  // L
      setGroupD(35, 110, 5);   // R
      setGroupE(90);   //
      Serial.println("B: Guard");
      break;  

    case 'n': case 'N': // --> 푸시업
      setGroupA(90, 65, 105, 90); // --> 다리 약간 펴기
      setGroupB(90, 105, 65, 90);
      setGroupC(165, 95, 45);  // L
      setGroupD(15, 85, 135);    // R
      setGroupE(90);
      delay(1000);
      setGroupC(90, 95, 140);  // --> Down
      setGroupD(90, 85, 40); 
      delay(1000);
      setGroupC(165, 95, 45);   // --> Up
      setGroupD(15, 85, 135); 
      Serial.println("N: Push-Up");
      break;
  
*/  
    
    /* -------------- 각도 초기화 -------------- */

    case '1': // --> 초기 각도
      setGroupA(90, 90, 90);
      setGroupB(90, 90, 90);
      setGroupC(90, 90, 90);  
      setGroupD(90, 90, 90); 
      Serial.println("1: Reset");
      break;

    default:
      // 기타 입력 무시
      break;
  }
}