//The Number of data sectors each sector is 360 deg/# of sectors
#define NUMBER_OF_SECTORS 12

// Sector struct which holds all the information about a sector
typedef struct{
  float sum = 0;             // used to calculate average
  uint32_t samples = 0;      // number of samples taken in sector
  float avgDistance = 0;     // avg = sum/samples
  uint16_t minVal = 0xFFFF;  // the closest point in the sector, set to max value if there are no points in sector
} Sector;

// Resets a sector to the default values
void sectorReset(Sector *s){
  s->sum = 0;
  s->samples = 0;
  s->avgDistance = 0;
  s->minVal = 0xFFFF;
}

// Resets all of the sectors in the data array
void sectorArrayReset(Sector* dataArray){
  for(uint8_t i = 0; i < NUMBER_OF_SECTORS; i++){
    sectorReset(&dataArray[i]);
  }
}

// takes lidar reading and updates the sector it belongs to using the data 
void addDataToSector(float angle,float distance,Sector *dataArray){
  
  int currentDataArrayIndex = (int)(angle/(360/NUMBER_OF_SECTORS)) % NUMBER_OF_SECTORS; // find out which sector the data belongs in based on its angle
  dataArray[currentDataArrayIndex].samples++;
  dataArray[currentDataArrayIndex].sum += distance;
  
  if(distance != 0 && distance < dataArray[currentDataArrayIndex].minVal) {  // if distance is less than current minVal make it the minVal
    dataArray[currentDataArrayIndex].minVal = distance;
  } 
 
}

// Calculates average for a sector and store it in the sector
void findSectorAvg(Sector * s){
  if(s->samples > 0){                    // make sure we don't divide by zero 
    s->avgDistance = s->sum/s->samples; 
  } 
}

// Calculates average for all the sectors in the data array 
void sectorArrayAvg(Sector* dataArray){
  for(uint8_t i = 0; i < NUMBER_OF_SECTORS; i++){
    findSectorAvg(&dataArray[i]);
  }  
}

// Prints information from the dataArray to console
void sectorArrayDebug(Sector * dataArray){
  for(uint8_t i = 0; i < NUMBER_OF_SECTORS; i++){
    Serial.print(dataArray[i].minVal);
    if(i != NUMBER_OF_SECTORS - 1)                 //print a new line instead of ',' after last sector
      Serial.print(",");
    else
      Serial.println();  
  }   
}
