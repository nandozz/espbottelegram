//IP address: 192.168.100.5

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "Thanks God"
#define STAPSK  "godbless@"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

///////////////////////////////////////////////
#include <CTBot.h>
int Relay = 0;
int OrderButt = 2;
bool UserOrder=false;
bool UserHelp=false;
CTBot myBot;
String token = "1934174372:AAEVXbfdOvipT6FOMj4rqgVo0aU8LvW4Ko0";   // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

const int AdminID = -508399154;  

String DevNum = "#Admin";
//const int UserID = -1;
String GroupName = "Admin Rumah Gas";



//////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  //////////////////////////////////////
  pinMode(Relay, OUTPUT);
  pinMode(OrderButt, INPUT);
  digitalWrite(OrderButt, HIGH);

  //Connect to WiFi
  myBot.wifiConnect(ssid, password);
  //set Token
  myBot.setTelegramToken(token);
  //Connection wifi check
  if(myBot.testConnection())
    {Serial.print("Success to connect");
    myBot.sendMessage(AdminID, "Device : "+DevNum+"\nAdmin ID : "+AdminID+"\n--- RESTART ---");  
    digitalWrite(Relay, HIGH);
    }
  else
    {Serial.print("Failed to connect");  
    digitalWrite(Relay, LOW);
    }
 /////////////////////////////////////////
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
   ArduinoOTA.setHostname("Admin8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  ArduinoOTA.handle();
  TBMessage msg;
  if(myBot.getNewMessage(msg)){
    String command = msg.text;

//alldevices
     if (command == "/alldevices"){
      myBot.sendMessage(AdminID, "Device : "+DevNum+"\nAdmin ID : "+AdminID+"\n--- ACTIVE ---");  
      }
//info
     else if (command == "/info"){
     myBot.sendMessage(msg.group.id, 
        "Hallo, "+ (String)msg.sender.firstName +" " + (String)msg.sender.lastName+
        "\nKunjungi website https://tokko.io/delivfoodku");
      }
//groupid
      else if (command == "/start"){
     myBot.sendMessage(msg.group.id, 
        "Hallo, "+ (String)msg.sender.firstName +" " + (String)msg.sender.lastName+
        "\nYour Group ID : " +  int32_t (msg.group.id)+
        "\nGunakan untuk registrasi, https://google.com");
      }
//help
      else if (command == "/help"){
      //toAdminID
      myBot.sendMessage(msg.group.id, "Untuk menggunakan perintah pada icon \"/\",\npastikan sudah menerima pesan \"Registrasi Berhasil\" setelah merestart alat");
      }
    
  }
}
