#include <EEPROM.h>
#define led1 13
//#define sensor1 7                // board cũ sensor1 pin2, Board mới sensor1 pin7
//#define sensor2 6
const byte sensors_size = 6;
int sensorsPin[sensors_size] = {2,3,4,5,6,7}; //4 left elements is terminal, 2 right elements is wire {2,3,4,5,6,7} BOARD MOI || {5,4,3,2,6,7} BOARD CU 
//Terminal4_Terminal3_Terminal2_Terminal1_Wire2_Wire1

//String chuoi[sensors_size] = {'0','0','0','0','0','1'};           // lưu tín hiệu đến
String chuoi = "000001";
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

void setup()
{
  inputString.reserve(200);
//  pinMode(sensor1, INPUT_PULLUP);
//  pinMode(sensor2, INPUT_PULLUP);
  for(int i = 0; i < sensors_size; i++){
      pinMode(sensorsPin[i], INPUT_PULLUP);    
  }
  pinMode(led1, OUTPUT);
  Serial.begin(9600);     // mở cổng serial với mức baudrate là 9600
  chuoi = readEEPROM(20, 6);
  if (chuoi.length() > 6) {
    chuoi = "000001";//set default
    saveEEPROM(chuoi, 20);
  }
}

void loop()
{
  for(int i = 0; i < sensors_size; i++){
      chuoi[i] = digitalRead(sensorsPin[i]) == HIGH ? '0' : '1'; //read signal of wire and terminal
  }
  
  if (stringComplete)
  {
    if (inputString == "GET")
    {
      delay(2);
      Serial.println(chuoi);
    }
    else if (inputString.substring(0, 4) == "SET_")
    {
      chuoi = inputString.substring(4, inputString.length());
      saveEEPROM(chuoi, 20);
      Serial.println("OK");
    }
    //-------------------------------------
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
//-----------------------------------------------------------------------------------------------------
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
    else {
      // add it to the inputString:
      inputString += inChar;
    }
  }
}

//=======================================================
//luu gia tri vao EEPROM
void saveEEPROM (String val, int addr) {
  int len = 0;
  len = val.length();
  for (int i = 0; i < len; i++) {
    EEPROM.write(i + addr, val.charAt(i));
  }
}
//=======================================================
// doc gia tri tu EEPROM
String readEEPROM(int addr, int len) {
  String temp = "";
  for (int i = 0; i < len; i++) {
    temp = temp + (String)(EEPROM.read(i + addr) - 48);
    //48 = "0" in ascii code
  }
  return temp;
}
