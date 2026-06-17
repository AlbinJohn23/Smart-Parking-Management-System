#include <LiquidCrystal.h>
#include <Servo.h>

// LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

Servo myservo;

// Ultrasonic sensor pins
int trigPin = 6;
int echoPin = 7;

int led1 = 13;
int led2 = 10;
int led3 = 8;

int buzzer = A5;

// Variables
int Slot = 1; // Total parking slots
int distance = 0;
bool carDetected = false;

const int threshold = 300;
bool spot1occ = false;
bool spot2occ = false;
bool spot3occ = false;

// Read ultrasonic distance
long readUltrasonicDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int cm = duration * 0.034 / 2;

  return cm;
}

void setup()
{
  Serial.begin(9600);

// LCD setup
  lcd.begin(16, 2);

// Ultrasonic setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  pinMode(buzzer, OUTPUT);

// Servo setup
  myservo.attach(9);
  myservo.write(0); // Gate closed

// Welcome message
  lcd.setCursor(0, 0);
  lcd.print("ARDUINO ");
  lcd.setCursor(0, 1);
  lcd.print("PARKING SYS");
  delay(5000);
  lcd.clear();

  digitalWrite(led1, HIGH);

  
}

void loop()
{
  distance = readUltrasonicDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

// Detect car within 30 cm
  if (distance < 5 && !carDetected)
  {
    carDetected = true;

    if (Slot > 0)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gate Opening");
      lcd.setCursor(0, 1);
      lcd.print("Slots Left:");
      lcd.print(Slot);

// Open gate
      for (int pos = 0; pos <= 120; pos++)
      {
        myservo.write(pos);
        analogWrite(buzzer, 256);
        
        
        delay(5);
        analogWrite(buzzer, 0);
      }

      delay(1100);

// Close gate
      for (int pos = 120; pos >= 0; pos--)
      {
        lcd.clear();
        lcd.print("Gate closing");
        myservo.write(pos);
        analogWrite(buzzer, 256);
        
        
        delay(5);
        analogWrite(buzzer, 0);
      }
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Parking Full");
      lcd.setCursor(0, 1);
      lcd.print("SORRY :(");
      delay(2000);
    }
  }

// Reset sensor when car moves away
  if (distance >= 5)
  {
    carDetected = false;
  }

// Main LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WELCOME! ");

  lcd.setCursor(0, 1);
  lcd.print("Slots Left: ");
  lcd.print(Slot);
  lcd.print(" ");

  delay(200);

  int ldr1=analogRead(A0); //LDR FOR SLOT ONE
  int ldr2=analogRead(A1); //LDR FOR SLOT TWO
  int ldr3=analogRead(A2); //LDR FOR SLOT THREE
  
  bool spot1 = (ldr1 < threshold);
  bool spot2 = (ldr2 < threshold);
  bool spot3 = (ldr3 < threshold);
  
  if(spot1 && !spot1occ) //first spot
  {
    Slot--;
    digitalWrite(led1, LOW);
  }
  else if(!spot1 && spot1occ)
  {
    Slot++;
    digitalWrite(led1, HIGH);
  }

  if(spot2 && !spot2occ) 
  {
    Slot--;
    digitalWrite(led2, LOW);
  }
  else if(!spot2 && spot2occ)
  {
    Slot++;
    digitalWrite(led2, HIGH);
  }

  if(spot3 && !spot3occ) 
  {
    Slot--;
    digitalWrite(led3, LOW);
  }
  else if(!spot3 && spot3occ)
  {
    Slot++;
    digitalWrite(led3, HIGH);
  }

  spot1occ = spot1;
  spot2occ = spot2;
  spot3occ = spot3;
  
}
