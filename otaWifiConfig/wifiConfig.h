/**

   Written and Verified by the owner of techiesms
   Mr. Sachin Soni

   This code is open for fair use.
   If you are using it for commercial purpose, then it's mandatory to give credits

   Tutorial Video Link :- 

*/
#include <CTBot.h>
#include "Utilities.h"

CTBot myBot;
#include "OTA.h";
// #include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>

//Variables
int i = 0;
int statusCode;
const char* ssid = "text";
const char* passphrase = "text";
String st;
String content;
String token = "1934174372:AAEVXbfdOvipT6FOMj4rqgVo0aU8LvW4Ko0"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN
String AP = "Rumah Gas";
const int AdminID = -508399154;

String DevNum = "#1";
int UserID = -465154529;
String GroupName = "Group Name";
String Address = "GMI G1.1";

//const char index_html[] PROGMEM = {"<!DOCTYPE html>\n<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<style>\n.card {\n  box-shadow: 0 4px 8px 0 rgba(0,0,0,0.2);\n  transition: 0.3s;\n  width: 40%;\n}\n\n.card:hover {\n  box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2);\n}\n\n.container {\n  padding: 20px 16px;\n}\n</style>\n</head>\n<body>\n\n<h2>\"Rumah Gas\"</h2>\n<div class=\"card\">  \n  <div class=\"container\">\n  \t<form action=\"/action_page.php\">\n      <label>SSID:</label><br>\n      <input name=\"ssid\"><br><br>\n      <label>Password:</label>\n      <input type=\"text\" name=\"pass\"><br><br>\n      <input type=\"submit\" >\n\t</form>\n  </div>\n</div>\n\n</body>\n</html> \n"};

//ESP1
int Relay = 0;
int OrderButt = 2;
//ESP8266 12E
//int Relay = 2;
//int OrderButt = 4;


bool fan = false;
bool gas = false;
//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
void createWebServer(void);
void resetAll(void);

//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);

void settingUp()
{

  Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
  Serial.println();
  Serial.println("Disconnecting previously connected WiFi");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);
  pinMode(OrderButt, INPUT);
  digitalWrite(OrderButt, HIGH);
  Serial.println();
  Serial.println();
  Serial.println("Startup");

  //---------------------------------------- Read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");

  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);

  String euserid = "";
  for (int i = 96; i < 106; ++i)
  {
    euserid += char(EEPROM.read(i));
  }
  Serial.print("User ID: ");
  Serial.println(euserid);
  UserID = euserid.toInt();


  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
  OTAfunc(esid.c_str(), epass.c_str(),"UserBot8266");
      //Connect to Bot
  myBot.wifiConnect(esid.c_str(), epass.c_str());
  //set Token
  myBot.setTelegramToken(token);
  
  //Connection wifi check
  if (myBot.testConnection())
  {
    Serial.print("Success to connect");
    myBot.sendMessage(UserID, "Device : " + DevNum + " ("+ WiFi.localIP().toString() +")\nUserID : " + UserID + "\nAddress : " + Address + "\n--- Alat Siap Digunakan ---");


    digitalWrite(Relay, HIGH);
  }
  else
  {
    Serial.print("Failed to connect");
    digitalWrite(Relay, LOW);
  }
    
    
    Serial.println("Succesfully Connected!!!");
    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
  }

  Serial.println();
  Serial.println("Waiting.");
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }

}



//----------------------------------------------- Fuctions used for WiFi credentials saving and connecting to it which you do not need to change 
bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}

void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  
  WiFi.softAP(AP, "");
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
}

void resetAll(){
  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.end();
  ESP.restart();
}

void createWebServer()
{
 {
    server.on("/", []() {
//const char index_html[] PROGMEM = {"<!DOCTYPE html>\n<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<style>\n.card {\n  box-shadow: 0 4px 8px 0 rgba(0,0,0,0.2);\n  transition: 0.3s;\n  width: 40%;\n}\n\n.card:hover {\n  box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2);\n}\n\n.container {\n  padding: 20px 16px;\n}\n</style>\n</head>\n<body>\n\n<h2>\"Rumah Gas\"</h2>\n<div class=\"card\">  \n  <div class=\"container\">\n  \t<form action=\"/action_page.php\">\n      <label>SSID:</label><br>\n      <input name=\"ssid\"><br><br>\n      <label>Password:</label>\n      <input type=\"text\" name=\"pass\"><br><br>\n      <input type=\"submit\" >\n\t</form>\n  </div>\n</div>\n\n</body>\n</html> \n"};

      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE html>\n<html>\n<head>\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n<style>\n.card {\n  box-shadow: 0 4px 8px 0 rgba(0,0,0,0.2);\n  transition: 0.3s;\n  width: 60%;\n}\n\n.card:hover {\n  box-shadow: 0 8px 16px 0 rgba(0,0,0,0.2);\n}\n\n.container {\n  padding: 20px 16px;\n}\n</style>\n</head>\n<body>\n\n<h2>\"Rumah Gas\"</h2>\n";
//      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p> Please select to your Access Point";
      content += st;
      content += "</p>";
      content += "<div class=\"card\">  \n  <div class=\"container\">\n  \t<form  method='get' action='setting'>\n<label>SSID:</label><br>\n<input name=\"ssid\"length=32><br><br>\n<label>Password:</label>\n<input type=\"text\" name=\"pass\"length=64><br><br>\n <label>Telegram ID Group:</label>\n<input type=\"text\" name=\"userid\"length=10><br><br>\n<input type=\"submit\" >\n\t</form>\n  </div>\n</div>\n\n</body>\n</html> \n";

      server.send(200, "text/html", content);
    });


    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      String quserid = server.arg("userid");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 106; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
        Serial.println(quserid);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        Serial.println("writing eeprom userid:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(96 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(quserid[i]);
        }
        EEPROM.commit();

        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);

    });
  } 
}
