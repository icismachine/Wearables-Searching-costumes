
//Access sensor... fast red planet
//ripples: 5-11 pins pwm
//twitch: 16, 12, 13
//ripple sensor in: z = a5/19
//twitch in: y = a4/18
//twitch in: x = a3/17

#define LED_LENGTH 100

#define ACCEL_IN_X 3
#define ACCEL_IN_Y 4
#define ACCEL_IN_Z 5

struct TwitchLED
{
  int source; //source analog pin
  int pin; //out
  int mask; //mask random bits
  int shift; //must be relative for the small lookup table we have
};


#define TWITCH_DELAY 2 //delay between changing 
#define TWITCH_LENGTH 3
#define TWITCH_TABLE_SIZE 8
TwitchLED TwitchLEDs[TWITCH_LENGTH] = {{ACCEL_IN_Y, 16, 0x7, 0}, {ACCEL_IN_X, 13, 0x070,4 }, {ACCEL_IN_Y, 12, 0x01C, 2}}; //Outpins to change
int TwitchTable[TWITCH_TABLE_SIZE] = {HIGH,LOW, LOW,HIGH, LOW, HIGH, HIGH, LOW}; //won't work for non pwm lines{0x1, 0x30, 0x70, 0xC};


void setup() {       
 
  RippleInit(); //ones on arm, light sensor connnected
  TwitchingLEDsInit(); //ones on tutu
  Serial.begin(9600);
  
}

void loop() {

  RippleControl(ACCEL_IN_Z); //ripple effect
  RippleTask();
  TwitchingTask();
}



void TwitchingLEDsInit()
{
  for(int i=0; i < TWITCH_LENGTH; i++)
    pinMode(TwitchLEDs[i].pin, OUTPUT); 

}

void TwitchingTask()
{
  int index;
  static int TwitchDelay = 0;
  int SensorIn; 
  
  if (TwitchDelay++ >= TWITCH_DELAY)
  {  
    for(int i=0; i < TWITCH_LENGTH; i++)
    {
      SensorIn = analogRead(TwitchLEDs[i].source); 
      Serial.print(SensorIn, HEX);  // print as an ASCII-encoded hexadecimal
      Serial.print("\t");    // prints a tab
      index = (TwitchLEDs[i].mask & SensorIn) >> TwitchLEDs[i].shift;
      Serial.print(index, HEX);  // print as an ASCII-encoded hexadecimal
      Serial.print("\t");    // prints a tab
      digitalWrite(TwitchLEDs[i].pin, TwitchTable[index] );
    
    }
    TwitchDelay = 0;
  }
  Serial.print("\n");    // prints a tab
}


#if 0
//based on input level, change light level
//no pulsate
void  AccelProcess(int OutputPin, int InPin)
{
    int SensorIn = analogRead(InPin);
    Serial.println(SensorIn);
   
    if (SensorIn > 600)
       analogWrite(OutputPin, 0xC0);
    else  if (SensorIn > 490)
       analogWrite(OutputPin, 0x60);   
    else analogWrite(OutputPin, 0x10);     

}


//based on input level, change light level
//no pulsate
void  AccelProcess_BrightnessChange(int Index)
{
    int SensorIn = analogRead(AccelIn[Index].InPin);
#ifdef DEBUG    
    Serial.print(SensorIn, HEX);  // print as an ASCII-encoded hexadecimal
    Serial.print("\t");    // prints a tab

    Serial.print(AccelIn[Index].Counter, HEX);  // print as an ASCII-encoded hexadecimal
    Serial.print("\t");    // prints a tab
 #endif   
   
    if (SensorIn > 600)
    {
       AccelIn[Index].pwm.Dir = UP;
#ifdef DEBUG
       Serial.print("UP\t"); // 
#endif
       Pulse(AccelIn[Index].OutPin, &(AccelIn[Index].pwm));
    }
    else  if (SensorIn < 490)
    {
      AccelIn[Index].pwm.Dir = DOWN;
#ifdef DEBUG
        Serial.print("DOWN\t"); // 
#endif
       Pulse(AccelIn[Index].OutPin, &(AccelIn[Index].pwm));  
    } 
 #ifdef DEBUG   
    Serial.print(AccelIn[Index].OutPin, DEC);  
    Serial.print("\n");    // prints a tab
#endif
}
#endif
