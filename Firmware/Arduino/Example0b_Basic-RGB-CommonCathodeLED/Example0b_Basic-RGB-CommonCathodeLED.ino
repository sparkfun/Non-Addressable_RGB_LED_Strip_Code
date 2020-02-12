/******************************************************************************
  basic_rgb-led-common-cathode.ino

  Non-Addressable Basic RGB LED Common Cathode
  WRITTEN BY: Ho Yun "Bobby" Chan @ SparkFun Electronics
  DATE: November 4, 2019
  GITHUB REPO: https://github.com/sparkfun/Non-Addresssable_RGB_LED_Strip_Code
  DEVELOPMENT ENVIRONMENT SPECIFICS:
    Firmware developed using Arduino IDE v1.8.9

  ============================== DESCRIPTION ==============================
  In this code we'll cycle through 13 colors before turning off for a
  common cathode LED. This will work if you are using a common anode LED
  strip connected to a transistor. This will not work for a common anode LED.

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

// Define our LED pins to a PWM Pin
#define redPin 5
#define greenPin 6
#define bluePin 9

void setup() {

  // Make all of our LED pins outputs:
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600); //initialize Serial Monitor
  //while (!Serial); // Comment out to wait for serial port to connect to Serial Monitor. Option for native USB.
  Serial.println("Basic Custom Color Mixing with Common Cathode LEDs.");
  Serial.println(" ");
}

void loop()
{
  // In this code we'll step through twelve rainbow colors (primary, secondary, tertiary).

  // Unlike digitalWrite, which can be only HIGH (on) or LOW (off),
  // analogWrite lets you smoothly change the brightness from 0 (off) to 255 (fully on).
  // When analogWrite is used with the RGB LED, you can create millions of colors!

  // In the analogWrite() functions:
  // 0 is off
  // 128 is halfway on (used for the tertiary colors)
  // 255 is full brightness.

  //RED
  Serial.println("RED");
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  delay(1000);

  //ORANGE
  Serial.println("ORANGE");
  analogWrite(redPin, 255);
  analogWrite(greenPin, 128);
  analogWrite(bluePin, 0);
  delay(1000);


  //YELLOW
  Serial.println("YELLOW");
  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
  delay(1000);

  //CHARTRUESE
  Serial.println("CHARTRUESE");
  analogWrite(redPin, 128);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
  delay(1000);

  //GREEN
  Serial.println("GREEN");
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 0);
  delay(1000);

  //SPRING GREEN
  Serial.println("SPRING GREEN");
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 128);
  delay(1000);

  //CYAN
  Serial.println("CYAN");
  analogWrite(redPin, 0);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);
  delay(1000);


  //AZURE
  Serial.println("AZURE");
  analogWrite(redPin, 0);
  analogWrite(greenPin, 128);
  analogWrite(bluePin, 255);
  delay(1000);

  //BLUE
  Serial.println("BLUE");
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
  delay(1000);

  //VIOLET
  Serial.println("VIOLET");
  analogWrite(redPin, 128);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
  delay(1000);

  //MAGENTA
  Serial.println("MAGENTA");
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 255);
  delay(1000);

  //ROSE
  Serial.println("ROSE");
  analogWrite(redPin, 255);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 128);
  delay(1000);

  //WHITE
  Serial.println("WHITE");
  analogWrite(redPin, 255);
  analogWrite(greenPin, 255);
  analogWrite(bluePin, 255);
  delay(1000);

  //OFF
  Serial.println("OFF");
  analogWrite(redPin, 0);
  analogWrite(greenPin, 0);
  analogWrite(bluePin, 0);
  delay(1000);
}
