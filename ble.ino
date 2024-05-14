#include <SoftwareSerial.h>

SoftwareSerial HM10(2, 3); // RX = 3, TX = 2
const int hackpin = 4;
const int bluetooth = 5;
const int b1 = 8;
const int b2 = 9;
const int buzz = 10;
char appData;  
int blue = 1;
String inData = "";


class Box{
  public:
  String Action;
  String Key;
  Box(String action, String key){
    Action = action;
    Key = key;
  }
};

Box box1("2", "");
Box box2("2", "");

void setup()

{

  Serial.begin(9600);

  Serial.println("HM10 serial started at 9600");

  HM10.begin(9600); // set HM10 serial at 9600 baud rate

  pinMode(13, OUTPUT); // onboard LED

  digitalWrite(13, LOW); // switch OFF LED

  pinMode(hackpin, OUTPUT);

  digitalWrite(hackpin, LOW);

  pinMode(bluetooth, OUTPUT);

  digitalWrite(bluetooth, LOW);

  pinMode(b1, OUTPUT);

  digitalWrite(b1, HIGH);

  pinMode(b2, OUTPUT);

  digitalWrite(b2, HIGH);

  pinMode(buzz, OUTPUT);

  digitalWrite(buzz, LOW);

  pinMode(12, OUTPUT);

  digitalWrite(12, LOW);
  delay(1000);
  digitalWrite(12, HIGH);

}


String data = "";




// verify message

void verifyMessage(String s){

  Serial.print("In verification");

  int i =0;

  while(s[i]!='{'){
    i=i+1;
  }

  i = i+1;

  // Box number
  String temp="";
  temp+=s[i];
  Serial.print("\n");
  Serial.print(temp);

  int boxnum = 0;

  if(temp.equals("1")){
    boxnum =1;
  }
  else if(temp.equals("2")){
    boxnum = 2;
  }

  if(boxnum == 0){
    digitalWrite(hackpin, HIGH);
    return ;
  }

  //find action
  temp = "";
  i= i+2;
  temp+=s[i];
  Serial.print("\n");
  Serial.print(temp);

  //verify action
  if(boxnum == 1){
    if(box1.Action.equals(temp)){
      digitalWrite(hackpin, HIGH);
      return;
    }
  }
  else{
    if(box2.Action.equals(temp)){
      digitalWrite(hackpin, HIGH);
      return;
    }
  }

  // verify key
  temp="";
  i=i+2;
  while(s[i]!=' '){
    temp+= s[i];
    i=i+1;
  }
  Serial.print("\n");
  Serial.print(temp);

  if(!temp.equals("HyperRoute1845")){
    digitalWrite(hackpin, HIGH);
    return;
  }

  //pin 
  temp = "";
  i=i+1;
  while(s[i]!='}'){
    temp+=s[i];
    i=i+1;
  }
  Serial.print("\n");
  Serial.print(temp);
  Serial.print("\n");

  
  //verify pin
  if(boxnum == 1){
    if(box1.Action.equals("1")){
      //handle receive
      if(box1.Key.equals(temp)){
        box1.Action = "2";
        digitalWrite(b1, LOW);
        digitalWrite(buzz, HIGH);
        //handle opening box
        delay(1000);
        digitalWrite(buzz, LOW);
        // digitalWrite(b1, LOW);
        delay(1000);
        digitalWrite(buzz, HIGH);
        // digitalWrite(b1, HIGH);
        delay(1000);
        digitalWrite(buzz, LOW);
        digitalWrite(b1, HIGH);
      }
      else{
        digitalWrite(hackpin, HIGH);
        return;
      }
    }
    else{
      box1.Key = temp;
      box1.Action = "1";
      digitalWrite(b1, LOW);
      digitalWrite(buzz, HIGH);
      //handle open
      delay(3000);
      digitalWrite(buzz, LOW);
      digitalWrite(b1, HIGH);
    }
  }
  else{
    Serial.print("In box 2");
    if(box2.Action.equals("1")){
      if(box2.Key.equals(temp)){
        box2.Action = "2";
         digitalWrite(b2, LOW);
         digitalWrite(buzz, HIGH);
        //handle opening box
         delay(1000);
        //  digitalWrite(b2, LOW);
         digitalWrite(buzz, LOW);
         delay(1000);
        //  digitalWrite(b2, HIGH);
         digitalWrite(buzz, HIGH);
         delay(1000);
         digitalWrite(b2, HIGH);
         digitalWrite(buzz, LOW);
      }
      else{
        digitalWrite(hackpin, HIGH);
        return;
      }
    }
    else{
      Serial.print("In box 2 send");
      box2.Key = temp;
      box2.Action = "1";
      digitalWrite(b2, LOW);
      digitalWrite(buzz, HIGH);
      //handle open
      delay(3000);
      digitalWrite(b2, HIGH);
      digitalWrite(buzz, LOW);
    }
  }

  return;
}

void loop()

{

  HM10.listen();  // listen the HM10 port
  while (HM10.available() > 0) { 
  // if HM10 sends something then read
    digitalWrite(bluetooth, HIGH);
    appData = HM10.read();
    data+=appData;
    Serial.print(appData);
    blue = 2;
  }

  if(data.endsWith("OK+LOST")){
    digitalWrite(bluetooth, LOW);
    data.trim();
    Serial.print("data is -"+data);
    verifyMessage(data);
    data="";
  }
  digitalWrite(bluetooth, LOW);
}