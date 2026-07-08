/*
自動時計　ターゲット時間設定、実行用スイッチボックスのためのコード

---ディスプレイ---
OLED SDA → GPIO21
OLED SCL → GPIO22

--- ロータリーエンコーダー ---
ENC_A → GPIO32
ENC_B → GPIO33
ENC_SW → GPIO25

--- モード切替ボタン ---
MODE_BTN → GPIO26

--- シーン実行ボタン ---
SCENE1 → GPIO27
SCENE2 → GPIO14
SCENE3 → GPIO12
SCENE4 → GPIO13

--- UART通信 ---
UART TX → GPIO17（→相手RX）
UART RX → GPIO16（未使用でもOK）
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== UART =====
HardwareSerial mySerial(2);

// ===== ピン =====
#define ENC_A 32
#define ENC_B 33
#define ENC_SW 25

#define BTN_MODE 26

#define BTN_S1 27
#define BTN_S2 14
#define BTN_S3 12
#define BTN_S4 13

// ===== モード =====
enum Mode {
  RUN,
  SELECT_SCENE,
  EDIT_HOUR,
  EDIT_MINUTE,
  EDIT_DIR
};

Mode mode = RUN;

// ===== シーン =====
struct Scene {
  int hour;
  int minute;
  bool reverse;
};

Scene scenes[4];

// ===== 状態 =====
int selectedScene = 0;
int editHour = 0;
int editMinute = 0;
bool editReverse = false;

// ===== エンコーダー =====
int lastA = HIGH;

// ===== デバウンス =====
unsigned long lastBtnTime = 0;
bool btnPressed(int pin) {
  if (digitalRead(pin) == LOW && millis() - lastBtnTime > 200) {
    lastBtnTime = millis();
    return true;
  }
  return false;
}

// ===== エンコーダー処理 =====
int readEncoder() {
  int a = digitalRead(ENC_A);
  int b = digitalRead(ENC_B);

  int val = 0;

  if (a != lastA) {
    if (b != a) val = 1;
    else val = -1;
  }

  lastA = a;
  return val;
}

// ===== UART送信 =====
void sendScene(int i) {
  char buf[16];
  sprintf(buf, "%d,%d,%d\n", scenes[i].hour, scenes[i].minute, scenes[i].reverse);
  mySerial.print(buf);
}

// ===== 描画 =====
void drawUI() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  if (mode == RUN) {
    display.setCursor(0,0);
    display.println("RUN");

    for (int i = 0; i < 4; i++) {
      display.setCursor(0, 10 + i * 12);
      display.printf("S%d %02d:%02d %s",
        i+1,
        scenes[i].hour,
        scenes[i].minute,
        scenes[i].reverse ? "<-" : "->");
    }
  }

  else {
    display.setCursor(0,0);
    display.printf("SET S%d", selectedScene + 1);

    display.setCursor(0,20);
    display.printf("H:%02d", editHour);

    display.setCursor(0,35);
    display.printf("M:%02d", editMinute);

    display.setCursor(0,50);
    display.printf("DIR:%s", editReverse ? "<-" : "->");
  }

  display.display();
}

// ===== セットアップ =====
void setup() {

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  pinMode(BTN_MODE, INPUT_PULLUP);

  pinMode(BTN_S1, INPUT_PULLUP);
  pinMode(BTN_S2, INPUT_PULLUP);
  pinMode(BTN_S3, INPUT_PULLUP);
  pinMode(BTN_S4, INPUT_PULLUP);

  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, 16, 17);

  Wire.begin(21,22);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  // 初期値
  for(int i=0;i<4;i++){
    scenes[i] = {0,0,false};
  }
}

// ===== ループ =====
void loop() {

  int enc = readEncoder();

  // ===== MODE =====
  if (btnPressed(BTN_MODE)) {
    mode = SELECT_SCENE;
    selectedScene = 0;
  }

  // ===== RUNモード =====
  if (mode == RUN) {

    if (btnPressed(BTN_S1)) sendScene(0);
    if (btnPressed(BTN_S2)) sendScene(1);
    if (btnPressed(BTN_S3)) sendScene(2);
    if (btnPressed(BTN_S4)) sendScene(3);
  }

  // ===== シーン選択 =====
  if (mode == SELECT_SCENE) {

    selectedScene += enc;
    if (selectedScene < 0) selectedScene = 3;
    if (selectedScene > 3) selectedScene = 0;

    if (btnPressed(ENC_SW)) {
      editHour = scenes[selectedScene].hour;
      editMinute = scenes[selectedScene].minute;
      editReverse = scenes[selectedScene].reverse;
      mode = EDIT_HOUR;
    }
  }

  // ===== 時設定 =====
  else if (mode == EDIT_HOUR) {

    editHour += enc;
    if (editHour < 0) editHour = 11;
    if (editHour > 11) editHour = 0;

    if (btnPressed(ENC_SW)) mode = EDIT_MINUTE;
  }

  // ===== 分設定 =====
  else if (mode == EDIT_MINUTE) {

    editMinute += enc * 5;
    if (editMinute < 0) editMinute = 55;
    if (editMinute > 55) editMinute = 0;

    if (btnPressed(ENC_SW)) mode = EDIT_DIR;
  }

  // ===== 方向 =====
  else if (mode == EDIT_DIR) {

    if (enc != 0) editReverse = !editReverse;

    if (btnPressed(ENC_SW)) {
      scenes[selectedScene] = {editHour, editMinute, editReverse};
      mode = RUN;
    }
  }

  drawUI();
}