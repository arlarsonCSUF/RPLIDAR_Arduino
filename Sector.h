typedef struct{
  uint32_t sum = 0;
  uint32_t samples = 0;
  float avgDistance = 0;
} Sector;

void sectorReset(Sector *s){
  s->sum = 0;
  s->samples = 0;
  s->avgDistance = 0;
}
