#define c4 554
#define e4 659
#define a3 440
#define b3 494
#define g4 831
#define a4 880

int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int melody[] = 
{
  a3,a3,a3,b3,a3,b3,c4,e4
};

int noteDurations[] =
{
  4,4,4,4,4,4,4
};
int thisNote = -1,noteDuration = 0;
long previousTime = 0,
     presentTime = 0,
     pauseBetweenNotes = 0;

void setup() {
  pinMode(latchPin,OUTPUT);
  pinMode(clockPin,OUTPUT);
  pinMode(dataPin,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(2,INPUT_PULLUP);
}

void checkToPlay(){

  presentTime = millis();
  if(presentTime-previousTime>=pauseBetweenNotes){
    thisNote+=1;
    if(thisNote>=8){
      thisNote=-1;
      pauseBetweenNotes=100;
      previousTime=millis();
    }else{
      noteDuration=500/noteDurations[thisNote];
      tone(9,melody[thisNote],noteDuration);
      pauseBetweenNotes=noteDuration*1.2;
      previousTime = millis();
    }
  }
    
}




byte colDataMatrix[8] = {
   B11111110,
   B11111101,
   B11111011,
   B11110111,
   B11101111,
   B11011111,
   B10111111,
   B01111111
}; //掃描
  

byte rowDataMatrix[8] = {
   B00000000,
   B00011000,
   B00111100,
   B01111110,
   B11111111,
   B01100110,
   B00011000,
   B01111110
}; //圖案
byte row1DataMatrix[8] = {
   B00000000,
   B00000000,
   B00111100,
   B00111100,
   B00111100,
   B00000000,
   B00000000,
   B00000000
}; //圖案


byte previous_buttonState=1,present_buttonState=1,patternNumber=0;

void loop() {
 
  present_buttonState=digitalRead(2);
  if( present_buttonState==0 && previous_buttonState==1){
      patternNumber++; 
      if(patternNumber>1) patternNumber=0;
  }
 if( patternNumber==0){
        for(int i = 0; i < 8 ; i++){
    	digitalWrite(latchPin,LOW);
    	shiftOut(dataPin, clockPin, MSBFIRST, colDataMatrix[i]);
    	shiftOut(dataPin, clockPin, MSBFIRST, rowDataMatrix[i]);
    	digitalWrite(latchPin,HIGH);
        checkToPlay();
    	delay(1);
    	}
  }
   if(patternNumber==1){
      for(int i = 0; i < 8 ; i++){
   	digitalWrite(latchPin,LOW);
    	shiftOut(dataPin, clockPin, MSBFIRST, colDataMatrix[i]);
    	shiftOut(dataPin, clockPin, MSBFIRST, row1DataMatrix[i]);
    	digitalWrite(latchPin,HIGH);
       checkToPlay();
    	delay(1);
    	}
   }
   
  
  delay(2);
  
  previous_buttonState= present_buttonState;
  
}