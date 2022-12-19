#include <TimedAction.h>
#include<LiquidCrystal.h>
#include<Servo.h>
Servo motor1, motor2, motor3, motor4;
bool isallslotedFull = false;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String command;
const int trigPin1 = 22;
const int echoPin1 = 23;
const int trigPin2 = 24;
const int echoPin2 = 25;
long duration1, duration2;
int distance01, distance02;
int redDistance = 4;
int maxDistance = 8;
const int maxspace = 5;
const int minspace = 3;

int slotA, slotB, slotC, slotD;
int bookC,bookD,gateC,gateD;

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


void closeGate01() {
  if (digitalRead(20) != LOW) {
    delay(1000);
    motor1.write(93);
    digitalWrite(13, LOW);
  }
}
TimedAction Closegate01Thread = TimedAction(300, closeGate01);

void closeGate02() {
  if (digitalRead(21) != LOW) {
    delay(1000);
    motor2.write(93);
    digitalWrite(13, LOW);
  }
}
TimedAction Closegate02Thread = TimedAction(300, closeGate02);

void warPark03() {
  if (distance01 <= redDistance && distance01 >= 0) {
    digitalWrite(26, HIGH);
  } else {
    digitalWrite(26, LOW);
  }
}

TimedAction park03WarrThread = TimedAction(100, warPark03);

void warPark04() {
  if (distance02 <= redDistance && distance02 >= 0) {
    digitalWrite(27, HIGH);
  } else {
    digitalWrite(27, LOW);
  }
}

TimedAction park04WarrThread = TimedAction(100, warPark04);


void ultrasonic() {
    // Clears the trigPin
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration1 = pulseIn(echoPin1, HIGH);

  // Clears the trigPin
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
  
  // Calculating the distance
  distance01 = duration1 * 0.034 / 2;
  distance02 = duration2 * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  park03WarrThread.check();
  park04WarrThread.check();
  
//  Serial1.print("disC - ");
//  Serial1.println(distance01);
//  Serial1.print("disD - ");
//  Serial1.println(distance02);
}

TimedAction ultraSonicThread = TimedAction(300, ultrasonic);

void gate01() {
  if (digitalRead(20) == LOW && !isallslotedFull) {
    lcd.clear();
    digitalWrite(13, HIGH);
    lcd.print("Front Gate Open ");
    motor1.write(153);
    delay(1000);
    ultraSonicThread.check();
    Closegate01Thread.check();
  }else{
  ultraSonicThread.check();
  Closegate01Thread.check();
  }
}

TimedAction motor01Thread = TimedAction(1000, gate01);

void gate02() {
  if (digitalRead(21) == LOW) {
    lcd.clear();
    digitalWrite(13, HIGH);
    lcd.print("Rear Gate Open  ");
    motor2.write(153);
    delay(1000);
    ultraSonicThread.check();
    Closegate02Thread.check();
  }else{
  ultraSonicThread.check();
  Closegate02Thread.check();
  }
}

TimedAction motor02Thread = TimedAction(1000, gate02);

void Cgate() {
  if (gateC == 1) {
    motor3.write(153);
    //delay(1000);
  }else{
    motor3.write(93);
  }
}
TimedAction motor03Thread = TimedAction(1000, Cgate);

void Dgate() {
  if (gateD == 1) {
    motor4.write(153);
    //delay(1000);
  }else{
    motor4.write(93);
  }
}
TimedAction motor04Thread = TimedAction(1000, Dgate);

void parking_slot01() {

  if (digitalRead(18) != LOW ) {
    digitalWrite(13, LOW);
    //  upload avalability to RASPBERRYPI
    slotA = 0;
    ultraSonicThread.check();
    motor01Thread.check();
    motor02Thread.check();
  }else{
    slotA = 1;
  }
}
void parking_slot02() {

  if (digitalRead(19) != LOW ) {
    digitalWrite(13, LOW);
    //  upload avalability to RASPBERRYPI
    slotB = 0;
    ultraSonicThread.check();
    motor01Thread.check();
    motor02Thread.check();
  }else{
    slotB = 1;
  }
}


void specialParking_slot03() {
  if (distance01 > maxDistance && bookC == 0) { // have to add a bollean about reseravation. and when its true then make it false in if statment here.
    //digitalWrite(13, HIGH);
    //  upload avalability to RASPBERRYPI
    slotC = 0;
    motor01Thread.check();
    motor02Thread.check();
  }else{
    slotC = 1;
  }
}

void specialParking_slot04() {
  if (distance02 > maxDistance && bookD == 0 ) {// have to add a bollean about reseravation. and when its true then make it false in if statment here.
    //digitalWrite(13, HIGH);
    //  upload avalability to RASPBERRYPI
    slotD = 0;
    motor01Thread.check();
    motor02Thread.check();
  }else{
    slotD = 1;
  }
}


void setup() {

  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);

  pinMode(trigPin1, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin2, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication


  motor1.attach(14);
  motor2.attach(15);
  motor3.attach(16);
  motor4.attach(8);
  
  pinMode(13, OUTPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  pinMode(20, INPUT);
  pinMode(21, INPUT);

   slotA = 0; 
   slotB = 0;
   slotC = 0;
   slotD = 0;
   bookC = 0;
   bookD = 0;
 

  lcd.createChar(0, cf);
  lcd.createChar(1, cb);
  lcd.createChar(2, pp);
  lcd.begin(1, 1);
  lcd.print("  Raspberry 05");
  delay(1000);
  lcd.begin(2, 2);
  lcd.setCursor(0, 0);
  lcd.write(byte(2));
  lcd.print("   Welcome   ");
  lcd.write(byte(0));
  lcd.write(byte(1));
  delay(750);

  for (int i = 0; i < 2; i++) {
    lcd.setCursor(0, 2);
    lcd.print(" Parking Master");
    delay(1000);
    lcd.setCursor(0, 2);
    lcd.print("                      ");
    delay(500);
  }
  ultrasonic();
  motor01Thread.check();
  motor02Thread.check();
  lcd.clear();
  digitalWrite(13, LOW);
  lcd.print("Avalible Slots :");
}

void loop() {
  //slotA_slotB_slotC_slotD_SlotCGate_SlotDGate
  //Serial.println(slotA+"_"+slotB+"_"+slotC+"_"+slotD+"_ _ _");
    parking_slot01();
    parking_slot02();
    specialParking_slot03();
    specialParking_slot04();
  Serial.print(slotA);
  Serial.print("_");
  Serial.print(slotB);
  Serial.print("_");
  Serial.print(slotC);
  Serial.print("_");
  Serial.print(slotD);
  Serial.println("_");

  //get & apply reservation data from raspberrypi throgh out usb.
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();
    if (command.substring(0, 2).equals("CB")) {
      bookC = 1;
    }else if (command.substring(0, 2).equals("CC")) {
      bookC = 0;
    } 
    
    if (command.substring(2, 4).equals("DB")) {
      bookD = 1;
    }else if (command.substring(2, 4).equals("DC")) {
      bookD = 0;
    } 
    
    if (command.substring(4, 6).equals("CG")) {
      gateC = 1;
    }else if (command.substring(4, 6).equals("CT")) {
      gateC = 0;
    }
    
    if (command.substring(6, 8).equals("DG")) {
      gateD = 1;
    }else if (command.substring(6, 8).equals("CT")) {
      gateD = 0;
    }
    Serial.println(command);
    Serial.println(command.substring(6, 8));
  }   

    parking_slot01();
    parking_slot02();
    specialParking_slot03();
    specialParking_slot04();
  
    //  ultraSonicThread.check();
  if (slotA == 1 && slotB == 1 && slotC == 1 && slotD == 1 ) {
    isallslotedFull = true;
    lcd.print("Avalible Slots :");
    lcd.setCursor(0, 1);
    lcd.print("All Slots Full  ");
    ultraSonicThread.check();
    motor02Thread.check();
    motor01Thread.check();
    motor03Thread.check();
    motor04Thread.check();
  }
  else if (slotA == 0 && slotB == 0 && slotC == 0 && slotD == 0 ) {
    isallslotedFull = false;
    lcd.print("Avalible Slots :");
    lcd.setCursor(0, 1);
    lcd.print("All Slots Empty ");
    ultraSonicThread.check();
    motor02Thread.check();
    motor01Thread.check();
//    motor03Thread.check();
//    motor04Thread.check();
    
  }
  else {
    motor04Thread.check();
    motor03Thread.check();
    motor02Thread.check();
    motor01Thread.check();
    digitalWrite(13, LOW);
    isallslotedFull = false;
    ultraSonicThread.check();
    lcd.setCursor(0, 0);
    lcd.print("Avalible Slots :");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    if(slotA == 0){
      lcd.setCursor(0, 1);
      lcd.print("01 ");
    }else{
      lcd.setCursor(0, 1);
      lcd.print("   ");
    }
    if(slotB == 0){
      lcd.setCursor(3, 1);
      lcd.print("02 ");
    }else{
      lcd.setCursor(3, 1);
      lcd.print("   ");
    }
    if(slotC == 0){
      lcd.setCursor(6, 1);
      lcd.print("03 ");
    }else{
      lcd.setCursor(6, 1);
      lcd.print("   ");
    }
    if(slotD == 0){
      lcd.setCursor(9, 1);
      lcd.print("04 ");
    }else{
      lcd.setCursor(9, 1);
      lcd.print("   ");
    }
  }
  
}
