
enum counter_directions {
  DOWN,
  UP,
  NONE
};

struct PWMcontrol
{
  unsigned char Counter;
  counter_directions Dir;
  unsigned char Center;  
  unsigned char Width;
  unsigned char Delta;
  unsigned char LimitMax;
  unsigned char LimitMin;
};

void ChangePulseCenter(struct PWMcontrol * pwm, unsigned char value)
{
  pwm->Center = value;
  
}

void ChangePulseBandWidth(struct PWMcontrol * pwm, unsigned char value)
{
  pwm->Width = value;
  
}

void ChangeDelta(struct PWMcontrol * pwm, unsigned char value)
{
  pwm->Delta = value;
  
}

void PrintDirection( struct PWMcontrol * pwm)
{
  switch (pwm->Dir)
  {
    case UP:
    Serial.print("UP ");  // print as an ASCII-encoded hexadecimal
    break;
    case DOWN:
        Serial.print("DOWN ");  // print as an ASCII-encoded hexadecimal
    break;
    
    case NONE:
        Serial.print("NONE ");  // print as an ASCII-encoded hexadecimal
    break;
    
    default:
        Serial.print("Dir??? ");  // print as an ASCII-encoded hexadecimal
    break;
  
  }
  
}	  
 
 void Pulse(int pin, struct PWMcontrol * pwm )
{
  analogWrite(pin, pwm->Counter);   // set the LED on
 
   int Upper = pwm->Center + pwm->Width;
   int Lower = pwm->Center - pwm->Width;
  
   if (Upper > pwm->LimitMax)
     Upper = pwm->Width;
   if (Lower < pwm->LimitMin)
     Lower = pwm->Width;
  
  if (pwm->Counter >= Upper)
    pwm->Dir = DOWN;
   
  if (pwm->Counter <= Lower) //might need to move Counter within limits asap
    pwm->Dir = UP;
  
  if ((pwm->Dir == UP) & (pwm->Counter <= pwm->LimitMax - pwm->Delta))
    (pwm->Counter) += pwm->Delta;
  else if ((pwm->Dir == DOWN)& (pwm->Counter >= pwm->LimitMin + pwm->Delta))
    (pwm->Counter) -= pwm->Delta;
  else Serial.print("bad: can't exit\n");  
  
}
