#include <socketIOClient.h>
#define W5100
#include "Ethernet.h"
#include "SPI.h"

/*
 * This is a test to connect an arduino uno with a W5100 ethernet shield to a NodeJS server with SocketIO. 
 * The corresponding circuit is just an led on pin 7 and a button on pin 8. When the button is clicked,
 * the light toggles on and off, and emits to the server which updates it's UI accordingly. The server can
 * emit to the board, which will also light up the led. The hostname is set to 'https://witty-cat-82.localtunnel.me',
 * which is a subdomain i use with localtunnel for testing. This should be changed accordingly.
**/


SocketIOClient client;

class pin{
  int pinNumber; 
  int previousState; 
      void setMode(uint8_t pinType){
        if(pinType == INPUT){
        pinMode(this->pinNumber, INPUT);
        }else{
        pinMode(this->pinNumber, OUTPUT);
        }
      }
      public:pin::pin(int pinNumber, uint8_t pinType){
        this->pinNumber = pinNumber; 
        setMode(pinType); 
      }
      uint8_t state(){
      return digitalRead(this->pinNumber);
      }
      uint8_t prevState(){
      return this->previousState;
      }
      void changeState(uint8_t state){
      this->previousState = digitalRead(this->pinNumber);
        digitalWrite(this->pinNumber, state);
      }
};

class DGUI{
  pin currentPins[];
  String currentPinsName[]; 
  String registerListener; 
  String switchListener; 
  int addComponent(String functionInput, pin functionPin, String componentType){
    // add component
    // functionInput = name of component; 
    // functionPin = pass it a pin that is configured; 
    // componentType = type of component; 
    bool returnObject = true; 
    return returnObject; 
  }
  int removeComponent(string functionInput){
    //remove component
    bool returnObject = true; 
    return returnObject; 
  }
  size_t currentComponentSize(){
    return (sizeof(currentPins) / sizeof(pin)); 
  }
  int printComponentInfo(){
    bool returnObject = true;  
    int currentLength = static_cast<int>this->currentComponentSize();
    
    for(int i = 0; i < currentLength; i++){
      Serial.println(this->currentPins[i]);  
    }
    
    return returnObject; 
  }
  int createJsonObject(){
    bool returnObject = true; 
    return returnObject; 
  }
  int sendJsonObject(){
    bool returnObject = true; 
    return returnObject; 
  }
  
};


pin ledPin(7, OUTPUT);
pin buttonPin(8, INPUT); 

long time = 0;         // the last time the output pin was toggled
long debounce = 600;   // the debounce time, increase if the output flickers
long lastping = 0;

byte mac[] = { 0x30, 0x30, 0x32, 0x38, 0x46, 0x38, 0x43, 0x43, 0x38, 0x35, 0x46, 0x44  };

//TODO: make subdomain its own variable, im too stupid for this.
char hostname[] = "http://68.183.70.213";
char hostnameshort[] = "68.183.70.213";

//localtunnel uses 80
int port = 8080;

// Set the static IP address to use if the DHCP fails to assign

IPAddress ip(68, 183, 70, 213);
//IPAddress myDns(208, 67, 222, 222);
//208.67.222.222

//Pretty sure i dont need these anymore.

extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0; 
long interval = 10000; 

int reading;

void setup() {
  //set pins to low
  ledPin.changeState(LOW); 
  buttonPin.changeState(LOW); 
  //begin serial client
  Serial.begin(9600);
  Serial.println("...");
  // wait for serial port to connect 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  //TODO: add serial interaction for serial config of IP/login/pass/port 

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  //Define the socketIO 'on' listener with funciton. "light" is the event, and light is the function to call...
  client.on("onStateChange", light);

  
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(hostname);
  Serial.println("...");

  if (client.connect(hostname, hostnameshort, port))
  {
    Serial.println("DONE CONNECTING");
    //On first connect, toggle off.
    client.emit("toggleFromBoard", "false");
    lastping = millis();
   
  }
  else
  {
    Serial.println("CONNECTION ERROR");
    while(1);
  }
  delay(1000);
}

void loop() {
  //Get current value of button
  reading = buttonPin.state();
  client.emit("dataFromDevice", "test"); 
  client.monitor();

}

//TODO: why the fuck does this read 'ru' and 'als'...
void light(String state) {
  Serial.println("[light] " + state);
  if (state == "ru") {
    Serial.println("[light] ON");
    ledPin.changeState(HIGH); 
  }
  else {
    Serial.println("[light] off");
    ledPin.changeState(LOW); 
  }
}
