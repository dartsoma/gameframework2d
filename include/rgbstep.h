#ifndef __RGBSTEP_h__
#define __RGBSTEP_h__

typedef struct{

    int red;
    int green;
    int blue;
    int phase;

} rgbVal;

void rainbowStep(rgbVal *val, int minValue, int maxValue);

#endif
