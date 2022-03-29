#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <RF24.h>
#include <spi.h>
#include <RF24Network.h>

//needs to get the signal from the master
// apply the the rb
RF24 radio(A0, 10);
RF24Network network(radio);

const uint16_t this_node = 01;
const uint16_t master = 00;

//setting up LED
#define LED_PIN 9
#define LED_COUNT 192
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// have the cube
// - each strip is 24 in length 8strips in total
//    - each bar is 48 in length
// - Have a way to control each strip

//  →         direction
//  04====15↓
//  ||    ||
//  ||    ||
// ↑37====26
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
    int data_array[12];
    network.read(header, &data_array, sizeof(data_array));

    Serial.println("A - W - WW");
    Serial.print(data_array[8]);
    Serial.print(data_array[9]);
    Serial.print(data_array[10]);

    uint32_t c = strip.Color(data_array[8], data_array[9], data_array[10]);
    strip.setBrightness(data_array[11]);
    for (int i = 0; i < 8; i++)
    {
      // checkout all the strip
      strip.fill(c, 24 * (i * data_array[i]), 24);
    }
    strip.show();
  }
}