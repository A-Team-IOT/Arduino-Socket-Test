
int ledPin = 7;

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
        public:pin::pin(int pinNumber, int pinType){
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
  pin test(ledPin, 1);

void setup() {
  test.changeState(HIGH); 
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  delay(2000); 
  test.changeState(LOW);

}
