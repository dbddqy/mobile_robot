#include <SoftwareSerial.h>
#include <stdint.h>

SoftwareSerial mySerial(2, 3); // RX, TX
uint8_t data_received[16] = { 0 };
bool isReceived = false;
String data_received_s;
//char* junk_data;

void setup() {
  Serial.begin(9600);
  mySerial.begin(74880);
  delay(2000);
  Serial.println("Arduino UART Receiver");
  Serial.println("-----------------------------");
}


void loop() {
//  // clear data
//  while(mySerial.read()>= 0);
  
  //read the data
//  while ( !mySerial.available() );
//  delay(2);
//  for (int i=0; i<8;i++)
//    data_received[i] = mySerial.read();
//
//  int16_t w4 = (int16_t)((data_received[6]) << 8)
//         | ((data_received[7]) << 0);
//  Serial.print(w4, DEC);

  while ( mySerial.available() ) {
    Serial.print(mySerial.read());
    Serial.print("\n");
  }
  //delay(300);
}
