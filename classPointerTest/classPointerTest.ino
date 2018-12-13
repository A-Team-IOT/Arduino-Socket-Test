

class testClass{
  public:
  char charBuffer[40] = "";
  void addCharArray(String testString){
    for(int i = 0; i < testString.length(); i++){
    this->charBuffer[i] = testString[i]; 
    }
  }
  void printArray(){
    Serial.println(this->charBuffer); 

  }
};

testClass test; 
String testString = "test and stuff and thingsssssss"; 
 

void setup() {
Serial.begin(9600); 
test.addCharArray("this test here and things"); 

}

void loop() {
test.printArray(); 

}
