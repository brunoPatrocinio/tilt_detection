/*
 * Author: Bruno A Patrocinio
 * Version : 3.1.0
 */
#include <Ultrasonic.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

//pin ultrassound sensor HC04
#define trigger 5
#define echo 4

//constants pin led and buzzer
const int pinoLedRed1 = 3;
const int pinoLedRed2 = 2; 
const int pinoLedGreen = 6;
const int pinoLedYellow = 7;
const int pinoLedBlue = 12;
const int pinoSom = 13;

//prototype
void updateAcell();
void engagePrint();
void printData();
void orientation();
void isThereGlass();

//global vars
unsigned long timer = 0;
boolean ligado = false;
boolean isGlass = false;
float distanceCentimeters;
long bounceTimer;

MPU6050 acell(Wire); //obj do tipo MPU6050
Ultrasonic ultra(trigger, echo);

void setup() {
 Serial.begin(38400);
 //initial configs
 pinMode(trigger, OUTPUT);                                 
 digitalWrite(trigger, LOW);  
 pinMode(echo, INPUT);

 pinMode(pinoLedRed1, OUTPUT); 
 pinMode(pinoLedRed2, OUTPUT); 
 pinMode(pinoLedGreen, OUTPUT); 
 pinMode(pinoLedYellow, OUTPUT);
 pinMode(pinoLedBlue, OUTPUT);
 pinMode(pinoSom, OUTPUT); 

 //initial leds config
 digitalWrite(pinoLedRed1, LOW);
 digitalWrite(pinoLedRed2, LOW);
 digitalWrite(pinoLedYellow, LOW);
 digitalWrite(pinoLedBlue, LOW);
 digitalWrite(pinoLedGreen, HIGH);

 Wire.begin();
 acell.begin();
 //calculate the offset values
 acell.calcGyroOffsets(true);

}//end of setup

void loop() {
 updateAcell(); //call the function
 isThereGlass();
 orientation();
 engagePrint();
 
}// end of loop

void updateAcell(){
  acell.update(); //update the sensor data
  
}//end of updateAcell

void engagePrint(){
  if(Serial.available()){
    char c = Serial.read();
    if(c){
     switch(c){
      case 'l':
        ligado = true;
        break;
      case 'd':
        ligado = false;
        break;
      default:
        break;
     }
      c = NULL;
    }
    
  }

  if(ligado == true){ 
     printData();
  }else{
    //nada
  }

}//end of engagePrint

void printData(){
  int rawDataZ = acell.getRawAccZ();
  if(millis() - timer > 1000){
    
    Serial.println("=======================================================");
    Serial.print("accX : ");Serial.print(acell.getAccX());
    Serial.print("\tRaw Z: ");Serial.print(rawDataZ);
    Serial.print("\taccY : ");Serial.print(acell.getAccY());
    Serial.print("\taccZ : ");Serial.println(acell.getAccZ());
    Serial.print("gyroX : ");Serial.print(acell.getGyroX());
    Serial.print("\tgyroY : ");Serial.print(acell.getGyroY());
    Serial.print("\tgyroZ : ");Serial.println(acell.getGyroZ());
    Serial.print("accAngleX : ");Serial.print(acell.getAccAngleX());
    Serial.print("\taccAngleY : ");Serial.println(acell.getAccAngleY());
    Serial.print("gyroAngleX : ");Serial.print(acell.getGyroAngleX());
    Serial.print("\tgyroAngleY : ");Serial.print(acell.getGyroAngleY());
    Serial.print("\tgyroAngleZ : ");Serial.println(acell.getGyroAngleZ());
    Serial.print("angleX : ");Serial.print(acell.getAngleX());
    Serial.print("\tangleY : ");Serial.print(acell.getAngleY());
    Serial.print("\tangleZ : ");Serial.println(acell.getAngleZ());
    Serial.print("Dist. Cent: ");Serial.print(distanceCentimeters);
    Serial.print("\tIs There a Glass? ");Serial.println(isGlass);
    Serial.println("=======================================================\n");
    timer = millis();
    
  }
}//end of printData

void orientation(){
  float angleX = acell.getAngleX();
  float angleY = acell.getAngleY();
  int rawDataZ = acell.getRawAccZ();
  
 if(rawDataZ >= 12000 && isGlass == true){
    if(angleX > 23.50){      
        digitalWrite(pinoLedRed1, HIGH);
        tone(pinoSom, 3000, 300); //speaker apita 
        digitalWrite(pinoLedGreen, LOW);                   
    }
    else if(angleX < 6.30){   
        digitalWrite(pinoLedRed2, HIGH);       
        tone(pinoSom, 3000, 300); //speaker apita
        digitalWrite(pinoLedGreen, LOW);                         
    }
    else{
      digitalWrite(pinoLedRed1, LOW); // deixa o pino dos leds em LOW
      digitalWrite(pinoLedRed2, LOW);
      digitalWrite(pinoLedGreen, HIGH);
      noTone(pinoSom);
    }
    
  }
  else{
      digitalWrite(pinoLedRed1, LOW); // deixa o pino dos leds em LOW
      digitalWrite(pinoLedRed2, LOW);
      digitalWrite(pinoLedGreen, HIGH);
      noTone(pinoSom);
  }
    
}//end of orientation

void isThereGlass(){  
  long microseg = ultra.timing();
  distanceCentimeters = ultra.convert(microseg,Ultrasonic::CM);
  if(distanceCentimeters < 8.00){
    isGlass = true;
    digitalWrite(pinoLedYellow, HIGH);
  }
  else{
    isGlass = false;
    digitalWrite(pinoLedYellow,LOW);
  }
}//end of isThereGlass
