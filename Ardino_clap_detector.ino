#include <LiquidCrystal.h>
#include <Keyboard.h>

bool typedOnce = false;

int redPinOne = 11;
int bluePinOne = 12;
int greenPinOne = 13;

int redPinTwo = 10;
int bluePinTwo = 9;
int greenPinTwo = 8;
int clapThreshold = 80;
bool unlocked = false;
float A_TIGHT_LO[5] = {0.47f, 0.85f, 1.80f, 2.15f, 0.46f};
float A_TIGHT_HI[5] = {0.60f, 0.98f, 2.10f, 2.35f, 0.53f};

float B_TIGHT_LO[5] = {0.65f, 0.30f, 1.70f, 1.65f, 0.40f};
float B_TIGHT_HI[5] = {1.05f, 0.75f, 3.10f, 2.80f, 0.70f};

#define MAX_CLAPS 128
unsigned long clapTime[MAX_CLAPS];
int clapCount = 0;

unsigned long differences[6];
int diffCount = 0;

float ratios[5];
int ratioCount = 0;

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int micPin = A5;

void setup() {
  pinMode(redPinOne, OUTPUT);
  pinMode(greenPinOne, OUTPUT);
  pinMode(bluePinOne, OUTPUT);

  pinMode(greenPinTwo, OUTPUT);
  pinMode(redPinTwo, OUTPUT);
  pinMode(bluePinTwo, OUTPUT);

  pinMode(micPin, INPUT);
  Serial.begin(9600);

  lcd.begin(16 ,2);
  lcd.print("COMPUTER");
  lcd.setCursor(0, 1);
  lcd.print("LOCKED");
}

void loop() {
  unsigned long t0 = micros();
  int maxV = 0;
  int minV = 1023;
  while((micros() - t0) < 8000UL){
    int micValue = analogRead(micPin);
    if(micValue > maxV){
      maxV = micValue;
    }if(micValue < minV){
      minV = micValue;
    }
  }
  int p2p = maxV - minV;
  if(p2p > 200){
    setColorOne(0,255,0);
    setColorTwo(0, 255, 0);
  }else {
    setColorOne(255, 0, 0);
    setColorTwo(255, 0, 0);
  }
  if(p2p > 200){
    Serial.print(millis());
    Serial.print(' ');
    if (clapCount < MAX_CLAPS) clapTime[clapCount++] = millis();
  }
  if (unlocked) {
    setColorOne(0,0,255);
    setColorTwo(0,0,255);
    lcd.clear();
    lcd.print("COMPUTER");
    lcd.setCursor(0,1);
    lcd.print("UNLOCKED");
    if (!typedOnce) {
    Keyboard.begin();
    delay(200);                 
    Keyboard.print("*type password here*"); 
    Keyboard.write(KEY_RETURN);   
    Keyboard.end();
    typedOnce = true;
    }
  } else {
    if (p2p > 200) {
      setColorOne(0,255,0);
      setColorTwo(0,255,0);
    } else {
      setColorOne(255,0,0);
      setColorTwo(255,0,0);
    }
  }

  diffCount  = 0;   
  ratioCount = 0; 
  int left = 0;
  int right = 1;

  while (right < clapCount) {
      unsigned long df = clapTime[right] - clapTime[left];
      if (df < (unsigned long)clapThreshold) {
          for (int i = right + 1; i < clapCount; ++i) clapTime[i - 1] = clapTime[i];
          --clapCount;
      } else {
          if (diffCount < 6) differences[diffCount++] = df;
          left = right;
          right = left + 1;
          if(diffCount >= 2){
            int ratioSize = (int)ratioCount;

            if (diffCount < ratioSize + 2) {
              continue;
            }

            float ratio = (float)differences[ratioSize + 1]/(float)differences[ratioSize];
            if((ratio >= A_TIGHT_LO[ratioSize] && ratio <= A_TIGHT_HI[ratioSize]) ||  (ratio >= B_TIGHT_LO[ratioSize] && ratio <= B_TIGHT_HI[ratioSize])){
              if (ratioCount < 5) ratios[ratioCount++] = ratio;
              if(ratioCount == 5){
                unlocked = true;
                break;
              }
            }else{
              ratioCount = 0;
              diffCount = 0;
              if (clapCount > 0) {
                for (int i = 1; i < clapCount; ++i) clapTime[i - 1] = clapTime[i];
                --clapCount;
              }

              left = 0;
              right = 1;
            }
          }
      }
  }

}

void setColorOne(int redValue, int greenValue, int blueValue){
  analogWrite(redPinOne, redValue);
  analogWrite(bluePinOne, blueValue);
  analogWrite(greenPinOne, greenValue);
}

void setColorTwo(int redValue, int greenValue, int blueValue){
  analogWrite(redPinTwo, redValue);
  analogWrite(bluePinTwo, blueValue);
  analogWrite(greenPinTwo, greenValue);
}
