/*
#include <SPI.h>
#include <SD.h>

const int chipSelect = 53;

void setup() {
  Serial.begin(9600);
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

  // Nếu tệp có thể mở, ghi vào tệp
  if (dataFile) {
    dataFile.println("Testing 1, 2, 3.");
    dataFile.println("Check value at 10:48 am");
    dataFile.close();
    Serial.println("Data written to file.");
  } else {
    Serial.println("error opening datalog.txt");
  }
}

void loop() {
  // Không cần làm gì trong loop
}*/

// CS -> 10
// CLK -> 13
// MOSI -> 11
// MISO -> 12
// 3.3V -> 3.3V
// GND -> GND

#include <SD.h>

const int chipSelect = 53;  // chân CS trên module thẻ nhớ
char filename[] = "test1.txt";             // tạo tên file
//đối tượng file
File logfile;
//========================================================

void setup() {

  Serial.begin(9600);  //khởi chạy serial ở baudrate 9600

  Serial.println("Khoi dong the SD...");


  if (!SD.begin(chipSelect)) {
    Serial.println("the co loi hoạc ko duoc cam vao");
    while (1);  //dung chuong trinh
  }
  Serial.println("the khoi dong xong.");
  
}
int i = 0;

void loop() {
  if(i > 3 && i < 15){
    logfile = SD.open(filename, FILE_WRITE);  // mở file để ghi
    logfile.println(i);
    logfile.close();
  }
  i++;
}
