#include <SoftwareSerial.h>
#include <stdint.h>

SoftwareSerial mySerial(2, 3); // RX, TX
uint8_t data_received[12] = { 0 };
bool isReceived = false;
String data_received_s;
//char* junk_data;

void setup() {
  Serial.begin(57600);
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

  if ( mySerial.available() ) {
    if ( mySerial.read() == 0x2F ) {
      for (int i=0; i<12; i++)
        data_received[i] = mySerial.read();
      if ( mySerial.read() == 0x3F ) {
         float x;
         uint8_t *p = (uint8_t*)&x;
         p[0] = data_received[0];
         p[1] = data_received[1];
         p[2] = data_received[2];
         p[3] = data_received[3];
//        float x = (float) ((((uint32_t)data_received[0]) << 24)
//                         | (((uint32_t)data_received[1]) << 16)
//                         | (((uint32_t)data_received[2]) << 8)
//                         | (((uint32_t)data_received[3]) << 0));
        Serial.print(x);
        Serial.print("\n");             
      }
    }
    
  }
  delay(1);
}
