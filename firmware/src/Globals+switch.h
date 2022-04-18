#define TRUE  1
#define FALSE 0
#define SD_AVAIL 1
#define NOT_SD_AVAIL 0

#define RELEASE      1
#define CLOSED       2
#define FLIGHT       3

#define GROUND       0
#define ASCENT       1
#define VENTING      2
#define FLOATING     3
#define DESCENT      4

int RELEASE_POS = 12;
int OPEN_POS    = 30;
int CLOSED_POS  = 95;
int VENT_ALT    = 22000;      // Meters (21000 ~ 68,900 feet)
                              // Meters (22700 ~ 75,000 feet)
                              // Meters (24400 ~ 80,000 feet)
int FLOAT_TIME  = 60;         // Minutes
int MAXFLOATALT = 27000;      // Meters (27500 ~ 91,500 feet)
int flightNum = 134;


#define NN_dAdt 60
#define N_ASCENTRATEBELOWZERO 30

long int floatStartTime=0;
long int timeOffset=0;

int programState;
int N_DAdt_lt_0=0;
String programStateString;
long int lastLogTime;


// at most 20 triggers
char *triggerStateNames[20];
float triggerAmin[20];
float triggerAmax[20];
float triggerDAdtmin[20];
float triggerDAdtmax[20];
int triggerTimemax[20];
char *triggerDestStateNames[20];

//////////////////////////////////////////////////////////////////////////////////////////////
//                                    Switches                                             ///
//////////////////////////////////////////////////////////////////////////////////////////////
int switch0 = 27;            //switch pin that starts FLIGHT sequence
int switch1 = 26;            //middle switch pin that moves actuator into CLOSED position
int switch2 = 4;             //switch pin that runs RELEASE sequence
int buttonPin = 16;          //button pin
boolean buttonPressed;

int switchPos(float currentAltitude) {
  
  if (currentAltitude>2000) return FLIGHT;

  if (digitalRead(switch0) == LOW) return RELEASE; 
  if (digitalRead(switch1) == LOW) return CLOSED;
  if (digitalRead(switch2) == LOW) return FLIGHT; 

  return -1;
}