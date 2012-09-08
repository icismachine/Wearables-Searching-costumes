//Light sensor... slow blue planet

//ripples: 5-11 pins pwm
//twitch: 16-18
//light sensor in: a5/19


struct TwitchLED
{
  int pin; //out
  int mask; //mask random bits
  int shift; //must be relative for the small lookup table we have
};



int LightSensorIn = 5;  //a5 or 19
#define TWITCH_DELAY 30 //delay between changing 
#define TWITCH_LENGTH 3
#define TWITCH_TABLE_SIZE 8
TwitchLED TwitchLEDs[TWITCH_LENGTH] = {{16, 0x7, 0}, {17, 0x038,3 }, {18, 0x01C, 2}}; //Outpins to change
int TwitchTable[TWITCH_TABLE_SIZE] = {HIGH,LOW, LOW,HIGH, LOW, HIGH, HIGH, LOW}; //won't work for non pwm lines{0x1, 0x30, 0x70, 0xC};



                
void setup() {       

  RippleInit(); //ones on arm, light sensor connnected
  TwitchingLEDsInit(); //ones on tutu
  Serial.begin(9600);
  
}

void loop() {

  RippleControl(LightSensorIn); //ripple effect
  RippleTask();
  TwitchingTask(LightSensorIn);
 
}

void TwitchingLEDsInit()
{
  for(int i=0; i < TWITCH_LENGTH; i++)
    pinMode(TwitchLEDs[i].pin, OUTPUT); 

}

void TwitchingTask(int InPin)
{
  int index;
  bool test;
  static int TwitchDelay = 0;
  int SensorIn = analogRead(InPin); 
  Serial.print(SensorIn, HEX);  // print as an ASCII-encoded hexadecimal
  Serial.print("\t");    // prints a tab
  
  if (TwitchDelay++ >= TWITCH_DELAY)
  {
    for(int i=0; i < TWITCH_LENGTH; i++)
    {
      index = (TwitchLEDs[i].mask & SensorIn) >> TwitchLEDs[i].shift;
      Serial.print(index, HEX);  // print as an ASCII-encoded hexadecimal
      Serial.print("\t");    // prints a tab
      //test = digitalRead(TwitchLEDs[i].pin) ^ 0x1;
      //digitalWrite(TwitchLEDs[i].pin, test );
      digitalWrite(TwitchLEDs[i].pin, TwitchTable[index] );
    
    }
    TwitchDelay = 0;
  }
  Serial.print("\n");    // prints a tab
}
   

