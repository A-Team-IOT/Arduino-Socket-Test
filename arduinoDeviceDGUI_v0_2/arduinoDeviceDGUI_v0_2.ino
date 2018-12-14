#include <SocketIOClient.h>
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
    if(functionInput[0] == '!'){
      Serial.println("[WARN] " + functionInput.substring(1));  
    }else{
      Serial.println("[Log]  " + functionInput); 
    }
  }
}
 
class pin{
  int pinNumber = 0; 
  int previousState; 
      public:
      int getPin(){
      return this->pinNumber;
      }
      void setMode(uint8_t pinType){
        if(pinType == INPUT){
        pinMode(this->pinNumber, INPUT);
        }else{
        pinMode(this->pinNumber, OUTPUT);
        }
      }
      pin(int pinNumber, uint8_t pinType){
        this->pinNumber = pinNumber; 
        setMode(pinType); 
      }
      pin(){
      }
      uint8_t state(){
        if(pinNumber == 0){
          return 0; 
        }else{
          return digitalRead(this->pinNumber);
        }
      }
      uint8_t prevState(){
      return this->previousState;
      }
      void changeState(uint8_t state){
      this->previousState = digitalRead(this->pinNumber);
        digitalWrite(this->pinNumber, state);
      }
};
class component{
  public:
    String componentName = "nullcomponent"; 
    String componentType;
    bool state = false;   
    pin pinHolder = pin(); 
    component::component(String conComName, String conComType, pin conPinHolder){
    this->componentName = conComName; 
    this->componentType = conComType; 
    this->pinHolder = conPinHolder; 
    }
    component::component(){
    }
};
class DGUI{
  public:
  DGUI::DGUI(component functionInput){
  addComponent(functionInput); 
  DBF("DGUI class instantiated with 1 new component.");
  }
  //TODO need to pass the client from the socketIOlib to this for now
  //TODO implament this class into a lib that includes the components needed from socketIO
  //TODO add serial menu
  //TODO add switch class as a component of DGUI to eliminate currentpins array
  component currentComponent[100];
  int currentMaxComponentIndex = 0; 
  bool debugCheckValue = false; 
  String registerListener; 
  String switchListener; 
  String currentState; 
  char hostName[40]; 
  char hostNameShort[40]; 
  int port; 
  DGUI::DGUI(){
  DBF("DGUI class instantiated with no parameters."); 
  }
 
  int addComponent(component functionInput){
    // TODO add component to currentPins[] and add name to current pins as well  
    // functionInput = name of component; 
    // functionPin = pass it a pin that is configured; 
    // componentType = type of component; 
    bool returnObject = true;
    for(int i = 0; i < sizeof(this->currentComponent)/sizeof(component); i++){
      if(currentComponent[i].componentName == "nullcomponent"){
        currentComponent[i].componentName = functionInput.componentName;
        DBF("Component Added To Component List");
        if(this->debugCheckValue == true){
          printComponentInfo();  
        }
        break; 
      }
    }
     
    return returnObject; 
  }
  int removeComponent(String functionInput){
    //remove component
    bool returnObject = true; 
    //TODO remove component by name 
    return returnObject; 
  }
  size_t currentComponentSize(){
    return (sizeof(currentComponent) / sizeof(component)); 
  }
  int printComponentInfo(){
    bool returnObject = true;  
    int currentLength = static_cast<int>(this->currentComponentSize());
    for(int i = 0; i < currentLength; i++){
      Serial.println("Pin: " + this->currentComponent[i].componentName + " State: " + this->currentComponent[i].pinHolder.state());   
    }
    return returnObject; 
  }
  int createJsonObject(){
    bool returnObject = true;
    //TODO create json from components array  
    return returnObject; 
  }
  int sendJsonObjectRegister(String functionInput){
    bool returnObject = true; 
      //TODO send register json
      DBF("JSON [REGISTER]: " + functionInput); 
    return returnObject; 
  }
  int sendJsonObjectState(String functionInput){
    bool returnObject = true; 
    //TODO send state json
    DBF("JSON [STATE]: " + functionInput); 
    return returnObject; 
  }
  int checkConnection(){
    bool returnObject = false;
      if (client.connect(this->hostName, this->hostNameShort, this->port))
          {
            DBF("Client Connection Successful");
            DBF(" " + String(this->hostName) + "  " + String(this->hostNameShort) + ":" + String(this->port));  
              // give the Ethernet shield a second to initialize:
            delay(1000);
            Serial.print("connecting to ");
            Serial.print(this->hostName);
            returnObject = true;
          }
          else
          {
            DBF("!Client Connection Failure");  
            if(strlen(this->hostName) | this->hostName == NULL | hostName == ""){
            DBF("!variable hostname is empty"); 
            }
            if(this->hostNameShort == NULL | this->hostNameShort == ""){
            DBF("!variable hostnameshort is empty"); 
            }
            if(this->port == NULL | String(this->port) == ""){
            DBF("!variable port is empty"); 
            }
            returnObject = false;
          }
    return returnObject; 
  }
  void setHostInfo(const String inputHostName,const String inputHostNameShort, int inputPort){
   //char arrays suck and so I did it this way because I would have to iterate through the input anyways if it was a char array :*(
   for(int i = 0; i < inputHostName.length(); i++){
      this->hostName[i] = inputHostName[i]; 
    }
    DBF(this->hostName);
  }
  int state(String functionInput){
    //this is redundant at this moment - want to add more component states/checks and balances to turning state off
    //this turns all components to off - no IE communication to the server 
    if(functionInput == "on"){
      this->currentState = "on";
         int currentLength = static_cast<int>(this->currentComponentSize());
         for(int i = 0; i < currentLength; i++){
           if(currentComponent[i].componentName != "nullcomponent"){
           currentComponent[i].state = true;    
           }
         }
      DBF("!ALL COMPONENTS SET TO ON"); 
    }
    if(functionInput == "off"){
      this->currentState = "off";
         int currentLength = static_cast<int>(this->currentComponentSize());
         for(int i = 0; i < currentLength; i++){
           if(currentComponent[i].componentName != "nullcomponent"){
           currentComponent[i].state = false;   
           }
         } 
      DBF("!ALL COMPONENTS SET TO OFF"); 
    }
  }
  void debugCheck(){
    if(this->debugCheckValue == true){
      //print important debug info about this current instance
      Serial.println("Debug check is set"); 
    }else{
      Serial.println("Debug check is not set");
    }
  }
  void setDebugCheck(int functionInput){
    if(functionInput == 0){
      this->debugCheckValue = false;
    }else{
      this->debugCheckValue = true; 
    }
  }
  
  }
  void wsLog(String functionInput){
    //TODO add emit to log listener 
    DBF(functionInput); 
  }
  void wsCurrentState(){
    //TODO add emit current state 
    
  }
};


void togglePin(class pin functionInput){
  if(functionInput.state() == HIGH){
    functionInput.changeState(LOW);
    DBF("Pin: " + String(functionInput.getPin()) + " State: " + String(functionInput.state())); 
  }else{
    functionInput.changeState(HIGH);
    DBF("Pin: " + String(functionInput.getPin()) + " State: " + String(functionInput.state())); 
  }
}

pin ledPin(7, OUTPUT);
pin buttonPin(8, INPUT); 

long time = 0;         // the last time the output pin was toggled
long debounce = 600;   // the debounce time, increase if the output flickers
long lastping = 0;

byte mac[] = { 0x30, 0x30, 0x32, 0x38, 0x46, 0x38, 0x43, 0x43, 0x38, 0x35, 0x46, 0x44  };

unsigned long previousMillis = 0; 
long interval = 10000; 

void setup() {
  ledPin.changeState(LOW); 
  buttonPin.changeState(LOW); 
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  } 
  
  // start the Ethernet connection:
//  Serial.println("Initialize Ethernet with DHCP:");
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
//    // Check for Ethernet hardware present
//    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
//      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
//      while (true) {
//        delay(1); // do nothing, no point running without Ethernet hardware
//      }
//    }
//    if (Ethernet.linkStatus() == LinkOFF) {
//      Serial.println("Ethernet cable is not connected.");
//    }
//    // try to congifure using IP address instead of DHCP:
//    Ethernet.begin(mac);
//  } else {
//    Serial.print("  DHCP assigned IP ");
//    Serial.println(Ethernet.localIP());
//  }
  //create a component 

  component demo; 
  demo.componentName = "testCom"; 
  DGUI testUI(demo);
  testUI.checkConnection();
  testUI.state("on");  
  
}

void loop() {
  //TODO: check button state and change ledpin based on button press

}


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
