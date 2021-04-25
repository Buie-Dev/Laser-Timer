#include <LiquidCrystal_I2C.h>

#include <Wire.h>

#define BUTTON 5
#define LANE1SENSOR A3
#define LANE2SENSOR A2
#define LANE1LASER 3
#define LANE2LASER 7
LiquidCrystal_I2C lcd(0x27, 16, 2);  // SDA A4, SCL A5

int maxTimeMillis = 10000;
int startButton = -1;
int lane1Time;
int lane2Time;

int totalTime;
boolean raceInProgress = false;
boolean lane1Finished = false;
boolean lane2Finished = false;
boolean mainTitleOn = false;


void setMainTitle() {
    lcd.clear();
    lcd.setCursor(3, 0);
    lcd.print("SKILLS  BC"); // LINE 1
    lcd.setCursor(0, 1);      
    lcd.print("GRAVITY CAR RACE"); // LINE 2
}

void setStartTitle() {
    lcd.clear();
    lcd.setCursor(0, 0); // Position Row
    lcd.print("LANE 1:");
    lcd.setCursor(0, 1);      
    lcd.print("LANE 2:"); 
  
}

void displayRaceCancelled() {
    lcd.clear();
    lcd.setCursor(1, 0); // Position Row
    lcd.print("RACE CANCELLED");  
}

int getRunningTime(int startTimeMillis, int currentTimeMillis) {
    return currentTimeMillis - startTimeMillis;
}

void displayRunningTime(int lane1Time, int lane2Time) {
    float runningTimeLane1Seconds = lane1Time / 1000.0f;
    float runningTimeLane2Seconds = lane2Time / 1000.0f;
  
    if (lane1Time >= maxTimeMillis) {
        lcd.setCursor(8, 0); // Position Row
        lcd.print("TIME>10s");
    } else {
        lcd.setCursor(8, 0); // Position Row
        lcd.print(runningTimeLane1Seconds, 3);
    }

    if (lane2Time >= maxTimeMillis) {
        lcd.setCursor(8, 1); // Position Row
        lcd.print("TIME>10s");
    } else {
        lcd.setCursor(8, 1); // Position Row
        lcd.print(runningTimeLane2Seconds, 3);
    }


  

    
}


void setup() {
  // put your setup code here, to run once:
  //lcd.begin();
  lcd.init();

  // Turn on the backlight.
  lcd.backlight();

  Serial.begin(9600);
  Serial.println("Laser Timer 2");

  pinMode(BUTTON, INPUT);
  pinMode(LANE1LASER, OUTPUT);
  pinMode(LANE2LASER, OUTPUT);

  pinMode(LANE1SENSOR, INPUT);
  pinMode(LANE2SENSOR,INPUT);
  
  setMainTitle();
  

}
int lastValue = 1024;

void loop() {

  //Serial.println(digitalRead(BUTTON));
  int buttonStatus = digitalRead(BUTTON);
  
  

  /*
  
  digitalWrite(LANE1LASER, HIGH);
      digitalWrite(LANE2LASER, HIGH);
  while(1) {
     int lane1Status = analogRead(LANE1SENSOR);
    int lane2Status = analogRead(LANE2SENSOR);
    Serial.print("Lane 1: ");
    Serial.println(lane1Status);
    Serial.print("Lane 2: ");
    Serial.println(lane2Status);
    delay(500);
  }

  */

  
  
  
 

  
  

  

  if (buttonStatus == LOW && !mainTitleOn && !raceInProgress ) {
      setMainTitle();
      mainTitleOn = true;
  } else if (buttonStatus == HIGH && !raceInProgress) {
      digitalWrite(LANE1LASER, HIGH);
      digitalWrite(LANE2LASER, HIGH);
      setStartTitle();
      mainTitleOn = false;
      raceInProgress = true;
  } else if (buttonStatus == LOW && raceInProgress) {
      float startTimeMillis = millis();
      while(raceInProgress) {
          int lane1Status = analogRead(LANE1SENSOR);
          int lane2Status = analogRead(LANE2SENSOR);
        
         
          //displayRunningTime(lane1Status, lane2Status);
          if (lane1Status > 700 && !lane1Finished) {
               lane1Time = getRunningTime(startTimeMillis, millis());
               lane1Finished = true;    
          }
          
          if (lane2Status > 700 && !lane2Finished) {
               lane2Time = getRunningTime(startTimeMillis, millis()); 
               lane2Finished = true; 
          } 
  


          //check if Timeed Out
          totalTime = millis() - startTimeMillis;
          if (totalTime >= maxTimeMillis) {
                if(!lane1Finished) {
                    lane1Time = getRunningTime(startTimeMillis, millis());
                    lane1Finished = true; 
                }
                if(!lane2Finished) {
                    lane2Time = getRunningTime(startTimeMillis, millis());
                    lane2Finished = true;
                }
              
          }


          if (lane1Finished && lane2Finished) {
            int button;
            do  {
              
              displayRunningTime(lane1Time, lane2Time);
              //Serial.println("Lane 1: " + String(lane1Time));
              //Serial.println("Lane 2: " + String(lane2Time));
              
              raceInProgress = false;
              lane1Finished = false;
              lane2Finished = false;
             
              button = digitalRead(BUTTON);
              digitalWrite(LANE1LASER, LOW);
              digitalWrite(LANE2LASER, LOW);
              
         
            } while (!button);
            delay(500);
            break;
          }
      
      }
      
  } 

  
  
  
  
  
  
   
}
