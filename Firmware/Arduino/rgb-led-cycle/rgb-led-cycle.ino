/******************************************************************************
  rgb-led-cycle.ino

  Non-Addressable RGB LED Custom Color Cycle
  WRITTEN BY: Ho Yun "Bobby" Chan @ SparkFun Electronics
  DATE: November 4, 2019
  GITHUB REPO: https://github.com/sparkfun/Non-Addresssable_RGB_LED_Strip_Code
  DEVELOPMENT ENVIRONMENT SPECIFICS:
    Firmware developed using Arduino IDE v1.8.9

  ============================== DESCRIPTION ==============================
  Expand your color options using analogWrite() and a non-addressable RGB LED.
  This code will cycle through 12 colors and white. There are options to adjust
  the brightness, type of LED, and rate at which the colors cycle.

  This example code works with an individual common anode and common cathode
  RGB LED. If you have a transistor or constant current LED driver, you can
  also use it to control an RGB LED strip or a higher power RGB LED.
  We'll assume that you are using a common anode LEDs in the strip. For more
  information checkout our tutorial: https://learn.sparkfun.com/tutorials/731

  Notes: There are twelve rainbow colors (primary, secondary, tertiary).
  Unlike digitalWrite(), which can be only HIGH (on) or LOW (off),
  analogWrite() lets you smoothly change the brightness from 0 (off) to 255 (fully on).
  When analogWrite() is used with the RGB LED, you can create millions of colors!
  For simplicity, we'll use 12 rainbow colors and white.  We will be blinking
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
float brightness_LED = 0.1;

//Create variables for type of LED and if it is used with a transistor
boolean common_anode = false;
boolean common_cathode = true;//i.e.) When pin is HIGH, LED will also go HIGH without a transistor/PicoBuck

// Note:
//  Common Anode is `common_anode`
//  Common Cathode LED is `common_cathode`
//  Common Anode RGB LED Strip with transistor is `!common_anode`
//  RGB High Power LED with PicoBuck is also  `!common_anode`
boolean RGB_type = !common_anode;

int blinkRate = 1000; //in milliseconds



void setup() {

  // Make all of our LED pins outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  allOFF(); //initialize LEDs with it turned off
  show_RGB(); //make sure to show it happening

#if DEBUG
  Serial.begin(9600); //initialize Serial Monitor
  //while (!Serial); // Comment out to wait for serial port to connect to Serial Monitor. Needed for native USB.
  Serial.println("Custom Color Cycling w/ an RGB LED. This example will cycle through 13 colors.");
  Serial.println(" ");
  Serial.println("Note: Make sure to adjust the code for a common cathode or common anode.");
  Serial.println(" ");
#endif

}//end setup()




void loop()
{
  //used to visually check when Arduino is initialized
#if DEBUG
  Serial.print("RED");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  redON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("ORANGE");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  orangeON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("YELLOW");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  yellowON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("CHARTRUESE");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  chartrueseON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("GREEN");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  greenON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("SPRING GREEN");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  springGreenON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("CYAN");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  cyanON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("AZURE");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  azureON();
  show_RGB();
  delay(blinkRate);


#if DEBUG
  Serial.print("BLUE");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  blueON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("VIOLET");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif

  violetON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("MAGENTA");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  magentaON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("ROSE");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  roseON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("WHITE");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  whiteON();
  show_RGB();
  delay(blinkRate);

#if DEBUG
  Serial.print("OFF");
  Serial.print(" | Brightness % = ");
  Serial.println(brightness_LED * 100);
#endif
  allOFF();
  show_RGB();
  delay(blinkRate);

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
