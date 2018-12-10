#define W5100
#include "socketIOClient.h"
//changed S to s for my local machine as this is renamed 
#include "Ethernet.h"
#include "SPI.h"


#define ledPin 7
#define inPin 8

SocketIOClient client;

bool ledOn = false;
bool clicked = false;
int state1 = HIGH;      // the current state of the output pin
int reading;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

long time = 0;         
long debounce = 600;   
long lastping = 0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//TODO: make subdomain its own variable, im too stupid for this.
char hostname[] = "https://witty-cat-82.localtunnel.me";
char hostnameshort[] = "witty-cat-82.localtunnel.me";

//localtunnel uses 80
int port = 80;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
IPAddress myDns(192, 168, 0, 1);

  
//Pretty sure i dont need these anymore.
extern String RID;
extern String Rname;
extern String Rcontent;

unsigned long previousMillis = 0; 
long interval = 10000; 
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(inPin, INPUT);
  //pinMode(10, OUTPUT);    
  //digitalWrite(10, LOW);
  //pinMode(4, OUTPUT);
  //digitalWrite(4, HIGH);
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
        delay(1); 
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    //Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }


  client.on("dev123", light);
  
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(hostname);
  Serial.println("...");

  if (client.connect(hostname,  hostnameshort, port))
  {
    Serial.println("DONE CONNECTING");
    //On first connect, toggle off.
    client.emit("register", "test this register thing here");
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
  reading = digitalRead(inPin);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading == HIGH && previous == LOW && millis() - time > debounce) {Serial.println("click");
    if (state1 == HIGH)
      state1 = LOW;
    else
      state1 = HIGH;

    time = millis();  
    if(!ledOn){
      digitalWrite(ledPin, HIGH);
      client.emit("deviceEvent",  "1");
      lastping = millis();
      Serial.println("test");
      ledOn = true;
    } 
    else{
      client.emit("deviceEvent", "0");
      lastping = millis();
      digitalWrite(ledPin, LOW);
      ledOn = false;
      Serial.println("test");
    }
  }
  client.monitor();

   if(millis() - lastping >= 50000){
      client.emit("heartbeat", "true");
      lastping = millis();
   }
   //added bull shit test loop for fun  
        //client.emit("deviceEvent", "0");
        //delay(5000);
  
}

//TODO: why the fuck does this read 'ru' and 'als'...
void light(String state) {
  //client.emit("deviceEvent", "0"); 
  //Serial.println("[light] " + state);
  Serial.println(ledOn); 
  if (ledOn == 1) {
    Serial.println("[light] 1");
    client.emit("deviceEvent", "1"); 
    ledOn = 0;
    digitalWrite(7, HIGH);
  }
  else {
    Serial.println("[light] 0");
    client.emit("deviceEvent", "0");
    ledOn = 1;
    digitalWrite(7, LOW);
  }
}
