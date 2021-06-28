/*
 * Arduino code to control a stepper motor's postion using a PS/2 touchpad or mouse.
 * Acceleration/decceleration feature, thanks to AccelSteper library.
 * Made by Tech Build:-https://www.youtube.com/channel/UCNy7DyfhSD9jsQEgNwETp9g?sub_confirmation=1
 * Code originally made to run a stepper motor using a 
 * ULN2003 or similar driver(For unipolar motors)
 *                     OR 
 * L298N or driver(For both unipolar and bipolar motors)
 * 
 * Feel free to modify the code to adapt it to any oter motor driver or shield being used.
 * 
 * The code is tested only with a PS/2 touchpad and might not work properly with a mouse!
 * 
 * Swipe finger on the touchpad or move the mouse to make the motor rotate 
 * in a particular direction for a particular number of steps.
 * 
 * Tap the touchpad while the motor is moving to make it stop instantly.
 */
#include<ps2.h>         //Library for interfacing the PS/2 touchpad with the Arduino MCU.
#include<Stepper.h>     //Library for controlling the stepper motor.
#include<AccelStepper.h>//Library for acceleration/deccelration feature, very useful for 
                        //stepper motor control.

AccelStepper stepper(AccelStepper :: DRIVER, 12, 13);//If the motor steps backand forth, try replacing the
                                   //sequence of the pins to(200, 8, 9, 10, 11);

#define statLED 25

char mstat;
char mx;
char my;
int x, y, m;
long p = 0;
int d;

/*
 * Pin 5 is the mouse data pin, pin 6 is the clock pin
 * Feel free to use whatever pins are convenient.
 */
PS2 mouse(6, 5);//(Clock, Data)

/*
 * initialize the mouse. Reset it, and place it into remote
 * mode, so we can get the encoder data on demand.
 */
void mouse_init()
{
  mouse.write(0xff);  // reset
  mouse.read();  // ack byte
  mouse.read();  // blank */
  mouse.read();  // blank */
  mouse.write(0xf0);  // remote mode
  mouse.read();  // ack
  delayMicroseconds(100);
}

void setup()
{
  pinMode(statLED, OUTPUT);
  
  Serial.begin(9600);
  
  mouse_init();
  
  stepper.setMaxSpeed(250);
  stepper.setAcceleration(500);
  
}

/*
 * get a reading from the mouse and report it back to the
 * host via the serial line.
 */
void loop()
{

  /* get a reading from the mouse */
  mouse.write(0xeb);  // give me data!
  mouse.read();      // ignore ack
  mstat = mouse.read();
  mx = mouse.read();
  my = mouse.read();

   m = (int)mstat;
   x = (int)mx;
   y = (int)my;

   Serial.println(p);

   p += x;
   
  if(m == 9)
  {
    digitalWrite(statLED, HIGH);
    p = stepper.currentPosition();

    d = stepper.distanceToGo();

    stepper.setAcceleration(10000); 

    stepper.moveTo(p);

    while(d != 0)
    {
     stepper.run();
     d = stepper.distanceToGo();
    }
    
    digitalWrite(statLED, LOW);

    stepper.setAcceleration(500); 
    
  }
  else digitalWrite(statLED, LOW);
  
  stepper.moveTo(p);
  
  stepper.run();
  
}
