
#include "rgbStep.h"


/* Start your object in at least one of the phases before using

 Phase 1 - Fill Green
 Phase 2 - Drain Red
 Phase 3 - Fill Blue
 Phase 4 - Drain Green
 Phase 5 - Fill Red
 Phase 6 - Drain Blue

 */
// ie - If you're using a 100 min then state so

void rainbowStep(rgbVal *val, int minValue, int maxValue) {

    // I would error wrap this but it runs on tick so...
    switch (val->phase){

        case 1:
        if (val->green >= maxValue){
            val->phase++;
            break;
        }
            val->green++;
        break;

        case 2:
            if (val->red <= minValue){
                val->phase++;
                break;
            }
            val->red--;

            break;
        case 3:
            if (val->blue >= maxValue){
                val->phase++;
                break;
            }
            val->blue++;
            break;

        case 4:
            if (val->green <= minValue){
                val->phase++;
                break;
            }
            val->green--;

            break;

        case 5:
            if (val->red >= maxValue){
                val->phase++;
                break;
            }
            val->red++;

            break;

        case 6:
            if (val->blue <= minValue){
                val->phase = 1;
                break;
            }
            val->blue--;

            break;
    }

    return;
}


