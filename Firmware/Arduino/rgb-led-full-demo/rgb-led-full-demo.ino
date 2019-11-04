/******************************************************************************
 rgb-led-full-demo.ino
  
  Non-Addressable RGB LED Full Demo
  WRITTEN BY: Ho Yun "Bobby" Chan @ SparkFun Electronics
  DATE: November 4, 2019
  GITHUB REPO: https://github.com/sparkfun/Non-Addresssable_RGB_LED_Strip_Code
  DEVELOPMENT ENVIRONMENT SPECIFICS:
    Firmware developed using Arduino IDE v1.8.9

============================== DESCRIPTION ==============================
  Expand your color options using analogWrite() and a non-addressable RGB LED.
  This code will either turn on a color, blink, fade, or cycle through 12
  colors and white depending on the button input. There are options to adjust
  the brightness, type of LED, and rate at which the colors blink/fade/cycle.

  This example code works with an individual common anode and common cathode
  RGB LED. If you have a transistor or constant current LED driver, you can
  also use it to control an RGB LED strip or a higher power RGB LED.
  We'll assume that you are using a common anode LEDs in the strip. For more
  information checkout our tutorial: https://learn.sparkfun.com/tutorials/731

  Notes: There are twelve rainbow colors (primary, secondary, tertiary).
  Unlike digitalWrite(), which can be only HIGH (on) or LOW (off),
  analogWrite() lets you smoothly change the brightness from 0 (off) to 255 (fully on).
  When analogWrite() is used with the RGB LED, you can create millions of colors!
  For simplicity, we'll use 12 rainbow colors and white.

  In the analogWrite() functions:
    0 is off
    128 is halfway on (used for the tertiary colors)
    255 is full brightness.

  ========== TUTORIAL ==========
  Non-Addressable RGB LED Strip Hookup Guide
  https://learn.sparkfun.com/tutorials/731

  Transistors | Applictions I: Switches
  https://learn.sparkfun.com/tutorials/transistors/all#applications-i-switches

  ==================== PRODUCTS THAT USE THIS CODE ====================
    LED RGB Strip (1M Bare) - https://www.sparkfun.com/products/12021
    LED RGB Strip (1M Sealed) - https://www.sparkfun.com/products/12023
    LED RGB Strip (5B Bare) - https://www.sparkfun.com/products/12022
    LED RGB Strip (5M Sealed) - https://www.sparkfun.com/products/12024
    Triple Output High Power RGB LED - https://www.sparkfun.com/products/15200

    PicoBuck LED Driver - https://www.sparkfun.com/products/13705
    N-Channel MOSFET Power Control Kit - https://www.sparkfun.com/products/12959

  ==================== HARDWARE CONNECTIONS ====================
  The hardware connection depends on your hardware and setup. Below is one possible
  arrangement.

  RGB Common Anode LED Strip => BJT/MOSFET => Arduino PWM Pin
      R pin => transistor => 5
      G pin => transistor => 6
      B pin => transistor => 9
      - pin                  -

  LICENSE: This code is released under the MIT License (http://opensource.org/licenses/MIT)

  ******************************************************************************/

//Debug mode, comment one of these lines out using a syntax
//for a single line comment ("//"):
//#define DEBUG 0     //0 = LEDs only
#define DEBUG 1     //1 = LEDs w/ serial output

// Define our LED pins to a PWM Pin
#define redPin 5
#define greenPin 6
#define bluePin 9

// Create integer variables for our LED color value
int redValue = 0;
int greenValue = 0;
int blueValue = 0;

//Create brightness variable
//Ranging from 0.0-1.0:
//  0.0 is off
//  0.5 is 50%
//  1.0 is fully on
float brightness_LED = 0.1;

//Create variables for type of LED and if it is used with a transistor
boolean common_anode = false;
boolean common_cathode = true;//i.e.) When pin is HIGH, LED will also go HIGH without a transistor/PicoBuck

// Note:
//  Common Anode LED is `common_anode`
//  Common Cathode LED is `common_cathode`
//  Common Anode RGB LED Strip with transistor is `!common_anode`
//  RGB High Power LED with PicoBuck is also  `!common_anode`
boolean RGB_type = !common_anode;

//Variables to keep track of color and pattern
int colorMode = 0; //color mode to control LED color
int pattern = 0; //pattern to turn off, stay on, fade, blink

//Variables for fading LED
int prev_FadeVal = 0;
int current_FadeVal = 0;
boolean increasing = true;
int fadeVal = 5; //value to step when increasing/decreasing, recommended to be 1 or 5, larger numbers will have problems lighting up
int fadeMAX = 255; //maximum fade value
int fadeMIN = 0;   //minimum fade value
int fadeDelay = 30;//delay between each step

//Variables for blinking LED
int blinkVal = 0;
boolean blinkON = false;
int counter = 0; //use as a "delay"
int blinkRate = 750; //in milliseconds

//Variables to transition between RGB in a rainbow
int rainbowRedVal = 0;
int rainbowGreenVal = 0;
int rainbowBlueVal = 0;
int rainbowTransitionVal = 0;
int rainbowDelay = 5; //in milliseconds to transition between colors

//Note: You'll want to not make `rainbowDelay` too long as this will
//      cause delays with button presses

//Button variables for color
const int button1Pin = 2;
boolean button1State = false;
boolean prev_button1State = false;
boolean current_button1State = false;

//Button variables for pattern
const int button2Pin = 3;
boolean button2State = false;
boolean prev_button2State = false;
boolean current_button2State = false;




void setup() {

  // Make all of our LED pins outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  if (RGB_type == common_anode) {
    //set values 255 to turn off OFF if common anode
    rainbowRedVal = 255;
    rainbowGreenVal = 255;
    rainbowBlueVal = 255;
  }

  sequenceTest();//visually initialization
  allOFF(); //make sure to initialize LEDs with it turned off
  show_RGB(); //make sure to show it happening

  pinMode(button1Pin, INPUT_PULLUP); //use internal pullup resistor with button
  pinMode(button2Pin, INPUT_PULLUP); //use internal pullup resistor with button

#if DEBUG
  Serial.begin(9600); //initialize Serial Monitor
  //while (!Serial); // Comment out to wait for serial port to connect to Serial Monitor. Needed for native USB.
  Serial.println("Custom Color Mixing Demo w/ an RGB LED. Toggling the buttons will adjust the color and pattern.");
  Serial.println(" ");
  Serial.println("Note: Make sure to adjust the code for a common cathode or common anode.");
  Serial.println("Default is set to no color and off!");
  Serial.println(" ");
#endif

}//end setup()




void loop()
{
  button1State = digitalRead(button1Pin);// button for Color Mode
  button2State = digitalRead(button2Pin);// button for Pattern

  //==================== CHECK BUTTON FOR COLOR MODE ====================
  //if button is pressed, it will be pulled low
  if (button1State == LOW) {
    current_button1State = true; // button has been pressed once

    if (prev_button1State != current_button1State) { //check to see if button is still being pressed
      colorMode = colorMode + 1; //change color MODE after button has been pressed
#if DEBUG
      Serial.print("Color Mode = ");
      if (colorMode == 1) {
        Serial.println("RED");
      }
      else if (colorMode == 2) {
        Serial.println("ORANGE");
      }
      else if (colorMode == 3) {
        Serial.println("YELLOW");
      }
      else if (colorMode == 4) {
        Serial.println("CHARTRUESE");
      }
      else if (colorMode == 5) {
        Serial.println("GREEN");
      }
      else if (colorMode == 6) {
        Serial.println("SPRING GREEN");
      }
      else if (colorMode == 7) {
        Serial.println("CYAN");
      }
      else if (colorMode == 8) {
        Serial.println("AZURE");
      }
      else if (colorMode == 9) {
        Serial.println("BLUE");
      }
      else if (colorMode == 10) {
        Serial.println("VIOLET");
      }
      else if (colorMode == 11) {
        Serial.println("MAGENTA");
      }
      else if (colorMode == 12) {
        Serial.println("ROSE");
      }
      else if (colorMode == 13) {
        Serial.println("WHITE");
      }
      else {
        Serial.println("OFF");
      }
#endif

      //Cycle through colors when pressing buttons
      if (colorMode < 0 || colorMode > 13) {
        //reset ledMode
        colorMode = 0;
        allOFF();
        show_RGB();
      }
    }
    else { //do nothing because finger is still on button
    }
    prev_button1State = current_button1State;//update button1 state
  }



  //button has not been pressed, it will be high
  else {
    current_button1State = false;
    prev_button1State = current_button1State;//update button1 state
  }

  //==================== END CHECK BUTTON FOR COLOR MODE ====================

  //==================== CHECK BUTTON FOR PATTERN ====================
  if (button2State == LOW) {
    current_button2State = true; //button has been pressed once

    if (prev_button2State != current_button2State) { //check to see if button is still being pressed
      pattern = pattern + 1; //change LED pattern after button has been pressed

#if DEBUG
      Serial.print("Pattern = ");
      if (pattern == 1) {
        Serial.println("ON");//print what pattern
      }
      else if (pattern == 2) {
        Serial.println("FADE");//print what pattern
      }
      else if (pattern == 3) {
        Serial.println("BLINK");//print what pattern
      }
      else if (pattern == 4) {
        Serial.println("RAINBOW");//print what pattern
      }
      else {
        Serial.println("OFF");//print what pattern
      }
#endif


      if (pattern < 0 || pattern > 4) {
        //reset pattern
        pattern = 0;
      }
    }



    else { //do nothing because finger is still on button
    }
    prev_button2State = current_button2State; //update button2 state
  }

  //button has not been pressed, it will be high
  else {
    current_button2State = false;
    prev_button2State = current_button2State; //update button2 state
  }
  switch (pattern) {
    case 1:
      patternON();
      break;
    case 2:
      patternFade();
      break;
    case 3:
      patternBlink();
      break;
    case 4:
      patternRainbow();
      break;
    default:
      allOFF();
      show_RGB();
      break;
  }
  //==================== END CHECK BUTTON FOR PATTERN ====================


}//end loop




// ==================== CUSTOM FUNCTIONS DEFINED BELOW ====================
void allOFF() {
  // Black (all LEDs off)
  // RGB LEDs:
  redValue = 0;
  greenValue =  0;
  blueValue = 0;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void redON() {
  // Red
  redValue = 255;
  greenValue =  0;
  blueValue = 0;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void orangeON() {
  // Orange
  redValue = 255;
  greenValue = 128;
  blueValue = 0;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void yellowON() {
  // Yellow
  redValue = 255;
  greenValue = 255;
  blueValue = 0;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void chartrueseON() {
  // Chartruese
  redValue = 128;
  greenValue = 255;
  blueValue = 0;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void greenON() {
  // Green
  redValue = 0;
  greenValue = 255;
  blueValue = 0;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void springGreenON() {
  // Spring Green
  redValue = 0;
  greenValue = 255;
  blueValue = 128;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void cyanON() {
  // Cyan
  redValue = 0;
  greenValue = 255;
  blueValue = 255;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void azureON() {
  // Azure
  redValue = 0;
  greenValue = 128;
  blueValue = 255;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void blueON() {
  // Blue
  redValue = 0;
  greenValue = 0;
  blueValue = 255;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void violetON() {
  // Violet
  redValue = 128;
  greenValue = 0;
  blueValue = 255;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void magentaON() {
  // Magenta
  redValue = 255;
  greenValue = 0;
  blueValue = 255;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void roseON() {
  // Rose
  redValue = 255;
  greenValue = 0;
  blueValue = 128;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}

void whiteON() {
  // White (all LEDs on)
  redValue = 255;
  greenValue = 255;
  blueValue = 255;

  calculate_RGB();

  redValue = int(redValue * brightness_LED);
  greenValue = int(greenValue * brightness_LED);
  blueValue = int(blueValue * brightness_LED);
}


//-------------------- sequenceTest() FUNCTION --------------------

void sequenceTest() {
  //used to visually check when Arduino is initialized
  redON();
  show_RGB();
  delay(50);

  orangeON();
  show_RGB();
  delay(50);

  yellowON();
  show_RGB();
  delay(50);

  chartrueseON();
  show_RGB();
  delay(50);

  greenON();
  show_RGB();
  delay(50);

  springGreenON();
  show_RGB();
  delay(50);

  cyanON();
  show_RGB();
  delay(50);

  azureON();
  show_RGB();
  delay(50);

  blueON();
  show_RGB();
  delay(50);

  violetON();
  show_RGB();
  delay(50);

  magentaON();
  show_RGB();
  delay(50);

  roseON();
  show_RGB();
  delay(50);

  whiteON();
  show_RGB();
  delay(50);

  allOFF();
  show_RGB();
  delay(50);
}//-------------------- END sequenceTest() FUNCTION --------------------

void calculate_RGB() {
  //use this to correctly light up LED depending on the setup
  if (RGB_type == common_anode) {
    /* If using a common anode LED, a pin
       should turn ON the LED when the pin is LOW.*/
    redValue = 255 - redValue;
    greenValue = 255 - greenValue;
    blueValue = 255 - blueValue;

  }
  else {
    /* If using a common cathode LED, an analog pin
       should turn on the LED when the pin is HIGH. The
       logic is flipped when using a Common Anode RGB LED
       strip, NPN BJT/N-Channel MOSFET, and microcontroller

       Leave RGB values as is, we're good!*/
  }
}

void show_RGB() {
  //once value is calculated, show the LED color
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

//-------------------- patternON() FUNCTION --------------------

void patternON() {
  // button is pressed, change LED color/sequence
  switch (colorMode)
  {
    case 1:
      redON();
      break;
    case 2:
      orangeON();
      break;
    case 3:
      yellowON();
      break;
    case 4:
      chartrueseON();
      break;
    case 5:
      greenON();
      break;
    case 6:
      springGreenON();
      break;
    case 7:
      cyanON();
      break;
    case 8:
      azureON();
      break;
    case 9:
      blueON();
      break;
    case 10:
      violetON();
      break;
    case 11:
      magentaON();
      break;
    case 12:
      roseON();
      break;
    case 13:
      whiteON();
      break;
    default:
      allOFF();
      break;
  }//end switch

  show_RGB();
}//-------------------- end patternON() FUNCTION --------------------




//-------------------- patternFade() FUNCTION --------------------
void patternFade() {

  switch (colorMode) {
    case 1://FADE RED
      redValue = current_FadeVal;
      greenValue = 0;
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 2://FADE ORANGE
      redValue = current_FadeVal;
      greenValue = current_FadeVal * 0.498; // 128/255 = ~0.498039
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);

      if (redValue > 0 && greenValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        redValue = 0;
      }
      // takes x amount of steps if you do not set it to zero for certain brightness (i.e. takes 8 more steps to turn off for 0.1)
      //Serial.print("Red Value =");
      //Serial.println( int((current_FadeVal) * brightness_LED));

      //Serial.print("Green Value =");
      //Serial.println( int((current_FadeVal * 0.498) * brightness_LED));
      break;
    case 3://FADE YELLOW
      redValue = current_FadeVal;
      greenValue = current_FadeVal;
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 4://FADE CHARTRUESE
      redValue = current_FadeVal * 0.498; // 128/255 = ~0.498039
      greenValue = current_FadeVal;
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);

      if (greenValue > 0 && redValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        greenValue = 0;
      }
      break;
    case 5://FADE GREEN
      redValue = 0;
      greenValue = current_FadeVal;
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 6://FADE SPRING GREEN
      redValue = 0;
      greenValue = current_FadeVal;
      blueValue = current_FadeVal * 0.498; // 128/255 = ~0.498039

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);

      if (greenValue > 0 && blueValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        greenValue = 0;
      }
      break;
    case 7://FADE CYAN
      redValue = 0;
      greenValue = current_FadeVal;
      blueValue = current_FadeVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 8://FADE AZURE
      redValue = 0;
      greenValue = current_FadeVal * 0.498; // 128/255 = ~0.498039
      blueValue = current_FadeVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);

      if (blueValue > 0 && greenValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        blueValue = 0;
      }
      break;
    case 9://FADE BLUE
      redValue = 0;
      greenValue = 0;
      blueValue = current_FadeVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 10://FADE VIOLET
      redValue = current_FadeVal * 0.498;
      greenValue = 0;
      blueValue = current_FadeVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);// 128/255 = ~0.498039
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);

      if (blueValue > 0 && redValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        blueValue = 0;
      }
      break;
    case 11://FADE MAGENTA
      redValue = current_FadeVal;
      greenValue = 0;
      blueValue = current_FadeVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 12://FADE ROSE
      redValue = current_FadeVal;
      greenValue = 0;
      blueValue = current_FadeVal * 0.498;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);// 128/255 = ~0.498039

      if (redValue > 0 && blueValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        redValue = 0;
      }
      break;
    case 13://FADE WHITE
      redValue = current_FadeVal;
      greenValue = current_FadeVal;
      blueValue = current_FadeVal;

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    default:
      allOFF();
      break;
  }

  show_RGB();
  delay(fadeDelay);


  if (increasing == true) {
    current_FadeVal += fadeVal;
  }
  else { //decreasing
    current_FadeVal -= fadeVal;
  }

  if (current_FadeVal > fadeMAX) {
    increasing = false;
    prev_FadeVal -= fadeVal;//undo addition

    current_FadeVal = prev_FadeVal;

  }
  else if (current_FadeVal < fadeMIN) {
    increasing = true;
    prev_FadeVal += fadeVal;//unto subtraction

    current_FadeVal = prev_FadeVal;
  }

  prev_FadeVal = current_FadeVal;
}//-------------------- END patternFade() FUNCTION --------------------




//-------------------- patternBlink() FUNCTION --------------------
void patternBlink() {

  switch (colorMode) {
    case 1://RED
      redValue = blinkVal;
      greenValue = 0;
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 2://ORANGE
      redValue = blinkVal;
      greenValue = blinkVal  * 0.498;
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 3://YELLOW
      redValue = blinkVal;
      greenValue = blinkVal;
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 4://CHARTREUSE
      redValue = blinkVal * 0.498;
      greenValue = blinkVal;
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 5://GREEN
      redValue = 0;
      greenValue = blinkVal;
      blueValue = 0;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 6://SRING GREEN
      redValue = 0;
      greenValue = blinkVal;
      blueValue = blinkVal * 0.498;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int((blueValue) * brightness_LED);
      break;
    case 7://CYAN
      redValue = 0;
      greenValue = blinkVal;
      blueValue = blinkVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 8://AZURE
      redValue = 0;
      greenValue = blinkVal * 0.498;
      blueValue = blinkVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 9://BLUE
      redValue = 0;
      greenValue = 0;
      blueValue = blinkVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 10://VIOLET
      redValue = blinkVal  * 0.498;
      greenValue = 0;
      blueValue = blinkVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 11://MAGENTA
      redValue = blinkVal;
      greenValue = 0;
      blueValue = blinkVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 12://ROSE
      redValue = blinkVal;
      greenValue = 0;
      blueValue = blinkVal * 0.498;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    case 13://WHITE
      redValue = blinkVal;
      greenValue = blinkVal;
      blueValue = blinkVal;

      calculate_RGB();

      redValue = int(redValue * brightness_LED);
      greenValue = int(greenValue * brightness_LED);
      blueValue = int(blueValue * brightness_LED);
      break;
    default:
      allOFF();
      break;
  }

  show_RGB();

  if (counter == blinkRate) {
    if (blinkON == true) {
      blinkVal = 0;
      blinkON = false;
    }
    else { //it was on, so turn off
      blinkVal = 255;
      blinkON = true;
    }
    counter = 0;
  }
  else {
    counter = counter + 1;
  }
}//-------------------- patternBlink() FUNCTION //--------------------



//-------------------- patternRainbow() FUNCTION --------------------
void patternRainbow() {
  if (RGB_type == common_cathode) {
    if (rainbowTransitionVal == 0) {
      //RED
      rainbowRedVal += 5;
      if (rainbowRedVal >= 255) {
        rainbowTransitionVal = 1;
      }
    }
    else if (rainbowTransitionVal == 1) {
      //RED TO ORANGE TO YELLOW
      rainbowGreenVal += 5;

      if (rainbowGreenVal >= 255) {
        rainbowTransitionVal = 2;
      }
    }
    else if (rainbowTransitionVal == 2) {
      //YELLOW to CHARTREUSE to GREEN
      rainbowRedVal -= 5;

      if (rainbowRedVal <= 0) {
        rainbowTransitionVal = 3;
      }
    }
    else if (rainbowTransitionVal == 3) {
      //GREEN to SPRING GREEN to CYAN
      rainbowBlueVal += 5;

      if (rainbowBlueVal >= 255) {
        rainbowTransitionVal = 4;
      }
    }
    else if (rainbowTransitionVal == 4) {
      //CYAN to AZURE to BLUE
      rainbowGreenVal -= 5;

      if (rainbowGreenVal <= 0) {
        rainbowTransitionVal = 5;
      }
    }
    else if (rainbowTransitionVal == 5) {
      //BLUE to VIOLET to MAGENTA
      rainbowRedVal += 5;

      if (rainbowRedVal >= 255) {
        rainbowTransitionVal = 6;
      }
    }
    else if (rainbowTransitionVal == 6) {
      //MAGENTA to ROSE to RED
      rainbowBlueVal -= 5;

      if (rainbowBlueVal <= 0) {
        rainbowTransitionVal = 1;
      }
    }
  }//end check for common_cathode



  else {
    if (rainbowTransitionVal == 0) {
      //RED
      rainbowRedVal -= 5;
      if (rainbowRedVal <= 0) {
        rainbowTransitionVal = 1;
      }
    }
    else if (rainbowTransitionVal == 1) {
      //RED TO ORANGE TO YELLOW
      rainbowGreenVal -= 5;

      if (rainbowGreenVal <= 0) {
        rainbowTransitionVal = 2;
      }
    }
    else if (rainbowTransitionVal == 2) {
      //YELLOW to CHARTREUSE to GREEN
      rainbowRedVal += 5;

      if (rainbowRedVal >= 255) {
        rainbowTransitionVal = 3;
      }
    }
    else if (rainbowTransitionVal == 3) {
      //GREEN to SPRING GREEN to CYAN
      rainbowBlueVal -= 5;

      if (rainbowBlueVal <= 0) {
        rainbowTransitionVal = 4;
      }
    }
    else if (rainbowTransitionVal == 4) {
      //CYAN to AZURE to BLUE
      rainbowGreenVal += 5;

      if (rainbowGreenVal >= 255) {
        rainbowTransitionVal = 5;
      }
    }
    else if (rainbowTransitionVal == 5) {
      //BLUE to VIOLET to MAGENTA
      rainbowRedVal -= 5;

      if (rainbowRedVal <= 0) {
        rainbowTransitionVal = 6;
      }
    }
    else if (rainbowTransitionVal == 6) {
      //MAGENTA to ROSE to RED
      rainbowBlueVal += 5;


      if (rainbowBlueVal >= 255) {
        rainbowTransitionVal = 1;
      }
    }
  }//end check for common_anode

  redValue = int(rainbowRedVal * brightness_LED);
  greenValue = int(rainbowGreenVal * brightness_LED);
  blueValue = int(rainbowBlueVal * brightness_LED);

  show_RGB();

  delay(rainbowDelay);
}//-------------------- END patternRainbow() FUNCTION --------------------


// ==================== END CUSTOM FUNCTIONS DEFINED ====================
