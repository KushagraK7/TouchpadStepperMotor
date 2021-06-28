/*
 * Arduino code to control a stepper motor's postion using a PS/2 touchpad or mouse.
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
 * Tap the touchpad to move the motor back to starting position.
 */
#include <ps2.h>
#include<Stepper.h>

Stepper stepper(200, 8, 10, 9, 11);//If the motor steps backand forth, try replacing the
                                   //sequence of the pins to(200, 8, 9, 10, 11);
#define statLED 13

char mstat;
char mx;
char my;
int x, y, m;
int p = 0;
int sp = 120;

/*
 * an arduino sketch to interface with a ps/2 mouse.
 * Also uses serial protocol to talk back to the host
 * and report what it finds.
 */

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
  stepper.setSpeed(sp);
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

  if(x != 0) digitalWrite(statLED, HIGH);
  else digitalWrite(statLED, LOW);

  x = (int)mx;
  y = (int)my;
  m = (int)mstat;
  
  stepper.step(x);
  p += x;

  if(m == 9)
  {
    stepper.step(-p);
    p = 0;
  }

  
}
