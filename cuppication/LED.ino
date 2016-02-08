

//　LEDリボンの初期設定
#include <Adafruit_NeoPixel.h>
#define LEDmax 5
#define LEDpin 2
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDmax,LEDpin,NEO_GRB + NEO_KHZ800);
int LEDcolorR[LEDmax],LEDcolorG[LEDmax], LEDcolorB[LEDmax];
int LEDintensity; 
int LEDmaxintensity = 100;


void LEDinit(){  //LEDリボンinit
  LEDintensity = LEDmaxintensity; 
  for(int i=0; i<LEDmax; i++) {
    setLEDintensity(i, LEDintensity);
  }
  strip.begin();
}


//LEDの色をセット
void setLEDintensity(int LEDpos, int LEDval) {
  LEDcolorR[LEDpos]=255; //LEDval;
  LEDcolorG[LEDpos]=255; //LEDval;
  LEDcolorB[LEDpos]=0; //LEDval;
}

void setLED(boolean a){

}


void onLED(){

  for(int i=0; i < LEDmax; i++) {
    strip.setPixelColor(i,strip.Color(LEDcolorR[i],LEDcolorG[i],LEDcolorB[i]));
  } 
  strip.show();

}

void offLED(){
  for(int i=0; i < LEDmax; i++) { 
    strip.setPixelColor(i,strip.Color(0,0,0));
  } 
  strip.show(); 

}

