
#include <TimedAction.h>
#include<LiquidCrystal.h>
#include<Servo.h> 
Servo motor1, motor2;
bool isallslotedFull=false;
LiquidCrystal lcd(12,11,5,4,3,2);

const int trigPin1 = 22;
const int echoPin1 = 23;
const int trigPin2 = 24;
const int echoPin2 = 25;
long duration;
int distance01,distance02;
const int maxspace = 5;
const int minspace = 3;

byte cf[8] = {
  0b00000,
  0b00011,
  0b00100,
  0b11000,
  0b10000,
  0b11111,
  0b00110,
  0b00000
};
byte cb[8] = {
  0b00000,
  0b10000,
  0b01000,
  0b01100,
  0b00010,
  0b11110,
  0b01100,
  0b00000
};
byte pp[8] = {
  0b00000,
  0b01110,
  0b01010,
  0b01010,
  0b01110,
  0b01000,
  0b01000,
  0b00000
};


 void closeGate01(){
  if(digitalRead(20)!= LOW){
    //delay(500);
    motor1.write(93); 
    }
}
TimedAction Closegate01Thread = TimedAction(300,closeGate01);
 void closeGate02(){
  if(digitalRead(21)!= LOW){
    //delay(500);
    motor2.write(93);  
    }
}
TimedAction Closegate02Thread = TimedAction(300,closeGate02);


 void warPark03(){
  if(distance01 <= 3 && distance01 >=0){
    digitalWrite(26,HIGH); 
    //delay(100);
  }else{
  digitalWrite(26,LOW); 
  }
 } 

TimedAction park03WarrThread = TimedAction(100,warPark03);
 
 void warPark04(){
  if(distance02 <= 3 && distance02 >=0){
    digitalWrite(27,HIGH); 
    delay(100);    
  }else{
    digitalWrite(27,LOW);
  } 
 }

TimedAction park04WarrThread = TimedAction(100,warPark04);


void ultrasonic(){
// Clears the trigPin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin1, HIGH);
  // Calculating the distance
  distance01 = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.println("Distance 1: ");
  Serial.println(distance01);
  
   // Clears the trigPin
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin2, HIGH);
  // Calculating the distance
  distance02 = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.println("Distance 2: ");
  Serial.println(distance02);
  park03WarrThread.check();
  park04WarrThread.check();
}

TimedAction ultraSonicThread = TimedAction(50,ultrasonic);

 void gate01(){
 if(digitalRead(20)== LOW && !isallslotedFull){
     lcd.clear();
     Serial.println("front");
     digitalWrite(13,HIGH); 
     lcd.print("Front Gate Open");
     motor1.write(153);
     ultraSonicThread.check();
     Closegate01Thread.check();
     //delay(1000);
     lcd.clear();
 }
    delay(500);    
    ultraSonicThread.check();
    Closegate01Thread.check();
  }

TimedAction motor01Thread = TimedAction(1000,gate01);
 
 void gate02(){
 if(digitalRead(21)== LOW){
    lcd.clear();
    digitalWrite(13,HIGH); 
    lcd.print("Rear Gate Open");
    motor2.write(153); 
    ultraSonicThread.check();
    Closegate02Thread.check();
    //delay(1000);
    lcd.clear();
  }
    delay(500);
    ultraSonicThread.check();
    Closegate02Thread.check();
 }

TimedAction motor02Thread = TimedAction(1000,gate02);

void parking_slot01(){
  
  if (digitalRead(18)!= LOW )
  { 
    digitalWrite(13,HIGH); 
    lcd.setCursor(0,1);
    lcd.print("01 ");
    ultraSonicThread.check();
    motor01Thread.check();
    motor02Thread.check();
    ultraSonicThread.check();
    //delay(500);
 }
}
 void parking_slot02(){
  
  if(digitalRead(19)!= LOW ){
    digitalWrite(13,HIGH); 
    lcd.setCursor(3,1);
    lcd.print("02 ");
    ultraSonicThread.check();
    motor01Thread.check();
    motor02Thread.check();
    ultraSonicThread.check();
    delay(500);
  }
 }


 void specialParking_slot03(){
 if(distance01 >5 ){
    digitalWrite(13,HIGH); 
    lcd.setCursor(6,1);
    lcd.print("03 ");
    ultraSonicThread.check();
    motor01Thread.check();
    motor02Thread.check();
    ultraSonicThread.check();
    delay(500);
  }
  ultraSonicThread.check();
 }

 void specialParking_slot04(){
 if(distance02 >5 ){
    digitalWrite(13,HIGH); 
    lcd.setCursor(9,1);
    lcd.print("04 ");
    ultraSonicThread.check();
    motor01Thread.check();
    motor02Thread.check();
    ultraSonicThread.check();
    delay(500);
  }
    ultraSonicThread.check();
 }


void setup() {

  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);

  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  
  motor1.attach(14);
  motor2.attach(15);
  pinMode(13,OUTPUT);
  pinMode(18,INPUT);
  pinMode(19,INPUT);
  pinMode(20,INPUT);
  pinMode(21,INPUT);
  Serial.begin(9600);
  
  lcd.createChar(0,cf);
  lcd.createChar(1,cb);
  lcd.createChar(2,pp);
  lcd.begin(1, 1);                
  lcd.print("  Raspberry 05");
  delay(1000);
  lcd.begin(2, 2);  
  lcd.setCursor(0,0);
   lcd.write(byte(2));
  lcd.print("   Welcome   ");
  lcd.write(byte(0));
  lcd.write(byte(1));
  delay(1000);
  
  for(int i=0;i<2;i++){
  lcd.setCursor(0,2);
  lcd.print(" Parking Master");
  delay(1000);
  lcd.setCursor(0,2);
  lcd.print("                      ");
  delay(500);
  } 
  ultrasonic();
  motor01Thread.check();
  motor02Thread.check();
}

void loop() {  
//  ultraSonicThread.check();
  lcd.clear();
  digitalWrite(13,LOW);
  if(digitalRead(18)!= LOW && digitalRead(19)!= LOW && distance01 >5 && distance02 >5 ) 
  {
     ultraSonicThread.check();
     isallslotedFull = false;
     motor01Thread.check();
     motor02Thread.check();
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("All Slots Empty");
     
  }else if(digitalRead(18)!= LOW || digitalRead(19)!= LOW || distance01 >5 || distance02 >5){
    isallslotedFull = false;
    ultraSonicThread.check();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Avalible Slots :");
    parking_slot01();
    parking_slot02();  
    specialParking_slot03();
    specialParking_slot04();
    lcd.setCursor(0,1);
    lcd.print("");
     motor01Thread.check();
     motor02Thread.check();
     ultraSonicThread.check();
  }else{
  isallslotedFull = true;
  ultraSonicThread.check();
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("All Slots Full");
   motor01Thread.check();
   motor02Thread.check();
   ultraSonicThread.check();

  }
  digitalWrite(13,LOW);
} 
