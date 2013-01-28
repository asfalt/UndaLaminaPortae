#include "Slice.h"


Slice::Slice()
{

    Xpos = 0.0;
    number = -1;
    hti.append(-1.0);
    vti.append(0.0);
    isBound = false;

    vti_dop.append(0.0);
}



Slice::Slice(int numberVal, double XposVal, double hVal)
{
    number = numberVal;
    Xpos = XposVal;
    hti.append(hVal);
    vti.append(0.0);
    isBound = false;

    vti_dop.append(0.0);
}


