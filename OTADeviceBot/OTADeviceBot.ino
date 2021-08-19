//IP address: 192.168.100.5

#include "OTA.h";

#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>res
//#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "Thanks God"
#define STAPSK "godbless@"
#endif
const char *ssid = STASSID;
const char *password = STAPSK;


//////////////////////
#include <CTBot.h>
#include "Utilities.h"

CTBot myBot;
String IP = "192.168.1.1";
String AP = "Rumah Gas";
String token = "1934174372:AAEVXbfdOvipT6FOMj4rqgVo0aU8LvW4Ko0"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

const int AdminID = -508399154;

String DevNum = "#1";
const int UserID = -465154529;
String GroupName = "Group Name";
String Address = "GMI G1.1";
int Relay = 0;
int OrderButt = 2;
bool fan = false;
bool gas = false;
//////////////////////



void setup()
{
  OTAfunc(ssid, password,"UserBot8266");

  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, HIGH);

  pinMode(OrderButt, INPUT);
  digitalWrite(OrderButt, HIGH);

  //Connect to WiFi
  myBot.wifiConnect(ssid, password);
  //set Token
  myBot.setTelegramToken(token);
  //Connection wifi check
  if (myBot.testConnection())
  {
    Serial.print("Success to connect");
    myBot.sendMessage(UserID, "Device : " + DevNum + "\nUserID : " + UserID + "\nAddress : " + Address + "\n--- Alat Siap Digunakan ---");
    digitalWrite(Relay, HIGH);
  }
  else
  {
    Serial.print("Failed to connect");
    digitalWrite(Relay, LOW);
  }
}

void loop()
{
  ArduinoOTA.handle();
  TBMessage msg;

  //Sensor

  //ORDER
  //Read message from UserID

  if (myBot.getNewMessage(msg))
  {
    String GroupName = (String)msg.group.title;
    String Name = (String)msg.sender.firstName;

    String command = msg.text;

    //Command FROM ADMIN
    if (msg.group.id == AdminID){
      
      
      
    if (command == "/alldevices")
    {
      myBot.sendMessage(AdminID, "Device : " + DevNum + " (" + UserID + ")\nName : " + Name + "\nAddress : " + Address + "\n--- ACTIVE ---");
    }
    else if (command == "/" + DevNum + "restart")
    {
      myBot.sendMessage(AdminID, "Device : " + DevNum + " (" + UserID + ")\n--- Restart ---");
      myBot.sendMessage(UserID, "Device : " + DevNum + " (" + UserID + ")\n--- Restart by Admin ---");
    }
    else if (command == "/" + DevNum + "on")
    {
      myBot.sendMessage(AdminID, "Device : " + DevNum + " (" + UserID + ")\n--- Relay ON ---");
      myBot.sendMessage(UserID, "Device : " + DevNum + " (" + UserID + ")\n--- Relay ON by Admin ---");
      digitalWrite(Relay, LOW);
      fan = true;
    }
    else if (command == "/" + DevNum + "off")
    {
      myBot.sendMessage(AdminID, "Device : " + DevNum + " (" + UserID + ")\n--- Relay OFF ---");
      myBot.sendMessage(UserID, "Device : " + DevNum + " (" + UserID + ")\n--- Relay OFF by Admin ---");
      digitalWrite(Relay, HIGH);
      fan = false;
    }
    else if (command == "/" + DevNum + "reset")
    {
      myBot.sendMessage(AdminID, "Device : " + DevNum + " (" + UserID + ")\n--- RESET ---");
      myBot.sendMessage(AdminID, "Device : " + DevNum + " (" + UserID + ")\n--- RESET by Admin ---");
    }
    }

    //MESSAGE FROM reg GROUP TELEGRAM
    else if (msg.group.id == UserID)
    {

      if (command == "/order")
      {
        //toAdminID
        myBot.sendMessage(AdminID, "Device : " + DevNum + " (" + int64ToAscii(msg.group.id) + ")\nName : " + (String)msg.group.title + "\nAddress : " + Address + "\n--- ORDER ---");
        //toUserID
        myBot.sendMessage(UserID, "Terimakasih, " + (String)msg.sender.firstName + "\nPesanan sudah diteruskan ke Admin");
      }

      else if (command == "/on")
      {
        digitalWrite(Relay, LOW);
        myBot.sendMessage(msg.group.id, "Relay Turn ON");
        fan = true;
      }

      else if (command == "/off")
      {
        digitalWrite(Relay, HIGH);
        myBot.sendMessage(msg.group.id, "Relay Turn OFF");
        fan = false;
      }

      else if (command == "/restart")
      {
        myBot.sendMessage(UserID, "Device : " + DevNum + " (" + UserID + ")\n--- Device Restart ---");
      }
      else if (command == "/status")
      {
        myBot.sendMessage(UserID, "Device : " + DevNum + " (" + UserID + 
        ")\nStatus Fan : "+(fan ? "HiDUP" : "MATI") + 
        "\nStatus Gas : "+(gas ? "BAHAYA" : "AMAN"));
      }

      else if (command == "/reset")
      {
        myBot.sendMessage(UserID, "Device : " + DevNum + " (" + UserID +
                                      ")\nAlat tidak terhubung ke WiFi, segera lakukan registrasi kembali dengan cara " +
                                      "\n1. Hubungkan perangkat handphone/komputer ke jaringan AP \"" + AP + "\"" +
                                      "\n2. akses alamat IP " + IP);
      }
      else {
        myBot.sendMessage(UserID,"Perintah tidak dikenali");
        }
    }
  }
  else if (digitalRead(OrderButt) == LOW)
  {
    //toAdminID
    myBot.sendMessage(AdminID, "Device : " + DevNum + " (" + int64ToAscii(msg.group.id) + ")\nAddress : " + Address + "\n--- ORDER ---");
    //toUserID
    myBot.sendMessage(UserID, "Terimakasih, " + (String)msg.sender.firstName + "\nPesanan sudah diteruskan ke Admin");
  }
}
