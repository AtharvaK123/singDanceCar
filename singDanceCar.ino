#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define WS2812_PIN 5
#define WS2812_COUNT  12
#define BRIGHTNESS 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(WS2812_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include<Servo.h>
Servo myservo;

#include "pitches.h"

#define LED_R 6
#define LED_L 9

bool right = false, left = false, fast = false, slow = false;

int tempo = 108;

int buzzer = 11;

int rgbArray[3][3][3] = {
  {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}},
  {{255, 150, 0}, {0, 255, 255}, {180, 0, 255}},
  {{192, 192, 192}, {128, 128, 128}, {255, 165, 0}}
};

int colorNumR = 0, colorNumC = 0;

int melody[] = {
  
  NOTE_AS4,8, NOTE_AS4,8, NOTE_AS4,8,//1
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,8, NOTE_C5,8, NOTE_C5,8,
  NOTE_F5,2, NOTE_C6,2,
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4,  
  
  NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F6,2, NOTE_C6,4, //8  
  NOTE_AS5,8, NOTE_A5,8, NOTE_AS5,8, NOTE_G5,2, NOTE_C5,-8, NOTE_C5,16, 
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C5,-8, NOTE_C5,16,
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  
  NOTE_C6,-8, NOTE_G5,16, NOTE_G5,2, REST,8, NOTE_C5,8,//13
  NOTE_D5,-4, NOTE_D5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
  NOTE_F5,8, NOTE_G5,8, NOTE_A5,8, NOTE_G5,4, NOTE_D5,8, NOTE_E5,4,NOTE_C6,-8, NOTE_C6,16,
  NOTE_F6,4, NOTE_DS6,8, NOTE_CS6,4, NOTE_C6,8, NOTE_AS5,4, NOTE_GS5,8, NOTE_G5,4, NOTE_F5,8,
  NOTE_C6,1
  
};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;

int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void setup() {
  pinMode(LED_R,OUTPUT);  
  pinMode(LED_L,OUTPUT);  

  myservo.attach(10);
  myservo.write(10);
  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);
  left = true;

  lcd.init(); 
  lcd.backlight();
  lcd.clear();

  lcd.setCursor(3,0);
  lcd.print("Star  Wars");
  lcd.setCursor(3,1); 
  lcd.print("Theme Song"); 

  delay(10000);

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    divider = melody[thisNote + 1];
    if (divider > 0) {
      slow =  true;
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      fast = true;
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5;
    }
    tone(buzzer, melody[thisNote], noteDuration*0.9);

    if(left == true){
      myservo.write(100);
      digitalWrite(LED_R,HIGH);
      digitalWrite(LED_L,HIGH);
      delay(20);
      digitalWrite(LED_R,LOW);
      digitalWrite(LED_L,LOW);

      right = true;
      left = false;
      fast = false;
      slow = false;
    }
    else if(right == true){
      myservo.write(10);
      
      digitalWrite(LED_R,HIGH);
      digitalWrite(LED_L,HIGH);
      delay(20);
      digitalWrite(LED_R,LOW);
      digitalWrite(LED_L,LOW);
      right = false;
      left = true;
      fast = false;
      slow = false;
    }

    uint32_t color = Color(rgbArray[colorNumR][colorNumC][0],
                            rgbArray[colorNumR][colorNumC][1],
                            rgbArray[colorNumR][colorNumC][2]);
      colorWipe(color);

      if(colorNumC == 2){
        colorNumR++;
        colorNumC = 0;
      }
      else if(colorNumR == 2){
        colorNumR = 0;
        colorNumC = 0;
      }
      else if(colorNumC < 2){
        colorNumC++;
      }

    delay(noteDuration);
    
    noTone(buzzer);
  }
}

void loop() {
}

uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
}


void colorWipe(uint32_t c) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);                 
    strip.show();                           
  }
}