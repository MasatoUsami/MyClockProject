/*
自動時計　モーター制御ESP32用コード

setup()
    |
    +-- RS485初期化
    +-- GPIO初期化
    +-- ホーミング
    |
loop()
    |
    +-- receiveCommand()
    |
    +-- updateMotor()
    |
    +-- updateClock()
    |
    +-- sensorCorrection()
*/

// モーター制御用IC　(ULN2003A)
#define IN1 18
#define IN2 19
#define IN3 21
#define IN4 22

// ホールセンサ
#define SENSOR_PIN 34

// RS485
HardwareSerial RS485(2);

#define RXD2 16
#define TXD2 17

//==========================

long currentStep = 0;
long targetStep  = 0;
long totalStep   = 0;

bool isJumping = false;
bool reverse = false;

int currentHour   = 0;
int currentMinute = 0;

float accumulator = 0.0;

unsigned long previousMillis = 0;

const unsigned long interval = 60000;


// モーターシーケンス
const uint8_t seq[4][4] = {

 {1,1,0,0},
 {0,1,1,0},
 {0,0,1,1},
 {1,0,0,1}

};

int stepIndex=0;


// stepMotor()関数
void stepMotor(bool dir)
{

    if(dir)
    {
        stepIndex++;

        if(stepIndex>=4)
            stepIndex=0;
    }
    else
    {
        stepIndex--;

        if(stepIndex<0)
            stepIndex=3;
    }

    digitalWrite(IN1,seq[stepIndex][0]);
    digitalWrite(IN2,seq[stepIndex][1]);
    digitalWrite(IN3,seq[stepIndex][2]);
    digitalWrite(IN4,seq[stepIndex][3]);
}


// UART受信
void receiveCommand()
{

    if(!RS485.available())
        return;

    String s=RS485.readStringUntil('\n');

    char cmd[10];

    int h,m,d;

    sscanf(s.c_str(),"%[^,],%d,%d,%d",
           cmd,&h,&m,&d);

    if(strcmp(cmd,"JUMP")==0)
    {
        startJump(h,m,d);
    }

}


// startJump() 関数
void startJump(int h,int m,bool dir)
{

    reverse=dir;

    int currentTotal=currentHour*60+currentMinute;

    int targetTotal=h*60+m;

    int diff=targetTotal-currentTotal;

    if(!dir)
    {
        if(diff<0)
            diff+=720;
    }
    else
    {
        if(diff>0)
            diff-=720;

        diff=-diff;
    }

    targetStep=totalStep+diff*200L/60L;

    currentHour=h;
    currentMinute=m;

    isJumping=true;

}


// updateMotor() 関数
void updateMotor()
{

    static unsigned long last=0;

    unsigned long now=millis();

    if(now-last<20)
        return;

    last=now;

    float stepRate;

    if(isJumping)
        stepRate=35.0;
    else
        stepRate=200.0/3600.0;

    accumulator+=stepRate*0.02;

    while(accumulator>=1.0)
    {
        accumulator-=1.0;

        stepMotor(reverse);

        totalStep++;

        currentStep++;

        if(currentStep>=200)
            currentStep=0;

        if(isJumping)
        {
            if(totalStep>=targetStep)
            {
                isJumping=false;

                accumulator=0;

                Serial.println("Jump Finish");
            }
        }
    }

}


// センサー補正　--ホールセンサー：１２時付近だけ
void sensorCorrection()
{

    if(currentStep>195 || currentStep<5)
    {
        if(digitalRead(SENSOR_PIN)==LOW)
        {
            currentStep=0;
        }
    }

}


// setup() 
void setup()
{

    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(IN3,OUTPUT);
    pinMode(IN4,OUTPUT);

    pinMode(SENSOR_PIN,INPUT_PULLUP);

    Serial.begin(115200);

    RS485.begin(9600,SERIAL_8N1,RXD2,TXD2);

}


// loop()
void loop()
{

    receiveCommand();

    updateMotor();

    sensorCorrection();

}
