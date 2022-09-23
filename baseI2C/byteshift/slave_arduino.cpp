#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd(32);
int z=0;
int a;

void setup()
{ 
  lcd.begin(16, 2);
  //lcd.print("VALUES");
  
  Wire.begin(9); 
  Wire.onReceive(receiveEvent);
  
  Serial.begin(9600);
  
  delay(500);

}

void receiveEvent(int bytes) 
{
  //while(1 < Wire.available()) // loop through all but the last
  //{
   	//char c = Wire.read(); // receive byte as a character
   	//Serial.print(c);         // print the character
 	
  //}
  byte x = Wire.read();    // receive byte as an integer | first byte
  byte y = Wire.read();    // receive byte as an integer | second byte
  a=x+y*256; // merge first byte and second byte
  //Serial.println(a);
}


void loop()
{
  
  //lcd.setCursor(2,0);
  //lcd.print("VALUES");
  //lcd.setCursor(0,1);
  
  
  
  //lcd.setBacklight(HIGH);
  
    lcd.setCursor(0, 1);
    lcd.print(a); //write display comming value
    Serial.println(a);
  
//  lcd.setBacklight(LOW);
  
  delay(200);
  //lcd.clear();
}