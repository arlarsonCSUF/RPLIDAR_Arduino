typedef struct{
  float sum = 0;             // used to calculate average
  uint32_t samples = 0;      // number of samples taken in sector
  float avgDistance = 0;     // avg = sum/samples
  uint16_t minVal = 0xFFFF;  // the closest point in the sector, set to max value if there are no points in sector
} Sector;

void sectorReset(Sector *s){
  s->sum = 0;
  s->samples = 0;
  s->avgDistance = 0;
  s->minVal = 0xFFFF;
}
