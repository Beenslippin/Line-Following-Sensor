
 #define F_CPU 8000000UL
 #include <avr/io.h>
 #include <util/delay.h>

 #define ledLeftOn (PORTA |= (1<<4)) //left Led on (|= bitwise OR, turn on bits)
 #define ledLeftOff (PORTA &=~ (1<<4))  //left Led off  (~ complement, &= bitwise AND, turn off bits)
 #define ledRightOn (PORTC |= (1<<4))  //right Led on
 #define ledRightOff (PORTC &=~ (1<<4))  //right Led off

 #define LeftControlCW (PORTA |= (1<<0)) //Configured for PA0 ON (OFF)
 #define LeftControlCCW (PORTA &=~ (1<<0)) //Configured for PA0 OFF (ON)
 #define RightControlCW (PORTC |= (1<<2)) //Configured for PC2 ON (OFF)
 #define RightControlCCW (PORTC &=~ (1<<2)) //Configured for PC2 OFF (ON)

 #define leftBumper !(PINA & (1<<2)) //active low
 #define rightBumper !(PINC & (1<<5)) //active low

 #define leftSensor (PINA & (1<<7)) //left sensor black
 #define centerSensor (PINA & (1<<6)) //center sensor black
 #define rightSensor (PINA & (1<<3)) //right sensor black

 #define CCW 0 // counter clockwise
 #define CW 1 // clockwise

 void setup(void);
 void SetLeftMotorSpeedandDirection(unsigned char Speed,char Direction);
 void SetRightMotorSpeedandDirection(unsigned char Speed,char Direction);


 int main(void)
 {
 setup();
 while (1)
 {
 //when left bumper is pushed
 if(leftBumper)
 {
 _delay_ms(200); //debounce
 SetLeftMotorSpeedandDirection(100,CW);
 SetRightMotorSpeedandDirection(100,CCW); // CW = REVERSE all motors, + reverse left turn
 _delay_ms(500);
 SetLeftMotorSpeedandDirection(100,CCW); //CCW = FORWARD all motors, heading straight
 SetRightMotorSpeedandDirection(100,CW);
 _delay_ms(400);
 SetLeftMotorSpeedandDirection(100,CCW); //CCW = FORWARD all motors, heading straight
 SetRightMotorSpeedandDirection(100,CW);
 }
 //when right bumper is pushed
 if(rightBumper)
 {
 _delay_ms(200); //debounce
 SetLeftMotorSpeedandDirection(100,CW); //reverse right turn
 SetRightMotorSpeedandDirection(70,CCW);
 _delay_ms(500);
 SetLeftMotorSpeedandDirection(100,CCW); //straight
 SetRightMotorSpeedandDirection(100,CW);
 //
 _delay_ms(400);
 SetLeftMotorSpeedandDirection(100,CCW); //CCW = FORWARD all motors, heading straight
 SetRightMotorSpeedandDirection(100,CW);
 }
 // Foward
 if(centerSensor && !rightSensor && !leftSensor)
 {
 ledLeftOff;
 ledRightOff;
 SetLeftMotorSpeedandDirection(100,CCW);
 SetRightMotorSpeedandDirection(100,CW);
 }
 // Slight Right Turn
 if(centerSensor && rightSensor && !leftSensor)
 {
 ledLeftOff;
 ledRightOn;
 SetLeftMotorSpeedandDirection(100,CCW);
 SetRightMotorSpeedandDirection(60,CW);
 }
 // Hard Right Turn
 if(!centerSensor && rightSensor && !leftSensor)
 {
 ledLeftOff;
 ledRightOn;
 SetLeftMotorSpeedandDirection(100,CCW);
 SetRightMotorSpeedandDirection(0,CW);
 }
 // Slight Left Turn
 if(centerSensor && !rightSensor && leftSensor)
 {
 ledLeftOn;
 ledRightOff;
 SetLeftMotorSpeedandDirection(60,CCW);
 SetRightMotorSpeedandDirection(100,CW);
 }
 // Hard Left Turn
 if(!centerSensor && !rightSensor && leftSensor)
 {
 ledLeftOn;
 ledRightOff;
 SetLeftMotorSpeedandDirection(0,CCW);
 SetRightMotorSpeedandDirection(100,CW);
 }
 // when center only white, used to turn slight left
 if(!centerSensor && rightSensor && leftSensor)
 {
 ledLeftOn;
 ledRightOn;
 SetLeftMotorSpeedandDirection(60,CCW);
 SetRightMotorSpeedandDirection(100,CW);
 }
 // when all black, full speed straight
 if(centerSensor && rightSensor && leftSensor)
 {
 ledLeftOn;
 ledRightOn;
 SetLeftMotorSpeedandDirection(100,CCW);
 SetRightMotorSpeedandDirection(100,CW);
 }

}
 }
 
 void setup(void)
 {
 DDRA = 0b00110001; //All inputs/outputs for A
 DDRC = 0b00010101; //All inputs/outputs for C
 /* Timer setup for Motors*/
 OCR0A = 0; // Right motor off
 OCR0B = 0; // Left motor off
 TCCR0A = 0b00000011; // fast pwm channel A and B, Top = 0xFF
 TCCR0B = 0b00000101; // prescaler = 1024
 }
 
 //**************************Left motor control********************************/
 // Speed 0-100%, direction 1 CW and 0 CCW
 
 void SetLeftMotorSpeedandDirection(unsigned char Speed,char Direction)
 {
 if(Speed>100) // limit the maximum speed to 100%
 Speed = 100;
 if(Speed <= 0) // if 0% selected stop the motor and disable the PWM
 {
 TCCR0A &=~(1<<4)&~(1<<5); // Disconnect OCR0B
 LeftControlCCW; // Left motor compare match will never happen
 OCR0B = 0;
 }
 else
 {
 if(Direction == CCW)
 {
 TCCR0A |= (1<<5);
 TCCR0A &= ~(1<<4); // clear OCR0B on compare match
 OCR0B = ((double)Speed/100)*255; //sets duty cycle dependant on "speed", same with all OCR0B and OCR0A
 LeftControlCCW; // clear right motor control line high
 }
 else if(Direction == CW)
 {
 TCCR0A |= (1<<5);
 TCCR0A |= (1<<4); // set OCR0B on compare match
 OCR0B = ((double)Speed/100)*255;
 LeftControlCW;// set left motor control line high
 }
 }
 }
 
 //**************************Right motor control*******************************/
 // Speed 0-100%, direction 1 CW and 0 CCW
 void SetRightMotorSpeedandDirection(unsigned char Speed,char Direction)
 {
 if(Speed>100) // limit the maximum speed to 100%
 Speed = 100;
 if(Speed <= 0) // if 0% selected stop the motor and disable the PWM
 {
 TCCR0A &=~(1<<7)&~(1<<6); // Disconnect OCR0A
 RightControlCCW; // Right motor compare match will never happen
 OCR0A = 0;
 }
 else
 {
 if(Direction == CCW)
 {
 TCCR0A |= (1<<7);
 TCCR0A |= (1<<6); // set OCR0A on compare match
 OCR0A = ((double)Speed/100)*255;
 RightControlCW; // set right motor control line high
 }
 else if(Direction == CW)
 {
 TCCR0A |= (1<<7);
 TCCR0A &= ~(1<<6); // clear OCR0A on compare match
 OCR0A = ((double)Speed/100)*255;
 RightControlCCW; // Clear right motor control line high
 }
 }
 }

