//Ripple


enum ripple_status {
  OFF,
  STARTED,
  IN_PROGRESS
};

struct RippleType
{
  int Level; // the lower the level, the dark it is will turn it on
  int Delay; // the bigger the delay, the less the overlap
  int Delta; //the slower the burn; the smaller the value
};


ripple_status RippleState;

int RippleStart = 0;

int RippleIndex;
 
#define RIPPLE_TYPES 2
#define RIPPLE_SLOW 0
#define RIPPLE_FAST 1

struct RippleType Ripples[2] = \
  { \
    {0x10, 0x10, 0x5}, \
    {0x40, 0x5, 0x20} \
  };
  

struct RippleElement
{
  int OutPin;
  PWMcontrol pwm;
};


#define RIPPLE_LENGTH   5  
struct RippleElement RippleEffect[RIPPLE_LENGTH] = {{5,{0, NONE, 0x80, 0x80, 0xA, 0xFF, 0 }}, \ 
                {6,{0, NONE, 0x80, 0x80, 0xA, 0xFF, 0 }} , \
                 {9,{0, NONE, 0x80, 0x80, 0xA, 0xFF, 0 }}, \
                  {10,{0, NONE, 0x80, 0x80, 0xA, 0xFF, 0 }}, \
                  {11,{0, NONE, 0x80, 0x80, 0xA, 0xFF, 0}} , \
                };

//light sensor... start ripple
void  RippleControl(int InPin)
{
    static int count = 0;
    int SensorIn = analogRead(InPin);
    
    Serial.print(SensorIn, HEX);  // print as an ASCII-encoded hexadecimal
    Serial.print("\n");    // prints a tab

    switch (RippleState)
    {
    case OFF:
      if (SensorIn < Ripples[RIPPLE_SLOW].Level) //dark: slow Can tune for device on extra ADC
      {
          RippleIndex = 0;
        }
      else if (SensorIn < Ripples[RIPPLE_FAST].Level) //faster 
      {
          RippleIndex = 1;
      }
      else break;
      
      Serial.print("Started Ripple\n");  // print as an ASCII-encoded hexadecimal
      RippleState = STARTED;
      RippleStartElement(&(RippleEffect[RippleStart++].pwm), RippleIndex);
      count = 0;
    break;
    case STARTED:

      //count delay
      //if not all LEDs have been activated.
      if (RippleStart <  RIPPLE_LENGTH)
      {
          if (count++ > Ripples[RippleIndex].Delay)
          {
              RippleStartElement(&(RippleEffect[RippleStart++].pwm), RippleIndex);
            count = 0;
          }
      }
      else RippleState = IN_PROGRESS; //in progress but not finished. all LEDs are started
      break;
    default:
    case IN_PROGRESS:
  
      Serial.print("In progress\n");  // print as an ASCII-encoded hexadecimal

    break;
    }
    
}
void RippleInit()
{
  for (int i =0; i < RIPPLE_LENGTH; i++)
  {
    pinMode(RippleEffect[i].OutPin, OUTPUT); 
    
  }
  RippleState = OFF;
  RippleIndex = RIPPLE_TYPES;
}

void RippleTask()
{
  int i, DeactiveCount = 0;
  if (RippleState != OFF)
  {
    Serial.print("In Ripple\t");  // print as an ASCII-encoded hexadecimal
    Serial.print(RippleStart, DEC);  // print as an ASCII-encoded hexadecimal
    Serial.print("\t");  // print as an ASCII-encoded hexadecimal
    for( i = 0; i < RIPPLE_LENGTH; i++)
    {

      if (RippleEffect[i].pwm.Dir == NONE)
      {
        Serial.print(i, DEC);  // print as an ASCII-encoded hexadecimal
         Serial.print(" is off ");  // print as an ASCII-encoded hexadecimal 
          Serial.print(RippleEffect[i].pwm.Counter, HEX);  // print as an ASCII-encoded hexadecimal
         Serial.print("\t");  // print as an ASCII-encoded hexadecimal
         DeactiveCount++;
         
      }
      else {
         Serial.print(RippleEffect[i].pwm.Counter, HEX);  // print as an ASCII-encoded hexadecimal
         Serial.print(" ");  // print as an ASCII-encoded hexadecimal  
         PrintDirection(&(RippleEffect[i].pwm));  // print as an ASCII-encoded hexadecimal
         Serial.print("\t");  // print as an ASCII-encoded hexadecimal  
        Pulse(RippleEffect[i].OutPin,&(RippleEffect[i].pwm));
      }
   
      
      if((RippleEffect[i].pwm.Counter == 0) && (RippleEffect[i].pwm.Dir == DOWN))
      {
        Serial.print("stop");  // print as an ASCII-encoded hexadecimal 
        //turn off direction
        RippleEndElement(&(RippleEffect[i].pwm), RippleEffect[i].OutPin);
      }

    }
    Serial.print("\n");  // print as an ASCII-encoded hexadecimal 
    if (DeactiveCount == RIPPLE_LENGTH)
    {//if delta is so high compared to delayed start then you might turn off too soon 
      RippleState = OFF;
      RippleStart = 0;
    }
  }
}

void RippleStartElement (struct PWMcontrol * pwm, int Index)
{
     pwm->Dir = UP;
     pwm->Counter = 0;
     pwm->Delta = Ripples[Index].Delta;
}

void RippleEndElement (struct PWMcontrol * pwm, int OutPin)
{
     pwm->Dir = NONE;
     digitalWrite(OutPin, LOW);
}
