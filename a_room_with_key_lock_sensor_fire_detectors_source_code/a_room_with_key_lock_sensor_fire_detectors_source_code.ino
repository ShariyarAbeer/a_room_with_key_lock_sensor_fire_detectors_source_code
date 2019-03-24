//Test
#include <Keypad.h>
#include <SoftwareSerial.h>

const byte ROWS = 4;
const byte COLS = 3;
boolean outs = true;
int LED = 7;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {0, 1, 2, 3};
byte colPins[COLS] = {4, 5, 6};

String password = "123#";
String pass;
String passWorng;
int index_number;
int count = 0;
int mypass = 4;
int arr[10] = { 1, 2, 3, 2, 3, 2, 1, 3, 4, 3};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

const byte rxPin = 10;
const byte txPin = 11;
const int smokepin = 9;
boolean sendSms = false;

int inputPin = 8;               // choose the input pin (for PIR sensor)
int val = 0;                    // variable for reading the pin status
int v = 0;

SoftwareSerial SIM900(rxPin, txPin);
char msg;
int countpress = 0;

void setup() {

  // pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(LED, OUTPUT);

  // put your setup code here, to run once:
  Serial.begin(9600);
  SIM900.begin(9600);
  //Serial.println("Smoke Detector Console");
  SIM900.print("AT+CMGF=1\r");
  delay(100);
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
  // input
  pinMode(smokepin, INPUT);
  // output

}

void loop() {
  char customKey = customKeypad.getKey();

  if (outs) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  if (customKey) {
    Serial.println(customKey);
    countpress++;
  }
  pass += String(customKey);
  String temp = String(customKey);
  if (temp == "1") {              //set pass=1 by temp
    Serial.println("in Pass ");
    Serial.println(pass);
    pass = "";
    pass = "1";
  }
  if (countpress == 10) {
    Serial.println("To much press");
    pass = "";
    countpress = 0;
    SendMessage("Security Breach");
  }
  if (password == pass) {
    pass = "";
    outs = false;
    Serial.println("pass");
  }
  if (customKey) {
    passWorng += String(customKey);
    if (password == pass) {
      Serial.println("wng");
      pass = "";
      if (outs) {
         // SendMessage();
        delay(1000);
        sendSms = true;
        Serial.println("SMS Send");
      }
    }
  }
  //-------------------------------------Reset System---------------------------
  if (temp == "*") {
    outs = true;
    pass = "";
    countpress = 0;
    Serial.println("reset");
    sendSms = false;
  }
  //------------------------------Motion-----------------------------------------
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    Serial.println("Motion detected!");
    delay(5000);
    if (outs) {
      SendMessage("Abnormal Activity");
      delay(50);
      sendSms = true;
      Serial.println("SMS Send");
    }
  }
  //-------------------------------Smoke-----------------------------------------
  v = digitalRead(smokepin);
  if (!digitalRead(smokepin)) {
    if (v == HIGH) {
      Serial.println("Smoke is High");
      Serial.println("Sms Send");
      SendMessage("Smoke alarm");
      delay(500);
      sendSms = true;
    }
  } else {
    if (v == LOW) {
      Serial.println("Smoke is LOW");
      sendSms = false;
    }

  }

}

void SendMessage(String msg)
{
  Serial.println("Sending Message ");
  Serial.print(msg);
  Serial.println(" ");
  
  SIM900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  SIM900.println("AT+CMGS=\"+8801920871929\"\r"); // Replace x with mobile number
  delay(1000);
  SIM900.println(msg);// The SMS text you want to send
  delay(100);
  SIM900.println((char)26);// ASCII code of CTRL+Z
  delay(1000);

}
