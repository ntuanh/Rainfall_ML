// TX2 : GPIO 17 
// RX2 : GPIO 16
/*
#include "HardwareSerial.h"




#define ADDRESS 0x01
#define FUNCTION_CODE 0x03
#define INITIAL_ADDRESS_HI 0x00
#define INITIAL_ADDRESS_LO 0x00
#define DATA_LENGTH_HI 0x00
#define DATA_LENGTH_LO 0x01
#define CHECK_CODE_LO 0x84
#define CHECK_CODE_HI 0x0A
#define NUMBER_BYTES_RESPONES 7
// Request frame for the soil sensor
const byte request[] = {ADDRESS, FUNCTION_CODE, INITIAL_ADDRESS_HI, INITIAL_ADDRESS_LO, DATA_LENGTH_HI, DATA_LENGTH_LO, CHECK_CODE_LO, CHECK_CODE_HI};
byte sensorResponse[NUMBER_BYTES_RESPONES];
unsigned long startTime;



//SoftwareSerial mod(2, 3); // Software serial for RS485 communication


void setup()
{
  Serial.begin(9600); // Initialize serial communication for debugging
  //mod.begin(4800);    // Initialize software serial communication at 4800 baud rate
  Serial2.begin(4800);
  delay(500);
  startTime = millis();
}
 
float Rainfall(){
//void loop()
//{
  // Wait for the response from the sensor or timeout after 1 second
  if((millis() > startTime+2000)){
    startTime = millis();
    // Send the request frame to the soil sensor
    Serial2.write(request, sizeof(request));
    Serial.println("Sent request data");
  }
  if (Serial2.available() >= NUMBER_BYTES_RESPONES) // If valid response received
  {
    // Read the response from the sensor
    byte index = 0;
    while (Serial2.available()  && index < NUMBER_BYTES_RESPONES)
    {
      sensorResponse[index] = Serial2.read();
      Serial.print(sensorResponse[index], HEX); // Print the received byte in HEX format
      Serial.print(" ");
      index++;
    }
    Serial.println(" $End of Rx data");
 
    // Parse and calculate the Rainfall value
    int Sensor_Int = int(sensorResponse[3] << 8 | sensorResponse[4]);
    float Senor_Float = Sensor_Int / 10.0;
 
    Serial.print("Float result: ");
    Serial.println(Senor_Float);
    return Senor_Float;
  }
  return 0 ;
}

void loop(){
  float rainfall = Rainfall();
  Serial.print("Result : ");
  Serial.println(rainfall);
}

*/

#include <HardwareSerial.h>

// Khai báo Serial2 cho RS485
HardwareSerial rs485Serial(2);

void setup() {
  // Khởi động Serial cho debug
  Serial.begin(9600);
  while (!Serial) {
    ; // Chờ để mở kết nối serial
  }
  
  // Khởi động Serial2 cho RS485
  rs485Serial.begin(9600, SERIAL_8N1, 16, 17); // RX, TX
}

void loop() {
  // Nếu có dữ liệu từ RS485
  if (rs485Serial.available()) {
    String rainfallData = "";
    // Đọc dữ liệu từ RS485
    while (rs485Serial.available()) {
      char c = rs485Serial.read();
      rainfallData += c;
    }
    // Hiển thị dữ liệu đọc được
    Serial.println("Dữ liệu cảm biến mưa: " + rainfallData);
  }
}
