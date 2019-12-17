/* OOMP wiswas
 * 
 * wiswas control based on: 
 * Ultrasonic Ranging Tank Robot
 * 
 * L = Left, R = Right , F = forward , B = backward
 *  U D L R S
 *  8 2 4 6
 */

/******************** 
psuedo 
  map surroundings in 45 degree increments
    movie to the middle of the area 
      (obstacle distance should be equal in both opposite directions)
   determine height of obstacles

   spiral the entire area until the end (and obstacle distance is 0)


**********************/
#include <Servo.h>

int pinLB = 12; // define pin 12
int pinLF = 3; // define pin 3
int pinRB = 13; // define pin 13
int pinRF = 11; // define pin 11

int inputPin = 4; // define pin for sensor echo
int outputPin = 5; // define pin for sensor trig

int Fspeedd = 0; // forward speed
int Rspeedd = 0; // right speed
int Lspeedd = 0; // left speed
int directionn = 0; // forward=8 backward=2 left=4 right=6

Servo myservo; // set myservo
int delay_time = 250; // settling time after steering servo motor moving B

int Fgo = 8; // Move F
int Rgo = 6; // move to the R
int Lgo = 4; // move to the L
int Bgo = 2; // move B

/* oomp */

int mapIncrem = 8; //set counter for the number of mapping points
int curMode = 1; // what mode is wiswas in: 1 = mapping surroundings, 2 = problem?, 3 = cleaning, 4 = end

int omniDirectionals[7]; //define array to store the distances in 8 measure directions. [0] = forward, [2] = left, [4] = backward, [6] = right
int omniCount = 0;

void setup() {
            Serial.begin(9600); // Define motor output pin
            pinMode(pinLB,OUTPUT); // pin 12
            pinMode(pinLF,OUTPUT); // pin 3 (PWM)
            pinMode(pinRB,OUTPUT); // pin 13
            pinMode(pinRF,OUTPUT); // pin 11 (PWM)
            pinMode(inputPin, INPUT); // define input pin for sensor
            pinMode(outputPin, OUTPUT); // define output pin for sensor
            
            myservo.attach(9); // Define servo motor output pin to D9 (PWM)
}

/* move forward */
void advance() {
            digitalWrite(pinLB,LOW); // right wheel moves forward
            digitalWrite(pinRB, LOW); // left wheel moves forward
            analogWrite(pinLF,255);
            analogWrite(pinRF,255);
}

/* stop */
void stopp() {
            digitalWrite(pinLB,HIGH);
            digitalWrite(pinRB,HIGH);
            analogWrite(pinLF,0);
            analogWrite(pinRF,0);
}

/* turn right in place ( not really single wheel? ) */
void right() {
            digitalWrite(pinLB,HIGH); // wheel on the left moves forward
            digitalWrite(pinRB,LOW); // wheel on the right moves backward
            analogWrite(pinLF, 255);
            analogWrite(pinRF,255);
}

/* turn left in place ( not really single wheel? ) */
void left() {
            //digitalWrite(pinLB,LOW); // wheel on the left moves backward
            //digitalWrite(pinRB,HIGH); // wheel on the right moves forward
            analogWrite(pinLF, 255);
            analogWrite(pinRF,255);
}

void incrSpinLeft(int deg) {
            //digitalWrite(pinLB,LOW); 
            //digitalWrite(pinRB,HIGH); // wheel on the right moves forward
            
            analogWrite(pinLF, 175); // wheel on the left moves backward
            analogWrite(pinRF, 175); // wheel on the right moves forward
            delay(deg * 10);
            analogWrite(pinLF,0);
            analogWrite(pinRF,0);
}

/* move backward */
void back() {
            digitalWrite(pinLB,HIGH); // motor moves to left rear
            digitalWrite(pinRB,HIGH); // motor moves to right rear
            analogWrite(pinLF,255);
            analogWrite(pinRF,255);
}

/* oomp wiswas
    spin in place 45 degrees 
    stop
    range forward
    store value
    repeat until map increment is 1
*/

void mapSurroundings() {
      //ask_pin_low(); // read the low distance ahead
      delay(1000);
      mapTurn();
      detection();
      omniDirectionals[omniCount] = Fspeedd;
      omniCount++;
}

void omniDirectionalCalc() {
      Serial.print("forward: ");
      Serial.print(omniDirectionals[0]);
      Serial.print(" & back: ");
      Serial.print(omniDirectionals[4]);
      Serial.print("\r\nso the difference is: ");
      Serial.print(omniDirectionals[0] - omniDirectionals[4]);
      Serial.print("\r\n");
      
      Serial.print("left: ");
      Serial.print(omniDirectionals[2]);
      Serial.print(" & right: ");
      Serial.print(omniDirectionals[6]);
      Serial.print("\r\nso the difference is: ");
      Serial.print(omniDirectionals[2] - omniDirectionals[6]);
      Serial.print("\r\n");

      Serial.print("\r\n mapincrem:");
      Serial.print(mapIncrem);


      // 1&5
      // 3&7          
}

/* turn 45 degrees, and reduce map increment */
void mapTurn() {
           // left();
           // directionn = 2;//Lgo; // move to the L
            incrSpinLeft(45);
            delay(200);
            mapIncrem--;
}

/* measure 3 angles (0.90.179) */
void detection() {
            int delay_time = 250; // stabilizing time for servo motor after moving backward
            ask_pin_F(); // read the distance ahead

             // if distance ahead is <10cm
            if(Fspeedd < 10) {
                            stopp(); // clear data  
                            delay(100);
                            back(); // move backward for 0.2S
                            delay(200);
            }

             // if distance ahead is <25cm
            if(Fspeedd < 25) {
                            stopp();
                            delay(100); // clear data
                            ask_pin_L(); // read distance on the left
                            delay(delay_time); // stabilizing time for servo motor
                            ask_pin_R(); // read distance on the right
                            delay(delay_time); // stabilizing time for servo motor

                            // if distance on the left is >distance on the right
                            if(Lspeedd > Rspeedd) {
                                                directionn = Lgo; // move to the L
                            }
                            
                            // if distance on the left is <= distance on the right
                            if(Lspeedd <= Rspeedd) {
                                                directionn = Rgo; // move to the right
                            }
                            
                            // if distance on left and right are both <10cm
                            if (Lspeedd < 10 && Rspeedd < 10) {
                                                directionn = Bgo; // move backward
                            } 
              } else {
                            // if distance ahead is >25cm
                            directionn = Fgo; // move forward
              }
}

/* measure the distance ahead */
// OOMP refactored to ask_pin_LW & ask_pin_HI

void ask_pin_low() {
              myservo.write(90);
              digitalWrite(outputPin, LOW); // ultrasonic sensor transmit low level signal 2μs
              delayMicroseconds(2);
              digitalWrite(outputPin, HIGH); // ultrasonic sensor transmit high level signal10μs, at least 10μs
              delayMicroseconds(10);
              digitalWrite(outputPin, LOW); // keep transmitting low level signal
              float Fdistance = pulseIn(inputPin, HIGH); // read the time in between
              Fdistance= Fdistance/5.8/10; // convert time into distance (unit: cm)
              Fspeedd = Fdistance; // read the distance into Fspeedd
            //!!!! edit
              Fspeedd = 5;
              Serial.print("LOW Distance :: "); //
              Serial.print(Fspeedd );
              Serial.println(" cm");
}

void ask_pin_hi() {
              myservo.write(90);
              digitalWrite(outputPin, LOW); // ultrasonic sensor transmit low level signal 2μs
              delayMicroseconds(2);
              digitalWrite(outputPin, HIGH); // ultrasonic sensor transmit high level signal10μs, at least 10μs
              delayMicroseconds(10);
              digitalWrite(outputPin, LOW); // keep transmitting low level signal
              float Fdistance = pulseIn(inputPin, HIGH); // read the time in between
              Fdistance= Fdistance/5.8/10; // convert time into distance (unit: cm)
              Fspeedd = Fdistance; // read the distance into Fspeedd
              Serial.print("High Distance :: "); //
              Serial.print(Fspeedd );
              Serial.println(" cm");
}

void ask_pin_F() {
              myservo.write(90);
              digitalWrite(outputPin, LOW); // ultrasonic sensor transmit low level signal 2μs
              delayMicroseconds(2);
              digitalWrite(outputPin, HIGH); // ultrasonic sensor transmit high level signal10μs, at least 10μs
              delayMicroseconds(10);
              digitalWrite(outputPin, LOW); // keep transmitting low level signal
              float Fdistance = pulseIn(inputPin, HIGH); // read the time in between
              Fdistance= Fdistance/5.8/10; // convert time into distance (unit: cm)
              Fspeedd = Fdistance; // read the distance into Fspeedd
              Serial.print("Fspeedd = ");
              Serial.print(Fspeedd );
              Serial.println(" cm, incr: ");
              Serial.print( mapIncrem );
              Serial.println(" ");
}

/* OOMP:: deprecate for just a low & high measurement */

/* measure distance on the left */
void ask_pin_L() {
              myservo.write(5);
              delay(delay_time);
              digitalWrite(outputPin, LOW); // ultrasonic sensor transmit low level signal 2μs
              delayMicroseconds(2);
              digitalWrite(outputPin, HIGH); // ultrasonic sensor transmit high level signal10μs, at least 10μs
              delayMicroseconds(10);
              digitalWrite(outputPin, LOW); // keep transmitting low level signal
              
              float Ldistance = pulseIn(inputPin, HIGH); // read the time in between
                    Ldistance= Ldistance/5.8/10; // convert time into distance (unit: cm)
                    Lspeedd = Ldistance; // read the distance into Lspeedd
              
              Serial.print("Lspeedd = ");
              Serial.print(Lspeedd );
              Serial.print(" cm ");
}

/* measure distance on the right */
void ask_pin_R() {
              myservo.write(177);
              delay(delay_time);
              digitalWrite(outputPin, LOW); // ultrasonic sensor transmit low level signal 2μs
              delayMicroseconds(2);
              digitalWrite(outputPin, HIGH); // ultrasonic sensor transmit high level signal10μs, at least 10μs
              delayMicroseconds(10);
              digitalWrite(outputPin, LOW); // keep transmitting low level signal
              
              float Rdistance = pulseIn(inputPin, HIGH); // read the time in between
                    Rdistance= Rdistance/5.8/10; // convert time into distance (unit: cm)
                    Rspeedd = Rdistance; // read the distance into Rspeedd
            
              Serial.print(" Rspeedd = ");
              Serial.print( Rspeedd );
              Serial.println(" cm");
}

/* set a loop */ 
void loop() {
              myservo.write(90); // home set the servo motor, ready for next measurement
              //detection(); // measure the angle and determine which direction to move
              if(mapIncrem > 1) {
                  mapSurroundings();  
              } else if(mapIncrem == 1) {
                  omniDirectionalCalc();
                  mapIncrem--;
              }
              
          /*  
          8 = move forward
          6 = move to the right
          4 = move to the left
          2 = move back

          */
              // if directionn= 2
              if(directionn == 2) {
                              //back();
                              //delay(800); // go backward
                              //left();
                          //  incrSpinLeft(45);
                            //  delay(200); // Move slightly to the left (to prevent stuck in dead end)
              }

              // if directionn = 6
/*              if(directionn == 6) { 
                              back();
                              delay(100);
                              right();
                              delay(600); // turn right
              }
*/              
              // if directionn = 4
/*              if(directionn == 4) { 
                              back();
                              delay(600);
                              left();
                              delay(600); // turn left
               }
*/
               // if directionn = 8
/*               if(directionn == 8) { 
                              advance(); // move forward
                              delay(100);
               }
*/
}
