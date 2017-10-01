#include <CustomStepper.h> ///use to stop steppers
#include <Stepper.h>
const int stepsPerRevolution = 200;

Stepper stepperone(stepsPerRevolution, 8, 9, 10, 11); // pink, orange, yellow, blue  -wires
Stepper steppertwo(stepsPerRevolution, 53,47, 49,51);  // pink,orange, yellow, blue  -wires

// LDR;
// name = analogpin;
int topleft= 0; // ldr1 green/black
int topright= 1; // ldr2 yellow/red
int bottomleft= 2; // ldr3 white/green
int bottomright= 3 ;// ldr4 orange/black

void setup()
{
  Serial.begin(9600); //initialize serial port
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  pinMode(49, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(53, OUTPUT);
  pinMode(47, OUTPUT);

  stepperone.setSpeed(90); // setting the speed of stepper 
  steppertwo.setSpeed(100);
  
}
void loop()
{
int topl = analogRead(topleft);         // sets topl to the input value from ldr1
int topr = analogRead(topright);        // sets topr to the input value from ldr2
int botl = analogRead(bottomleft);      // sets botl to the input value from ldr3
int botr = analogRead(bottomright);     // sets botr to the input value from ldr4
int delayt = 2000;                        //time delay
int sensitivity = 0.001;                  // used to ensure the values are significantly greater
int index;
int averagetop;
int averagebot;
int averageleft;
int averageright;

int state(0);   // INDICATOR TO SHOW POSITION     1- north      2- east        3- south       4- west
int vstate(0);  // INDICATOR TO SHOW TILT OF PANEL   0 - flat   1 - 15deg   2 - 45 deg    3 - 70deg    
int counter;
bool movement;  //used to stop movement
///// current motor rotations are only to test


////     AVERAGES     ////



///////   calculate average for top half    /////////
for (index=0 ; index<4 ; index++)
{
  averagetop+= (analogRead(topleft) + analogRead(topright));
  
}
averagetop = (averagetop/4);


///////   calculate average for bottom half   /////////
for (index=0 ; index<4 ; index++)
{
  averagebot+= (analogRead(bottomleft) + analogRead(bottomright) );
}
averagebot = (averagebot/4);


////////    calculate average for left half   ///////
for (index=0 ; index<4 ; index++)
{
  averageleft += (analogRead(bottomleft) + analogRead(topleft));
}
averageleft = (averageleft/4);


///////   calculate average for right half    ///////
for (index=0 ; index < 4; index++)
{
  averageright += (analogRead(bottomright) + analogRead(topright));
}
averageright = (averageright/4);



////      MOTOR ROTATIONS   ////

////Horizontal rotation

/////////   compare top and bottom      //////////

//// TOP  1 v 3 ////

if (averagetop > averagebot)
{
  if (state == 1 && counter > 0)  // if state is north
  {
    // do nothing
  }
  else if (state == 2 && counter > 0)  // if state is east
  {
    stepperone.step(-1000); // rotate -1/4 turn to north
  }
  else if (state == 3 && counter > 0)  // if state is south
  {
    stepperone.step(2000);  // rotate 1/2 turn to north
  }
  else if (state == 4 && counter > 0)
  {
    stepperone.step(1000); // roate 1/4 turn to north
  }
  /// initialize position at north therefore no if statement needed to position north
  state = 1;
  delay(delayt);
  
}
//// BOTTOM 3 v 1////

else if (averagebot > averagetop)
{
  if (state == 3 && counter > 0) 
  {
    
  }
  
  else if (state == 1 && counter > 0) // if state is north
  {
    stepperone.step(-2000);  // rotate 1/2 turn to south
  }
  else if (state == 2 && counter > 0) //if state is east
  {
    stepperone.step(1000);  // rotate 1/4 turn to south
  }
  else if (state == 4 && counter > 0)  // if state is west
  {
    stepperone.step(-1000); // rotate -1/4 turn to south
  }
  else if (counter == 0)
  {
  stepperone.step(2000); //roate 1/2 turn to south
  }
  state = 3;  //set state south
  delay(delayt);
  
}


////////    compare left and right    /////////

////  LEFT  4 v 2 ////

if (averageleft > averageright)
{
  if (state = 4 && counter > 0)
  {
    
  }
  else if (state == 1 && counter > 0)  // if state is north
  {
    stepperone.step(-1000); // rotate -1/4 to west
  }
  else if (state == 2 && counter > 0)  // if state is east
  {
    stepperone.step(2000); //rotate 1/2 turn to west
  }
  else if (state == 3 && counter > 0) // if state is south
  {
    stepperone.step(1000);  // rotate 1/4 turn to west
  }
  else if (counter == 0) //panel is north initially
  {
    stepperone.step(-1000); // roate -1/4 turn to west
  }
  state = 4;
  delay(delayt);
}

////  RIGHT   2 v 4 ////

else if ( averageright > averageleft)
{
  if (state == 2 && counter > 0 )
  {
    
  }
  else if (state == 1) // if state is north
  {
    stepperone.step(1000);  // rotate 1/4 turn to east
    movement = true;
  }
  else if (state == 3) // if state is south
  {
    stepperone.step(-1000); // rotate -1/4 turn to east
    movement = true;
  }
  else if (state == 4) // if state is west
  {
    stepperone.step(-2000);  // rotate -1/2 turn to east
    movement = true;
  }
  state = 2;  // east
  delay (delayt);
}


//////////////////////////////////////////////////
////////                            //////////////
////////    VERTICAL ROTATIONS      //////////////
////////                            //////////////
//////////////////////////////////////////////////


if (averagetop > averagebot)
{
  if (vstate == 0)
  {
    if (averagetop > (averagebot + 0.6))
    {
      steppertwo.step(1750); // rotate from 0 to 3
      vstate = 3;
    }
   else if ( averagetop > (averagebot + 0.2))
   {
      steppertwo.step(750); //rotate from 0 to 2
      vstate = 2;
   }
   else if (averagetop > (averagebot ))
   {
      steppertwo.step(450);  //rotate from 0 to 1
      vstate = 1;
   }
  }
  /////// if panel is tilted
  else if (vstate == 1)
  {
   if ( averagetop > (averagebot + 0.6))
   {
      steppertwo.step(1300);  //rotate from 1 to 3
      vstate = 3;
   }
   else if (averagetop > (averagebot + 0.2))
   {
      steppertwo.step(300);  //rotate from 1 to 2
      vstate = 2;
   }
  }
  else if (vstate == 2)
  {
   if (averagetop > (averagebot + 0.6))
   {
      steppertwo.step(1000); //rotate from 2 to 3
      vstate = 3;
   }
   else if (averagetop > (averagebot + 0.2))
   {
      steppertwo.step(-300); //rotate from 2 to 1 
      vstate = 1;
   }
  }
  else if (vstate == 3)
  {
    if (averagetop > (averagebot + 0.6))
    {
      //// do nothing
    }
    else if (averagetop > (averagebot + 0.2))
    {
      steppertwo.step(-1000);  // rotate from 3 to 2
      vstate = 2;
    }
    else if (averagetop > (averagebot ))
    {
      steppertwo.step(-1300);  // rotate from 3 to 1
      vstate = 1;
    } 
  }
}



// bot vs top ///

if (averagebot > averagetop)
{
  if (vstate == 0)
  {
    if (averagebot > (averagetop + 0.6))
    {
      steppertwo.step(1750);  //rotate from 0 to 3
      vstate = 1;
    }
   else if ( averagebot > (averagetop + 0.2))
   {
      steppertwo.step(750);  // rotate from 0 to 2
      vstate = 2;
   }
   else if (averagebot > (averagetop ))
   {
      steppertwo.step(450);  // rotate from 0 to 1
      state = 1;
   }
  }
  /////// if panel is tilted
  else if (vstate == 1)
  {
   if ( averagebot > (averagetop + 0.6))
   {
      steppertwo.step(1300);  // rotate from 1 to 3
      vstate = 3;
   }
   else if (averagebot > (averagetop + 0.2))
   {
      steppertwo.step(300); // rotate from 1 to 2
      vstate = 2;
   }
  }
  else if (vstate == 2)
  {
   if (averagebot > (averagetop + 0.6))
   {
      steppertwo.step(1000);  // rotate from 2 to 3
      vstate = 3;
   }
   else if (averagebot > (averagetop + 0.2))
   {
      steppertwo.step(-300);  //rotate from 2 to 1
      vstate = 1;
   }
  }
   else if ( vstate == 3 )
   {
    if (averagebot > (averagetop + 0.6))
    {
      //do nothing
    }
    else if (averagebot > (averagetop + 0.2))
    {
      steppertwo.step(-1000);
      vstate = 2; // rotate from 3 to 2
    }
    else if (averagebot > (averagetop ))
    {
      steppertwo.step(-1300);
      vstate = 3; //rotate from 3 to 1
    }
   }
  
}


/// left vs right ///

if (averageleft > averageright)
{
  if (vstate == 0)
  {
    if (averageleft > (averageright + 0.6))
    {
      steppertwo.step(1750);  // rotate from 0 to 3
      vstate = 3;
    }
   else if ( averageleft > (averageright + 0.2))
   {
      steppertwo.step(750);  // rotate from 0 to 2
      vstate = 2;
   }
   else if (averageleft > (averageright ))
   {
      steppertwo.step(450);  // rotate from 0 to 1
      vstate = 1;
   }
  }
  /////// if panel is tilted
  else if (vstate == 1)
  {
   if ( averageleft > (averageright + 0.6))
   {
      steppertwo.step(1300);  // rotate from 1 to 3
      vstate = 3;
   }
   else if (averageleft > (averageright + 0.2))
   {
      steppertwo.step(300);  // rotate from 1 to 2
      vstate = 2;
   }
  }
  else if (vstate == 2)
  {
   if (averageleft > (averageright + 0.6))
   {
      steppertwo.step(1000); // rotate from 2 to 3
      vstate = 3;
   }
   else if (averageleft > (averageright + 0.2))
   {
      steppertwo.step(-300); // rotate from 2 to 1
      vstate = 1;
   }
  }
  else if (vstate == 3)
  {
    if (averageleft > (averageright + 0.6))
    {
      //do nothing
    }
    else if (averageleft > (averageright + 0.2))
    {
      steppertwo.step(-1000); // rotate from 3 to 2
      vstate = 2;
    }
    else if (averageleft > (averageright ))
    {
      steppertwo.step(-1300);  //rotate from  3 to 1
      vstate = 3;
    }
  }
}


/// right vs left ///

if (averageright > averageleft)
{
  if (vstate == 0)
  {
    if (averageright > (averageleft + 0.6))
    { 
      vstate = 3;
      steppertwo.step(1750); // rotate from 0 to 3
    }
   else if ( averageright > (averageleft + 0.2))
   {
      vstate = 2;
      steppertwo.step(750);  // rotate from 0 to 2
   }
   else if (averageright > (averageleft ))
   {
      steppertwo.step(450);  // rotate from 0 to 1
      vstate = 1;
   }
  }
  /////// if panel is tilted
  else if (vstate == 1)
  {
   if ( averageright > (averageleft + 0.6))
   {
      vstate = 3;
      steppertwo.step(1300);  // rotate from position 1 to 3
   }
   else if (averageright > (averageleft + 0.2))
   {
    vstate = 2;
    steppertwo.step(300);  // rotate from position 1 to 2
   }
  }
  else if (vstate == 2)
  {
   if (averageright > (averageleft + 0.6))
   {
    steppertwo.step(1000); // rotate from position 2 to 3
    vstate = 3;
   }
   else if (averageright > (averageleft ))
   {
    steppertwo.step(-300); //rotate from position 2 to 1
    vstate = 1;
   }
  }
  else if (vstate == 3 )
  {
    if (averageright > (averageleft + 0.6))
    {
      // do nothing
    }
    else if (averageright > (averageleft + 0.2))
    {
      steppertwo.step(-1000); // rotate from position 3 to 2
      vstate = 2;
    }
    else if (averageright > (averageleft ))
    {
      steppertwo.step(-1300); //rotate from positon 3 to 1
      vstate = 1;
    }
  }
}





///////  VERTICAL ROTATIONS BACK TO 0 ////////

if ( vstate > 0)
{
  if (vstate == 3)  
  {
    if (averagetop == averagebot && averageright == averageleft)
    {
     steppertwo.step(-175);  // rotate back to flat
    }
  }
  else if (vstate == 2)
  {
    if (averagetop == averagebot && averageright == averageleft)
    {
     steppertwo.step(-75);  // rotate back to flat
    }
  }
  else if (vstate == 1 )
  {
    if (averagetop == averagebot && averageright == averageleft)
    {
     steppertwo.step(-45);  //rotate back to flat
    }
  }
}


if (movement == true)
{
  delay(100);
}

counter++;   //increments counter to detect that the motors have moved
}
