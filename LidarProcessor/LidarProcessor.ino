// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>
#include "QueueList.h"
#include "lidarMeasurement.h"
#include "UART.h"

// Uncomment to select which microcontroller you are using
#define TEENSY_LC

#define SIZE_OF_READING_BUFFER 32

// You need to create an driver instance
RPLidar lidar;

#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor.
// This pin should connected with the RPLIDAR's MOTOCTRL signal

QueueList <lidarMeasurement> readings;
float sumX,sumY = 0;
int countX,countY = 1;
void setup() {
  //  In this section we bind the RPLIDAR driver to the arduino hardware serial
  
  //  When we use the teensy we are able to use one of the serial ports to talk with the RPLIDAR
  //  and another for debug. Serial2 <-> RPLIDAR, Serial1 <-> debug console
  #ifdef TEENSY_LC
    lidar.begin(Serial2);
    Serial.begin(9600);
    Serial3.begin(57600);
  #endif
  
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  restartScan(); 
}

void loop() {
  if(readings.count() > 0){
    lidarMeasurement data;
    data = readings.pop();
    
    if(data.startBit){
      if(countX == 0)
        countX = 1;
      if(countY == 0)
        countY = 1;
        
      int avgX = 100*(sumX/countX);
      int avgY = 100*(sumY/countY);
      
      Serial3.write(0x00);
      UART::sendInt(avgX);
      UART::sendInt(avgY);
      
      Serial3.write(0x01);
      UART::sendFloat(0.5); 
      
      Serial.print(avgX);
      Serial.print("\t");
      Serial.println(avgY);
      
      sumX = 0;
      sumY = 0;
      countX = 0;
      countY = 0;
      
    }
    
    else{
      if(data.distance > 0){
        countX++;
        countY++;
        sumX += 100000.0/data.distance * sin(data.angle/573.0);
        sumY += 100000.0/data.distance * cos(data.angle/573.0);
      }
    }
  } 
}

/*
SerialEvent occurs whenever a new data comes in the
hardware serial RX. This routine is run between each
time loop() runs, so using delay inside loop can delay
response. Multiple bytes of data may be available.
*/
void serialEvent2() {
     if (IS_OK(lidar.waitPoint())) {
         lidarMeasurement reading;
          reading.distance = lidar.getCurrentPoint().distance; //distance value in mm unit
          reading.angle    = lidar.getCurrentPoint().angle; //angle value in tenths of a degree
          reading.startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
          reading.quality  = lidar.getCurrentPoint().quality; //quality of the current measurement, this is proportional to amount of reflected light
          //Serial.println(lidar.getCurrentPoint().distance);
          if(readings.count() <= SIZE_OF_READING_BUFFER)
            readings.push(reading);
            
          else{
            readings.pop();
            readings.push(reading);
            Serial.println("BUFFER OVF");
          }
     }
     else{
       restartScan();
     }
}

void restartScan(){
  analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
        Serial.println("restart");
        // try to detect RPLIDAR...
        rplidar_response_device_info_t info;
        if (IS_OK(lidar.getDeviceInfo(info, 100))) {
            // detected...
            lidar.startScan();
            // start motor rotating at max allowed speed
            analogWrite(RPLIDAR_MOTOR, 255);
            delay(1000);
        }
}
