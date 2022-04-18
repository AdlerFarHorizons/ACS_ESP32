

// get the best etimate of the derivative of the altitude
// we do a running simple linear regression on the last NN altitude 
// data points and return the slope beta
// see https://en.wikipedia.org/wiki/Simple_linear_regression for details
// (y = alpha + beta *x) where y is altitude and x is time
// cA is current Altitude, and cT is current Time, both get added into the internally maintained 
// running arrays

float dAdt_fiT_2(float cA,long int cT) {

  static int startFlag=1;
  static int n=0;
  static double altitudeList[NN_dAdt];
  static double timeList[NN_dAdt];
  double St=0,Sa=0,Sta=0,Stt=0;
  double pA;
  double pT;
  int i;

// if we are just starting initialize the internal arrays
  if (startFlag==1) {
    for (i=0;i<NN_dAdt;i++) {
      altitudeList[i]=0.0;
      timeList[i]=i;
      St+=i;
      Stt+=i*i;
    }
    startFlag=0;
  }

// write the new data into the internal arrays
  pA=altitudeList[n];                                       // find the past Altitude we are about to overwrite
  pT=timeList[n];                                           // find the past Time we are about to overwrite
  altitudeList[n]=cA;                                       // overwrite with current Altitude
  timeList[n]=cT;                                           // overwrite with current Time
  n=(n+1)%NN_dAdt;                                          // move our pointer to the next available space

  for (i=0;i<NN_dAdt;i++) {
    St+=(timeList[i]-pT)/10.0;
    Stt+=((timeList[i]-pT)*(timeList[i]-pT))/100.0;
    Sa+=altitudeList[i]/10.0;
    Sta+=(altitudeList[i]*(timeList[i]-pT))/100.0;
  }
 
  return 1000.0* (NN_dAdt*Sta-St*Sa)/(NN_dAdt*Stt-St*St);   // return the fitted slope adjusted for the millisec units of time

}



// this is a very simple and fast (but not terribly accurate) way to get altitude from the pressure
float get_altitude(float pressure) {

   return 25000.0*(log(1010)-log(pressure));
}


// a more accurate way to get altitude from the pressure
// takes mbar and returns meters
// valid from 1122mb to 3.16mb or approximately -800m to 40km
// values from the 1976 std atmosphere model at http://www.luizmonteiro.com/stdatm.aspx
float altitudeFromPressure(float pressure) {
  static float altitude[60]={-868.3,110.9,1069.2,2006.9,2924.7,3822.8,4701.7,5561.8,6403.4,7227.1,8033.1,8821.9,
      9593.8,10349.1,11088.3,11821.0,12553.9,13287.0,14020.3,14753.7,15487.2,16221.0,16954.9,
      17689.0,18423.2,19157.7,19892.4,20627.8,21365.9,22106.6,22850.0,23596.2,24345.0,25096.5,
      25850.7,26607.6,27367.3,28129.7,28894.8,29662.9,30433.6,31207.1,31983.4,32763.9,33552.0,
          34347.6,35151.0,35962.2,36781.4,37608.5,38443.7,39286.9,40138.5};
  float lp,a1,a2,da;
  float index;
  lp=log10(pressure);
  index=61.0-20.0*lp;
    
  a1=altitude[(int)index];
  a2=altitude[(int)index+1];
  da=a2-a1;
  
  return a1+da*(index-((int)index));
}

void parseTrigger(String lineBuffer, int i);

void spaceTrim(char *s) {
  int i,si,li;
  for (i=strlen(s)-1;i>0;i--) {
    if (!isspace(s[i])) break;
    s[i]='\0';
  }

  si=strspn(s," \f\n\r\t\v");
  li=strlen(s);
  for (i=si;i<li+1;i++) {
    s[i-si]=s[i];
  }

}

void parseTrigger(char lineBuf[], int i) 
{
  static int N=-1;
 
  char *token;
  if (N==-1) {
    N=0;
    return;
  }

// get the trigger state
  token=strtok(lineBuf,",");
  spaceTrim(token);
  triggerStateNames[N]= (char *)calloc(strlen(token), sizeof(char));
  strcpy(token,triggerStateNames[N]);

// get Amin  
triggerAmin[N]=atof(strtok(NULL,","));
  
// get Amax  
triggerAmax[N]=atof(strtok(NULL,","));

// get dAdtmin  
triggerDAdtmin[N]=atof(strtok(NULL,","));

// get dAdtmax  
triggerDAdtmax[N]=atof(strtok(NULL,","));

// get Timemax
triggerTimemax[N]=atoi(strtok(NULL,","));

// get the trigger state
  token=strtok(NULL,",");
  spaceTrim(token);
  triggerStateNames[N]= (char *)calloc(strlen(token), sizeof(char));
  strcpy(token,triggerStateNames[N]);

}