#include <socketIOClient.h>
#define W5100
#include <Ethernet.h>
#include "SPI.h"


/*
 * This is a test to connect an arduino uno with a W5100 ethernet shield to a NodeJS server with SocketIO. 
 * The corresponding circuit is just an led on pin 7 and a button on pin 8. When the button is clicked,
 * the light toggles on and off, and emits to the server which updates it's UI accordingly. The server can
 * emit to the board, which will also light up the led. The hostname is set to 'https://witty-cat-82.localtunnel.me',
 * which is a subdomain i use with localtunnel for testing. This should be changed accordingly.
**/


SocketIOClient client;
//base program serial print debug flag
bool localDBFB = true; 

void DBF(String functionInput){
  if(localDBFB == true){
  Serial.println(functionInput); 
  }
}
 
class pin{
  int pinNumber; 
  int previousState; 
      public:int getPin(){
      return this->pinNumber;
      }
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
  //TODO need to pass the client from the socketIOlib to this for now
  //TODO implament this class into a lib that includes the components needed from socketIO
  //TODO add serial menu
  //TODO add switch class as a component of DGUI to eliminate currentpins array
  pin currentPins[];
  bool debugCheckValue = false; 
  String currentPinsName[]; 
  String registerListener; 
  String switchListener; 
  String currentState; 
  char hostName[40]; 
  char hostNameShort[40]; 
  int port; 
  int addComponent(String functionInput, pin functionPin, String componentType){
    // TODO add component to currentPins[] and add name to current pins as well  
    // functionInput = name of component; 
    // functionPin = pass it a pin that is configured; 
    // componentType = type of component; 
    bool returnObject = true; 
    return returnObject; 
  }
  int removeComponent(String functionInput){
    //remove component
    bool returnObject = true; 
    //TODO remove component by name 
    return returnObject; 
  }
  size_t currentComponentSize(){
    return (sizeof(currentPins) / sizeof(pin)); 
  }
  int printComponentInfo(){
    bool returnObject = true;  
    int currentLength = static_cast<int>(this->currentComponentSize());
    for(int i = 0; i < currentLength; i++){
      Serial.println(this->currentPinsName[i]);
      Serial.println(this->currentPins[i].state());   
    }
    return returnObject; 
  }
  int createJsonObject(){
    bool returnObject = true;
    //TODO create json string  
    return returnObject; 
  }
  int sendJsonObjectRegister(){
    bool returnObject = true; 
    //TODO send register json
    return returnObject; 
  }
  int sendJsonObjectState(){
    bool returnObject = true; 
    //TODO send state json
    return returnObject; 
  }
  int checkConnection(){
    bool returnObject = false;
      if (client.connect(this->hostName, this->hostNameShort, this->port))
          {
            DBF("Client Connection Successful");
            //TODO fix these empty tests - they are broken as of right now - I dont think I can compare char array to string
            DBF(" " + String(this->hostName) + "  " + String(this->hostNameShort) + ":" + String(this->port));  
            if(this->hostName == NULL | hostName == ""){
            DBF("variable hostname is empty"); 
            }
            if(this->hostNameShort == NULL | this->hostNameShort == ""){
            DBF("variable hostnameshort is empty"); 
            }
            if(this->port == NULL | String(this->port) == ""){
            DBF("variable port is empty"); 
            }
            returnObject = true;
          }
          else
          {
            DBF("Client Connection Failure"); 
            returnObject = false;
          }
    return returnObject; 
  }
  void setHostInfo(char inputHostName[], char inputHostNameShort[], int inputPort){
    *this->hostName = *inputHostName; 
    *this->hostNameShort = *inputHostNameShort; 
    this->port = inputPort;
  }
  int state(String functionInput){
    //this is redundant at this moment - want to add more states and checks and balances to turning state off
    //this turns all components to off - no communication to the server 
    if(functionInput == "on"){
      this->currentState = "on";
    }
    if(functionInput == "off"){
      this->currentState = "off"; 
    }
  }
  void debugCheck(){
    if(this->debugCheckValue == true){
      //print important debug info about this current instance
      Serial.println("Debug check is set"); 
    }
  }
  void wsLog(String functionInput){
    //TODO add emit to log listener 
    DBF(functionInput); 
  }
};

pin ledPin(7, OUTPUT);
pin buttonPin(8, INPUT);

void togglePin(class pin functionInput){
  if(functionInput.state() == HIGH){
    functionInput.changeState(LOW);
    DBF("Pin: " + String(functionInput.getPin()) + " State: " + String(functionInput.state())); 
  }else{
    functionInput.changeState(HIGH);
    DBF("Pin: " + String(functionInput.getPin()) + " State: " + String(functionInput.state())); 
  }
}

 

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

//IPAddress ip(68, 183, 70, 213);
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
  //TODO add the implementation of the DGUI
  ledPin.changeState(LOW); 
  buttonPin.changeState(LOW); 
  Serial.begin(9600);
  Serial.println("...");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  } 
  
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
  //TODO remove connection test and replace with DGUI connection test 

}

void loop() {
  //Get current value of button
  reading = buttonPin.state();
  client.emit("dataFromDevice", "test"); 
  client.monitor();
  //TODO: check button state and change ledpin based on button press

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
