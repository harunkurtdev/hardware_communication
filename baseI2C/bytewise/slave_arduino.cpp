#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);


int x;
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    //Serial.print(c);         // print the character
  }
  x = (Wire.read()<<2);    // receive byte as an integer 2^8 ->2^10
  //int pot = Wire.read();  
  //Serial.println(x);         // print the integer
  //lcd.setCursor(2,0);
  //lcd.print("VALUES");
  //lcd.setCursor(0,1);
  //lcd.print(x);
  //Serial.println(x);
  //lcd.clear();
  //delay(50);
  
}

void requestEvent(){
  
  //lcd.setBacklight(HIGH);
  //lcd.setCursor(2,0);
  //lcd.print("VALUES");
  //lcd.setCursor(0,1);
  //lcd.print(x);
  //Serial.println(x);
  //lcd.clear();
  //Serial.println("helelo");
  //delay(50);
  
}

void setup()
{
  lcd.begin(16, 2);
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(9600);           // start serial for output
  lcd.setCursor(2,0);
  lcd.print("VALUES");
  //lcd.clear();
  delay(500);

}

void loop()
{
  //delay(100);
  //int pot = Wire.read();  
  Serial.println(x);         // print the integer
 // lcd.setCursor(2,0);
  //lcd.print("VALUES");
  lcd.setCursor(0,1);
  lcd.print(x);
  //Serial.println(x);
  
  //lcd.setCursor(2,0);
  //lcd.print("VALUES");
  delay(500);
  //lcd.clear();
  
}

