#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <SerialCommand.h>  // Thêm vào sketch thư viện Serial Command
const byte RX = 3;          // Chân 3 được dùng làm chân RX
const byte TX = 2;          // Chân 2 được dùng làm chân TX
 
SoftwareSerial mySerial(RX, TX); 
 
 
SerialCommand sCmd(mySerial); // Khai báo biến sử dụng thư viện Serial Command
 
int red = 4, blue = 5; // led đỏ đối vô digital 4, led xanh đối vô digital 5
 
void setup() {
  //Khởi tạo Serial ở baudrate 57600 để debug ở serial monitor
  Serial.begin(57600);
 
  //Khởi tạo Serial ở baudrate 57600 cho cổng Serial thứ hai, dùng cho việc kết nối với ESP8266
  mySerial.begin(57600);
  
  //pinMode 2 đèn LED là OUTPUT
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  
  
  // Một số hàm trong thư viện Serial Command
  sCmd.addCommand("LED",   led); //Khi có lệnh LED thì sẽ thực thi hàm led  
  Serial.println("Da san sang nhan lenh");
}
 
void loop() {
  sCmd.readSerial();
  //Bạn không cần phải thêm bất kỳ dòng code nào trong hàm loop này cả
}
 
// hàm led_red sẽ được thực thi khi gửi hàm LED_RED
void led() {
  Serial.println("LED");
  char *json = sCmd.next(); //Chỉ cần một dòng này để đọc tham số nhận đươc
  Serial.println(json);
  StaticJsonDocument<200> jsonDocument; //tạo Buffer json có khả năng chứa tối đa 200 ký tự
  JsonObject root;//đặt một biến root mang kiểu json
  deserializeJson(jsonDocument, json);// fixme: throw error at here - Error compiling for board Arduino Uno => https://arduinojson.org/v6/api/json/deserializejson/

  int redStatus = jsonDocument["led"][0];//json -> tham số root --> phần tử thứ 0. Đừng lo lắng nếu bạn không có phần tử này, không có bị lỗi đâu!
  int blueStatus = jsonDocument["led"][1];//json -> tham số root --> phần tử thứ 0. Đừng lo lắng nếu bạn không có phần tử này, không có bị lỗi đâu!
 
  //kiểm thử giá trị
  Serial.print(F("redStatus "));
  Serial.println(redStatus);
  Serial.print(F("blueStatus "));
  Serial.println(blueStatus);
 
 
  //xuất ra màn hình
  digitalWrite(red, redStatus);
  digitalWrite(blue, blueStatus);
}
