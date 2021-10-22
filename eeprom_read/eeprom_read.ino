#define DEBUG 1 
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#include <EEPROM.h>

// start reading from the first byte (address 0) of the EEPROM
int address = 0;
byte value;

void setup() {
  // initialize serial and wait for port to open:
  Serial.begin(115200);
  EEPROM.begin(512);
}

void loop() {


 //---------------------------------------- Read eeprom for ssid and pass
//  debugln("Reading EEPROM ssid");
  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  debugln();
  debug("SSID: ");
  debugln(esid);
  
//  debugln("Reading EEPROM pass");
  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  debug("PASS: ");
  debugln(epass);


//  debugln("Reading EEPROM herocode");
  String eherocode = "";
  for (int i = 96; i < 106; ++i)
  {
    eherocode += char(EEPROM.read(i));
  }
  debug("Herocode: ");
  debugln(eherocode);
//  Herocode = eherocode.c_str();

  ////////////////////////////////////////////////////// PIN
  //  debugln("Reading EEPROM PIN");
  String epin = "";
  for (int i = 106; i < 111; ++i)
  {
    epin += char(EEPROM.read(i));
  }
  debug("PIN: ");
  debugln(epin);
//  PIN = epin.c_str();


  ////////////////////////////////////////////////////// Address
 String eaddress = "";
  for (int i = 111; i < 211; ++i)
  {
    eaddress += char(EEPROM.read(i));
  }
  debugln();
  debug("Address: ");
  debugln(eaddress);

  



  /////////////////
}
