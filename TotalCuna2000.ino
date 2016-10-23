#include <Servo.h>

//Set initial values
int audioPin=A0;
int audioThreshold=3;
int audioAverage=0;

int sleepDistance=15;
int trigger=9;
int echo=8;
int emptyCradleDistance=20;
int cradleThreshold = 2;

Servo myServo;
int angle=90;
int servoPin=10;

int audio69=0;

//init 
void setup()
{
   pinMode(trigger, OUTPUT); 
   pinMode(echo, INPUT); 
   Serial.begin(9600);
   myServo.attach(servoPin);
   delay(400);
   setAudioAverage(15);
}

void loop()
{

  //if(isEmptyCradle()){Serial.println("Cuna vacía");}
  //if(isMoving()){Serial.println("Se detecto movimiento");}
  //if(isNoisy()){Serial.print("Audio Threshold destroyed ");Serial.println(audioAverage);}
  //Serial.print(audioAverage);
  //Serial.print("  ---   value: ");
  //Serial.println(audio69);

  //if brat is awakem screaming and moving the crib swings
  if(isNoisy() && isMoving()){
    swing(5);delay(5000);}
    delay(150);
  
   
}

//detect noise from the crib
bool isNoisy(){

   bool audioFlag=false;
   int testValue = 0;
   testValue=analogRead(audioPin);
   if((testValue > audioAverage + audioThreshold)||((testValue < audioAverage - audioThreshold))){
   audioFlag=true;
   }
   //audioppp=testValue;
   return audioFlag;
  }

//set the average noise from the room to detect the baby's noise
void setAudioAverage(int cycles){

  int total=0;
  int temporalValue=0;
  for(int i=0;i<cycles;i++){
   temporalValue=analogRead(audioPin);
   total = total + temporalValue;
   }
   audioAverage = total/cycles; 
  }

//detect baby's movements
  bool isMoving(){

   bool movingFlag=false;
   int testDistance = distanceValue(7);
   if(testDistance>0){
   if(((testDistance>sleepDistance + cradleThreshold)||(testDistance<sleepDistance - cradleThreshold)) && (testDistance < emptyCradleDistance))
   {movingFlag=true;}}
   return movingFlag;
  
  }

//detect if the crib is empty
bool isEmptyCradle(){

   bool emptyFlag=false;
   int testDistance = distanceValue(7);
   if((testDistance>emptyCradleDistance)&&testDistance>0)
   {emptyFlag=true;}
   return emptyFlag;
  
  }

//get a filtered value 
int distanceValue(int cycles){
  
  int k=0;
  int brake=0;
  int temporalValue=0;
  int total=0;
  while(k<cycles)
  {
    temporalValue = getSensor();
    if(temporalValue > 5 && temporalValue < 40)
    {total = total + temporalValue;k++;brake=0;}
    else{
      
      if(brake==20){break;}
      else{total=0;brake++;}
      
      }
    delay(20);
    
  }
  int result = total/cycles;
  return result;
  
  }

//get a unfiltered value
int getSensor(){
  digitalWrite(trigger,LOW); 
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH); 
  delayMicroseconds(10);
  int timeValue=pulseIn(echo, HIGH); 
  int distanceValue= int(0.017*timeValue); 
  return distanceValue;
  }


void swing(int cycles){
  
  int i=0;
  int pos=0;
  while(i<cycles){

for (pos = 90; pos <= 115; pos += 1) { 
    myServo.write(pos);             
    delay(35);                       
  }
  for (pos = 115; pos >= 65; pos -= 1) { 
    myServo.write(pos);             
    delay(35);                       
  }
  for (pos = 65; pos <= 90; pos += 1) { 
    myServo.write(pos);              
    delay(35);                       
  }
i++;
  }
  }

