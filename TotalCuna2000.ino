#include <Servo.h>

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

int audioppp=0;

void setup()
{
   pinMode(trigger, OUTPUT); /*activación del pin 9 como salida: para el pulso ultrasónico*/
   pinMode(echo, INPUT); /*activación del pin 8 como entrada: tiempo del rebote del ultrasonido*/
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
  //Serial.println(audioppp);
  if(isNoisy() && isMoving()){
    mecer(5);delay(5000);}
    delay(150);
  
  
   
}

bool isNoisy(){

   bool audioFlag=false;
   int testValue = 0;
   testValue=analogRead(audioPin);
   if((testValue > audioAverage + audioThreshold)||((testValue < audioAverage - audioThreshold))){
   audioFlag=true;
   }

   audioppp=testValue;
   
   return audioFlag;
  
  }

void setAudioAverage(int cycles){

  int total=0;
  int temporalValue=0;
  
  for(int i=0;i<cycles;i++){
   temporalValue=analogRead(audioPin);
   total = total + temporalValue;
   }
   audioAverage = total/cycles; 
  
  }

  bool isMoving(){

   bool movingFlag=false;
   int testDistance = distanceValue(7);
   if(testDistance>0){
   if(((testDistance>sleepDistance + cradleThreshold)||(testDistance<sleepDistance - cradleThreshold)) && (testDistance < emptyCradleDistance))
   {movingFlag=true;}}
   return movingFlag;
  
  }

bool isEmptyCradle(){

   bool emptyFlag=false;
   int testDistance = distanceValue(7);
   if((testDistance>emptyCradleDistance)&&testDistance>0)
   {emptyFlag=true;}
   return emptyFlag;
  
  }

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

int getSensor(){
  digitalWrite(trigger,LOW); /* Por cuestión de estabilización del sensor*/
  delayMicroseconds(5);
  digitalWrite(trigger, HIGH); /* envío del pulso ultrasónico*/
  delayMicroseconds(10);
  int timeValue=pulseIn(echo, HIGH); /* Función para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envío
  del pulso ultrasónico y cuando el sensor recibe el rebote, es decir: desde que el pin 12 empieza a recibir el rebote, HIGH, hasta que
  deja de hacerlo, LOW, la longitud del pulso entrante*/
  int distanceValue= int(0.017*timeValue); /*fórmula para calcular la distancia obteniendo un valor entero*/
  return distanceValue;
  }


void mecer(int cycles){
  
  int i=0;
  int pos=0;
  while(i<cycles){


for (pos = 90; pos <= 115; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 115; pos >= 65; pos -= 1) { // goes from 180 degrees to 0 degrees
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 65; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(35);                       // waits 15ms for the servo to reach the position
  }

i++;
  }

  
  }

