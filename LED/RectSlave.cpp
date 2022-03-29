#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <RF24.h>
#include <spi.h>
#include <RF24Network.h>

//needs to get the signal from the master
// apply the the rb
RF24 radio(A0, 10);
RF24Network network(radio);

const uint16_t this_node = 02;
const uint16_t master = 00;

//setting up LED
#define LED_PIN 9
#define LED_COUNT 288
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// have the rectangle
// - each strip is 24 in length 8strips in total
//    - each bar is 48 in length
// - Have a way to control each strip

//  →         direction
//  04======15↓
// ↑37======26
//         ←  direction

// 01 bar -> the location of the mcu

void setup()
{
  // put your setup code here, to run once:
  SPI.begin();
  Serial.begin(9600);

  radio.begin();
  network.begin(90, this_node);

  strip.begin();
  strip.show();
}

void loop()
{
  network.update();
  while (network.available())
  {
    RF24NetworkHeader header;
    // Each strip is represented by an element in the array
    // The colour and brigness will also be represent in an array
    // expected to get int data_array [12] = [0,1,2,3,4,5,6,7,r,b,g,a]

    // also need something else to tell me to when to do a
    // a ladder like a loading
    // maybe have different modes?

    int data_array[9];
    network.read(header, &data_array, sizeof(data_array));
    uint32_t c = strip.Color(data_array[5], data_array[6], data_array[7]);
    strip.setBrightness(data_array[8]);

    for (int i = 0; i < 5; i++)
    {
      if (data_array[i] == 1)
      {
        if (i == 0)
        {
          strip.fill(c, 0, 59);
        }
        if (i == 1)

        {
          strip.fill(c, 59, 59);
        }

        if (i == 2)

        {
          strip.fill(c, 119, 59);
        }
        if (i == 3)

        {
          strip.fill(c, 177, 58);
        }
        if (i == 4)

        {
          strip.fill(c, 235, 53);
        }
      }
      if (data_array[i] == 0)
      {
        if (i == 0)
        {
          strip.fill(strip.Color(0, 0, 0), 0, 59);
        }
        if (i == 1)

        {
          strip.fill(strip.Color(0, 0, 0), 59, 59);
        }

        if (i == 2)

        {
          strip.fill(strip.Color(0, 0, 0), 119, 59);
        }
        if (i == 3)

        {
          strip.fill(strip.Color(0, 0, 0), 177, 58);
        }
        if (i == 4)

        {
          strip.fill(strip.Color(0, 0, 0), 235, 53);
        }
      }
    }
    strip.show();
  }
}