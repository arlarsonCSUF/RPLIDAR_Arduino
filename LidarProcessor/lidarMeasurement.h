typedef struct{
  uint16_t distance = 0; //distance value in mm unit
  uint32_t angle    = 0; //angle value in tenths of a degree
  bool  startBit = 0; //whether this point is belong to a new scan
  byte  quality  = 0; //quality of the current measurement, this is proportional to amount of reflected light  
}lidarMeasurement;

