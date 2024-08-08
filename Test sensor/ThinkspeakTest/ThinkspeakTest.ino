#include <WiFi.h>
#include "ThingSpeak.h"
#include <Wire.h>
// check push
/* Địa chỉ của DS1307 */



const byte DS1307 = 0x68;

/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;

 /* Số byte dữ liệu sẽ đọc từ DS1307 */
const byte NumberOfFields = 7;

// information about network
const char* ssid = "IoT";
const char* password = "12345678";

// Information about Thingspeak channel 
unsigned long myChannelNumber = 2616698 ;
const char* myWriteAPI = "Y8FDLPTBK2EAUXWK" ;

WiFiClient client ;

void setup(){
    Wire.begin();
    Serial.begin(115200);
    WiFi.begin(ssid , password);
    Serial.print("Connecting to WiFi ...");
    while ( WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print('.');
    }
    Serial.println("Connected.");

  // setup for Thinkspeak 
  ThingSpeak.begin(client);
  //setTime(13, 55, 00, 6, 2, 2, 24);
}
void loop(){
  readDS1307();
  Serial.print(hour);
  Serial.print(" : ");
  Serial.print(minute);
  Serial.print(" : ");
  Serial.println(second);
  // send data 
  ThingSpeak.setField(1 , minute);
  ThingSpeak.setField(2 , second);
  // write data 
  int x  = ThingSpeak.writeFields(myChannelNumber , myWriteAPI);
  if ( x == 200){
    Serial.print("Successful.");
  }else {
    Serial.print("Failed ");
    Serial.println(x);
  }
  delay(20000);

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