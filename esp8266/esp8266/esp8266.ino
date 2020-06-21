#include <ESP8266WiFiGratuitous.h>
#include <WiFiServerSecure.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <SoftwareSerial.h>
#include <SocketIOClient.h>

//include thư viện để kiểm tra free RAM trên con esp8266
extern "C" {
#include "user_interface.h"
}

const byte RX = D1;
const byte TX = D2;

SoftwareSerial mySerial(RX, TX);

SocketIOClient client;

const char* ssid = "AnNhienTech";
const char* password = "SushiLily";

char host[] = "192.168.68.114";
int port = 3484;

//từ khóa extern: dùng để #include các biến toàn cục ở một số thư viện khác. Trong thư viện SocketIOClient có hai biến toàn cục
// mà chúng ta cần quan tâm đó là
// RID: Tên hàm (tên sự kiện
// Rfull: Danh sách biến (được đóng gói lại là chuối JSON)
extern String RID;
extern String Rfull;

//Một số biến dùng cho việc tạo một task
unsigned long previousMillis = 0;
long interval = 2000;

void setup() {
  //Bật baudrate ở mức 115200 để giao tiếp với máy tính qua Serial
  Serial.begin(57600);
  mySerial.begin(57600); //Bật software serial để giao tiếp với Arduino, nhớ để baudrate trùng với software serial trên mạch arduino
  delay(10);

  //Việc đầu tiên cần làm là kết nối vào mạng Wifi
  Serial.print("Ket noi vao mang ");
  Serial.println(ssid);

  //Kết nối vào mạng Wifi
  WiFi.begin(ssid, password);

  //Chờ đến khi đã được kết nối
  while (WiFi.status() != WL_CONNECTED) { //Thoát ra khỏi vòng
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.println(F("Da ket noi WiFi"));
  Serial.println(F("Di chi IP cua ESP8266 (Socket Client ESP8266): "));
  Serial.println(WiFi.localIP());

  if (!client.connect(host, port)) {
    Serial.println(F("Ket noi den socket server that bai!"));
    return;
  }

  //Khi đã kết nối thành công
  if (client.connected()) {
    //Thì gửi sự kiện ("connection") đến Socket server ahihi.
    client.send("connection", "message", "Connected from ASP2866!!!!");
  }
}

void loop() {
  //Khi bắt được bất kỳ sự kiện nào thì chúng ta có hai tham số:
  //  +RID: Tên sự kiện
  //  +RFull: Danh sách tham số được nén thành chuỗi JSON!
  if (client.monitor()) {
    //in ra serial cho Arduino
    mySerial.print(RID);
    mySerial.print('\r');
    mySerial.print(Rfull);
    mySerial.print('\r');

    //in ra serial monitor
    Serial.print(RID);
    Serial.print(' ');
    Serial.println(Rfull);

    //Kiểm tra xem còn dư bao nhiêu RAM, để debug
    uint32_t free = system_get_free_heap_size();
    Serial.println(free);//in ra serial cho Arduino
    mySerial.print(RID);
    mySerial.print('\r');
    mySerial.print(Rfull);
    mySerial.print('\r');

    //in ra serial monitor
    Serial.print(RID);
    Serial.print(' ');
    Serial.println(Rfull);
  }

  //Kết nối lại!
  if (!client.connected()) {
    client.reconnect(host, port);
  }
}
