#include <Arduino.h>
#include <RF24.h>
#include <spi.h>
#include <RF24Network.h>

// this is going to be done with an esp or web server

// the master will provide the singal
// its going to send data for the rgba to device 1 and device 2
RF24 radio(A0, 10);
RF24Network network(radio);

const uint16_t this_node = 00;
const uint16_t node01 = 01;

const uint16_t node02 = 02;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);
}
String incomingByte = ""; // for incoming serial data
void loop()
{
  // put your main code here, to run repeatedly:
  network.update();

  // maybe have a pin, which reads the voltage
  // lets say pin 1 and 2
  // if pin 1 == high, then control
  // if pin 1 and pin 2 low then contorl both
  if (Serial.available() > 0)
  {
    // read the incoming byte

    int first = Serial.readStringUntil('d').toInt();
    Serial.read(); //next character is comma, so skip it using this
    int second = Serial.readStringUntil('d').toInt();
    Serial.read();
    int third = Serial.readStringUntil('d').toInt();
    Serial.read();
    int last = Serial.readStringUntil('d').toInt();
    //parse your data here. example:
    //double x = Double.parseDouble(first);

    int data_array[12] = {1, 0, 1, 0, 1, 0, 1, 0, first, second, third, last};

    RF24NetworkHeader header(node01);
    network.write(header, &data_array, sizeof(data_array));
    // say what you got:
    Serial.print("I received: ");

    Serial.println(first);
    Serial.println(second);
    Serial.println(third);
    Serial.println(last);
  }

  //Bar -> node 1

  //Cube -> node 2
}