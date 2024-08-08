#include "HardwareSerial.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



#define ADDRESS 0x01
#define FUNCTION_CODE 0x03
#define INITIAL_ADDRESS_HI 0x00
#define INITIAL_ADDRESS_LO 0x00
#define DATA_LENGTH_HI 0x00
#define DATA_LENGTH_LO 0x01
#define CHECK_CODE_LO 0x84
#define CHECK_CODE_HI 0x0A
#define NUMBER_BYTES_RESPONES 7
#define CONSTANT_FACTOR 7.5 //covert hall sensor signal to lit / minute
// Request frame for the soil sensor
const byte request[] = {ADDRESS, FUNCTION_CODE, INITIAL_ADDRESS_HI, INITIAL_ADDRESS_LO, DATA_LENGTH_HI, DATA_LENGTH_LO, CHECK_CODE_LO, CHECK_CODE_HI};
byte sensorResponse[NUMBER_BYTES_RESPONES];
unsigned long startTime;

const int chipSelect = 5;
volatile int flowFrequency;  
unsigned int lPerHour; // 
unsigned char flowSensorPin = 2; 
unsigned long currentTime;
unsigned long cloopTime;

//LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte DS1307 = 0x68;

/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;

 /* Số byte dữ liệu sẽ đọc từ DS1307 */
  const byte NumberOfFields = 7;

//SoftwareSerial mod(2, 3); // Software serial for RS485 communication
void flowCount() // Interrupt function
{
    flowFrequency++;
}

void setup()
{
  Serial.begin(115200); // Initialize serial communication for debugging
  //mod.begin(4800);    // Initialize software serial communication at 4800 baud rate
  Serial2.begin(4800);
  Wire.begin();
  delay(500);
  startTime = millis();
  while (!Serial) {
    ; // Chờ để mở cổng Serial
  }

  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  // Mở tệp để ghi
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

      pinMode(flowSensorPin, INPUT_PULLUP);
    attachInterrupt(0, flowCount, RISING);
    sei(); // Enable interrupts
    currentTime = millis();
    cloopTime = currentTime;
  Wire.begin();
  //lcd.init();

  
  //setTime(16, 55, 00, 6, 2, 2, 24);
}

int Waterflow()
{
    currentTime = millis();
    //after 1s read the sensor signal count and reset it
    if (currentTime >= (cloopTime + 1000))
    {
        cloopTime = currentTime;  // update referent time
        lPerHour = (flowFrequency * 60 / CONSTANT_FACTOR);
        flowFrequency = 0;
        return lPerHour;
    }
    else
    return 0; 
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
void readDS1307()
{
          Wire.beginTransmission(DS1307);
          Wire.write((byte)0x00);
          Wire.endTransmission();
          Wire.requestFrom(DS1307, NumberOfFields);
          
          second = bcd2dec(Wire.read() & 0x7f);
          minute = bcd2dec(Wire.read() );
          hour   = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
          wday   = bcd2dec(Wire.read() );
          day    = bcd2dec(Wire.read() );
          month  = bcd2dec(Wire.read() );
          year   = bcd2dec(Wire.read() );
          year += 2000;    
  }

  /* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
  int bcd2dec(byte num)
  {
          return ((num/16 * 10) + (num % 16));
  }
  /* Chuyển từ Decimal sang BCD */
  int dec2bcd(byte num)
  {
          return ((num/10 * 16) + (num % 10));
}

void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
          Wire.beginTransmission(DS1307);
          Wire.write(byte(0x00)); // đặt lại pointer
          Wire.write(dec2bcd(sec));
          Wire.write(dec2bcd(min));
          Wire.write(dec2bcd(hr));
          Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
          Wire.write(dec2bcd(d)); 
          Wire.write(dec2bcd(mth));
          Wire.write(dec2bcd(yr));
          Wire.endTransmission();
}
void loop(){
  readDS1307();
  //lcd.setCursor(0,0);
  //lcd.print(minute);
}