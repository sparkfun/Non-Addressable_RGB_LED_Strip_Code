/******************************************************************************
  Example2_RGB-FadeLED.ino

  Non-Addressable RGB LED Custom Color Fade
  WRITTEN BY: Ho Yun "Bobby" Chan @ SparkFun Electronics
  DATE: November 4, 2019
  GITHUB REPO: https://github.com/sparkfun/Non-Addresssable_RGB_LED_Strip_Code
  DEVELOPMENT ENVIRONMENT SPECIFICS:
    Firmware developed using Arduino IDE v1.8.9

  ============================== DESCRIPTION ==============================
  Expand your color options using analogWrite() and a non-addressable RGB LED.
  This code will fade through 12 colors and white. There are options to adjust
  the brightness, type of LED, and rate at which the colors fade.

  This example code works with an individual common anode and common cathode
  RGB LED. If you have a transistor or constant current LED driver, you can
  also use it to control an RGB LED strip or a higher power RGB LED.
  We'll assume that you are using a common anode LEDs in the strip. For more
  information checkout our tutorial: https://learn.sparkfun.com/tutorials/731

  Notes: There are twelve rainbow colors (primary, secondary, tertiary).
  Unlike digitalWrite(), which can be only HIGH (on) or LOW (off),
  analogWrite() lets you smoothly change the brightness from 0 (off) to 255 (fully on).
  When analogWrite() is used with the RGB LED, you can create millions of colors!
  For simplicity, we'll use 12 rainbow colors and white.  We will be fading
  between each color.

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
float brightnessLED = 0.1;

//Create variables for type of LED and if it is used with a transistor
boolean commonAnode = false;
boolean commonCathode = true;//i.e.) When pin is HIGH, LED will also go HIGH without a transistor/PicoBuck

// Note:
//  Common Anode is `commonAnode`
//  Common Cathode LED is `commonCathode`
//  Common Anode RGB LED Strip with transistor is `!commonAnode`
//  RGB High Power LED with PicoBuck is also  `!commonAnode`
boolean rgbType = !commonAnode;

int colorMode = 1; //color mode to control LED color

//Variables for fading LED
int prevFadeVal = 0;
int currentFadeVal = 0;
boolean increasing = true;
int fadeVal = 5; //value to step when increasing/decreasing, recommended to be 1 or 5, larger numbers will have problems lighting up
int fadeMAX = 255; //maximum fade value
int fadeMIN = 0;   //minimum fade value
int fadeDelay = 30;//delay between each step




void setup() {

  // Make all of our LED pins outputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  allOFF(); //make sure to initialize LEDs with it turned off
  rgbCalc();//calculate for RGB type
  rgbShow(); //make sure to show it happening

#if DEBUG
  Serial.begin(9600); //initialize Serial Monitor
  //while (!Serial); // Comment out to wait for serial port to connect to Serial Monitor. Option for native USB.
  Serial.println("Custom Color Fading w/ an RGB LED.");
  Serial.println(" ");
  Serial.println("Note: Make sure to adjust the code for a common cathode or common anode.");
  Serial.println(" ");
#endif

}//end setup()

void loop()
{


  switch (colorMode) {
    case 1://FADE RED
      redValue = currentFadeVal;
      greenValue = 0;
      blueValue = 0;

      rgbCalc();
      break;
    //========== END FADE RED ==========

    case 2://FADE ORANGE
      redValue = currentFadeVal;
      greenValue = currentFadeVal * 0.498; // 128/255 = ~0.498039
      blueValue = 0;

      rgbCalc();

      if (redValue > 0 && greenValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        redValue = 0;
      }

      // takes x amount of steps if you do not set it to zero for certain brightness (i.e. takes 8 more steps to turn off for 0.1)
      //Serial.print("Red Value =");
      //Serial.println( int((currentFadeVal) * brightnessLED));

      //Serial.print("Green Value =");
      //Serial.println( int((currentFadeVal * 0.498) * brightnessLED));
      break;
    //========== END FADE ORANGE ==========

    case 3://FADE YELLOW
      redValue = currentFadeVal;
      greenValue = currentFadeVal;
      blueValue = 0;

      rgbCalc();
      break;
    //========== END FADE YELLOW ==========

    case 4://FADE CHARTRUESE
      redValue = currentFadeVal * 0.498; // 128/255 = ~0.498039
      greenValue = currentFadeVal;
      blueValue = 0;

      rgbCalc();

      if (greenValue > 0 && redValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        greenValue = 0;
      }
      break;
    //========== END FADE CHARTRUESE ==========

    case 5://FADE GREEN
      redValue = 0;
      greenValue = currentFadeVal;
      blueValue = 0;

      rgbCalc();
      break;
    //========== END FADE GREEN ==========

    case 6://FADE SPRING GREEN
      redValue = 0;
      greenValue = currentFadeVal;
      blueValue = currentFadeVal * 0.498; // 128/255 = ~0.498039

      rgbCalc();

      if (greenValue > 0 && blueValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        greenValue = 0;
      }
      break;
    //========== END FADE SPRING GREEN ==========

    case 7://FADE CYAN
      redValue = 0;
      greenValue = currentFadeVal;
      blueValue = currentFadeVal;

      rgbCalc();
      break;
    //========== END FADE CYAN ==========

    case 8://FADE AZURE
      redValue = 0;
      greenValue = currentFadeVal * 0.498; // 128/255 = ~0.498039
      blueValue = currentFadeVal;

      rgbCalc();

      if (blueValue > 0 && greenValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        blueValue = 0;
      }
      break;
    //========== END FADE AZURE ==========

    case 9://FADE BLUE
      redValue = 0;
      greenValue = 0;
      blueValue = currentFadeVal;

      rgbCalc();
      break;
    //========== END FADE BLUE ==========

    case 10://FADE VIOLET
      redValue = currentFadeVal * 0.498;
      greenValue = 0;
      blueValue = currentFadeVal;

      rgbCalc();

      if (blueValue > 0 && redValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        blueValue = 0;
      }
      break;
    //========== END FADE VIOLET ==========

    case 11://FADE MAGENTA
      redValue = currentFadeVal;
      greenValue = 0;
      blueValue = currentFadeVal;

      rgbCalc();
      break;
    //========== END FADE MAGENTA ==========

    case 12://FADE ROSE
      redValue = currentFadeVal;
      greenValue = 0;
      blueValue = currentFadeVal * 0.498;

      rgbCalc();

      if (redValue > 0 && blueValue == 0) {
        //tertiary component is 1/2, so when it calculates to decimal with fade value,
        //it will be basically be off, make sure to turn off other color so that
        //it does not just show the other color
        redValue = 0;
      }
      break;
    //========== END FADE ROSE ==========

    case 13://FADE WHITE
      redValue = currentFadeVal;
      greenValue = currentFadeVal;
      blueValue = currentFadeVal;

      rgbCalc();
      break;
    //========== END FADE WHITE ==========

    default:
      allOFF();
      rgbCalc();
      break;
  }



#if DEBUG
  Serial.print("Color Fading = ");
  if (colorMode == 1) {
    Serial.print("RED");
  }
  else if (colorMode == 2) {
    Serial.print("ORANGE");
  }
  else if (colorMode == 3) {
    Serial.print("YELLOW");
  }
  else if (colorMode == 4) {
    Serial.print("CHARTRUESE");
  }
  else if (colorMode == 5) {
    Serial.print("GREEN");
  }
  else if (colorMode == 6) {
    Serial.print("SPRING GREEN");
  }
  else if (colorMode == 7) {
    Serial.print("CYAN");
  }
  else if (colorMode == 8) {
    Serial.print("AZURE");
  }
  else if (colorMode == 9) {
    Serial.print("BLUE");
  }
  else if (colorMode == 10) {
    Serial.print("VIOLET");
  }
  else if (colorMode == 11) {
    Serial.print("MAGENTA");
  }
  else if (colorMode == 12) {
    Serial.print("ROSE");
  }
  else if (colorMode == 13) {
    Serial.print("WHITE");
  }
  else {
    Serial.print("OFF");
  }
  Serial.print(" | Brightness % = ");
  Serial.print(brightnessLED * 100);
  Serial.print("%");

  Serial.print(" | Fade Val Before Calc= ");
  Serial.println(currentFadeVal);
#endif

  rgbShow();
  delay(fadeDelay);


  if (increasing == true) {
    //increasing
    currentFadeVal += fadeVal;
  }
  else {
    //decreasing
    currentFadeVal -= fadeVal;
  }

  if (currentFadeVal > fadeMAX) {
    increasing = false;
    prevFadeVal -= fadeVal;//undo addition

    currentFadeVal = prevFadeVal;
  }
  else if (currentFadeVal < fadeMIN) {
    increasing = true;
    prevFadeVal += fadeVal;//undo subtraction

    currentFadeVal = prevFadeVal;

    colorMode += 1;//next color
    if (colorMode > 13) {
      colorMode = 0;
    }
  }

  prevFadeVal = currentFadeVal;

}//END LOOP




// ==================== CUSTOM FUNCTIONS DEFINED BELOW ====================
void allOFF() {
  // Black (all LEDs off)
  // RGB LEDs:
  redValue = 0;
  greenValue =  0;
  blueValue = 0;

  rgbCalc();
}

void redON() {
  // Red
  redValue = 255;
  greenValue =  0;
  blueValue = 0;

  rgbCalc();
}

void orangeON() {
  // Orange
  redValue = 255;
  greenValue = 128;
  blueValue = 0;

  rgbCalc();
}

void yellowON() {
  // Yellow
  redValue = 255;
  greenValue = 255;
  blueValue = 0;

  rgbCalc();
}

void chartrueseON() {
  // Chartruese
  redValue = 128;
  greenValue = 255;
  blueValue = 0;

  rgbCalc();
}

void greenON() {
  // Green
  redValue = 0;
  greenValue = 255;
  blueValue = 0;

  rgbCalc();
}

void springGreenON() {
  // Spring Green
  redValue = 0;
  greenValue = 255;
  blueValue = 128;

  rgbCalc();
}

void cyanON() {
  // Cyan
  redValue = 0;
  greenValue = 255;
  blueValue = 255;

  rgbCalc();
}

void azureON() {
  // Azure
  redValue = 0;
  greenValue = 128;
  blueValue = 255;

  rgbCalc();
}

void blueON() {
  // Blue
  redValue = 0;
  greenValue = 0;
  blueValue = 255;

  rgbCalc();
}

void violetON() {
  // Violet
  redValue = 128;
  greenValue = 0;
  blueValue = 255;

  rgbCalc();
}

void magentaON() {
  // Magenta
  redValue = 255;
  greenValue = 0;
  blueValue = 255;

  rgbCalc();
}

void roseON() {
  // Rose
  redValue = 255;
  greenValue = 0;
  blueValue = 128;

  rgbCalc();
}

void whiteON() {
  // White (all LEDs on)
  redValue = 255;
  greenValue = 255;
  blueValue = 255;

  rgbCalc();
}



void rgbCalc() {
  //use this to correctly light up LED depending on the setup
  if (rgbType == commonAnode) {
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

  redValue = int(redValue * brightnessLED);
  greenValue = int(greenValue * brightnessLED);
  blueValue = int(blueValue * brightnessLED);
}

void rgbShow() {
  //once value is calculated, show the LED color
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
