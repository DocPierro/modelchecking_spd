#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>

using namespace std;
double gK = 0.016890168;
double gc = 0.166;
double gC = 16.66;

double gCurrentReward = 0.0;
double gTime = 0.0;
double gY1 = (1-gc)*16.66;
double gY2 = gc*16.66;
double gIR = 10.0;

struct TimeReward
{
    double time;
    double reward;
};

struct RetValTR
{
    TimeReward * pt;
    unsigned int size;
};

TimeReward tr53[] = { {1.0, 2.0}, {12.3,14.5},{22.3,24.5},{32.3,34.5} };
TimeReward tr54[] = { {1.0, 2.0}, {2.3,4.5} };

RetValTR RetStruct(int id)
{
    RetValTR ret = {NULL,0};
    
    switch(id) {
        case 53:
            ret.pt = & tr53[0];
            ret.size = sizeof(tr53)/(sizeof(double)*2);
            break;
        case 54:
            //ret = & tr54[0];
            break;
            
    }
    return ret;
}

double TransitionTime(int id)
{
    double timeVal = 0.0;
    RetValTR rt = RetStruct(id);
    
    srand (time(NULL));
    int idx = rand() % rt.size;
    
    timeVal = rt.pt[idx].time;
    gCurrentReward = rt.pt[idx].reward/timeVal;
    
    return timeVal;
}

double y1fun(double kc, double kmc, double t, double y10, double y20, double L)
{
    return (exp(-(t*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*(kc*((y20*(kc*kc + 6*kc*kmc + kmc*kmc) - 4*L*kc + 2*kc*kc*y10 - kc*kc*y20 + kmc*kmc*y20 - 2*kc*kmc*y10 - 2*kc*y10*sqrt(kc*kc + 6*kc*kmc + kmc*kmc) + 2*kmc*y20*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/(2*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc)) + (2*L*kc*exp(-(kc*t)/2)*exp((kmc*t)/2)*exp((t*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/2))/((kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc))) + kmc*((y20*(kc*kc + 6*kc*kmc + kmc*kmc) - 4*L*kc + 2*kc*kc*y10 - kc*kc*y20 + kmc*kmc*y20 - 2*kc*kmc*y10 - 2*kc*y10*sqrt(kc*kc + 6*kc*kmc + kmc*kmc) + 2*kmc*y20*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/(2*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc)) + (2*L*kc*exp(-(kc*t)/2)*exp((kmc*t)/2)*exp((t*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/2))/((kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc))) - ((y20*(kc*kc + 6*kc*kmc + kmc*kmc) - 4*L*kc + 2*kc*kc*y10 - kc*kc*y20 + kmc*kmc*y20 - 2*kc*kmc*y10 - 2*kc*y10*sqrt(kc*kc + 6*kc*kmc + kmc*kmc) + 2*kmc*y20*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/(2*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc)) + (2*L*kc*exp(-(kc*t)/2)*exp((kmc*t)/2)*exp((t*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/2))/((kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc)))*sqrt(kc*kc + 6*kc*kmc + kmc*kmc) + kc*exp((t*(kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*exp((t*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*((y20*(kc*kc + 6*kc*kmc + kmc*kmc) - 4*L*kc + 2*kc*kc*y10 - kc*kc*y20 + kmc*kmc*y20 - 2*kc*kmc*y10 + 2*kc*y10*sqrt(kc*kc + 6*kc*kmc + kmc*kmc) - 2*kmc*y20*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/(2*(kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc)) + (2*L*kc*exp(-(kc*t)/2)*exp((kmc*t)/2)*exp(-(t*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/2))/((kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc))) + kmc*exp((t*(kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*exp((t*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*((y20*(kc*kc + 6*kc*kmc + kmc*kmc) - 4*L*kc + 2*kc*kc*y10 - kc*kc*y20 + kmc*kmc*y20 - 2*kc*kmc*y10 + 2*kc*y10*sqrt(kc*kc + 6*kc*kmc + kmc*kmc) - 2*kmc*y20*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/(2*(kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc)) + (2*L*kc*exp(-(kc*t)/2)*exp((kmc*t)/2)*exp(-(t*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/2))/((kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc))) + exp((t*(kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*exp((t*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*((y20*(kc*kc + 6*kc*kmc + kmc*kmc) - 4*L*kc + 2*kc*kc*y10 - kc*kc*y20 + kmc*kmc*y20 - 2*kc*kmc*y10 + 2*kc*y10*sqrt(kc*kc + 6*kc*kmc + kmc*kmc) - 2*kmc*y20*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/(2*(kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc)) + (2*L*kc*exp(-(kc*t)/2)*exp((kmc*t)/2)*exp(-(t*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/2))/((kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(6*kc*kmc + kc*kc + kmc*kmc)))*sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/(2*kc);
}

double y2fun(double kc, double kmc, double t, double y10, double y20, double L)
{
    return exp(-(t*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*((y20*(kc*kc + 6*kc*kmc + kmc*kmc) - 4*L*kc + 2*kc*kc*y10 - kc*kc*y20 + kmc*kmc*y20 - 2*kc*kmc*y10 - 2*kc*y10*sqrt(kc*kc + 6*kc*kmc + kmc*kmc) + 2*kmc*y20*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/(2*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(kc*kc + 6*kc*kmc + kmc*kmc)) + exp((t*(kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*exp((t*(kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc)))/2)*((y20*(kc*kc + 6*kc*kmc + kmc*kmc) - 4*L*kc + 2*kc*kc*y10 - kc*kc*y20 + kmc*kmc*y20 - 2*kc*kmc*y10 + 2*kc*y10*sqrt(kc*kc + 6*kc*kmc + kmc*kmc) - 2*kmc*y20*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/(2*(kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(kc*kc + 6*kc*kmc + kmc*kmc)) + (2*L*kc*exp(-(kc*t)/2)*exp((kmc*t)/2)*exp(-(t*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/2))/((kc - kmc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))) + (2*L*kc*exp(-(kc*t)/2)*exp((kmc*t)/2)*exp((t*sqrt(kc*kc + 6*kc*kmc + kmc*kmc))/2))/((kmc - kc + sqrt(kc*kc + 6*kc*kmc + kmc*kmc))*sqrt(kc*kc + 6*kc*kmc + kmc*kmc)));
}

void EnterActiv(double time)
{
    gY1 = y1fun(gK/gc, gK/(1-gc), time-gTime, gY1, gY2, gCurrentReward);
    gY2 = y2fun(gK/gc, gK/(1-gc), time-gTime, gY1, gY2, gCurrentReward);
    gTime += time;
}

void EnterIdle(double time)
{
    gY1 = y1fun(gK/gc, gK/(1-gc), time-gTime, gY1, gY2, gIR);
    gY2 = y2fun(gK/gc, gK/(1-gc), time-gTime, gY1, gY2, gIR);
    gTime += time;
}

bool CheckBattery(void)
{
    return (gY1>=0.0);
}

int main()
{
    double time;
    cout << "Hello World" << endl;
    
    time = TransitionTime(53);
    gCurrentReward = 4;
    EnterActiv(1);
    cout <<time<<" "<<gCurrentReward<<endl;
    cout <<gY1<<" "<<gY2<<endl;
    return 0;
}


