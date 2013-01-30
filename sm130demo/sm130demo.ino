// SM130 demo application
// Marc Boon <http://www.marcboon.com>
// June 2009

// Controls a SonMicro SM130/mini RFID reader or RFIDuino by I2C
// Arduino analog input 4 is I2C SDA (SM130/mini pin 10/6)
// Arduino analog input 5 is I2C SCL (SM130/mini pin 9/5)
// Arduino digital input 4 is DREADY (SM130/mini pin 21/18)
// Arduino digital output 3 is RESET (SM130/mini pin 18/14)

#include <Wire.h>
#include <SM130.h>

// Actions
#define NONE 0
#define READ 2
#define WRITE 3

// Create SM130 instance for RFIDuino
SM130 RFIDuino;

String readline = "";
String tagstring;
char c;

struct Record {
  String id;
  String name;
};

struct Record *tmp;
struct Record records[10];
int records_count = 0;


struct Record *getRecord(String id) {
  int n;
  for(n=0; n<records_count; n++) {
    if(records[n].id == id) {
       return &records[n];
    }
  }
  return NULL;
}

void setRecord(String id, String name) {
  Record *r = getRecord(id);

  if(r != NULL) {
     r->name = name; 
  }
  else {
    records[records_count].id = id;
    records[records_count].name = name;
    records_count++; 
  }
}

void printRecords() {
  int n;
    Serial.println("printling");
   for(n= 0; n<records_count; n++) {
     Serial.println("    " + records[n].id + ": " + records[n].name);
   } 
}

void setup()
{
  
  Wire.begin();
  Serial.begin(115200);
  
//  initList();

  // reset RFIDuino
  RFIDuino.reset();
}

void loop()
{
  RFIDuino.selectTag();
  
  if(RFIDuino.available())
  {
    tagstring = RFIDuino.getTagString();
    if(tagstring != "") {
      Serial.print("Found Tag: ");
      Serial.println(tagstring);
      tmp = getRecord(tagstring);
      if(tmp == NULL) {
        Serial.print("Enter Name: ");
        while(true) {
          if(Serial.peek() != -1) {
            c = Serial.read();
            readline += c;
            if(c == '\n') break;
          }
        }
        setRecord(tagstring, readline);
//        printRecords();
        readline = "";
      }
      else {
         Serial.println("Found record!: " + tmp->name); 
        }

      }
    }
}

