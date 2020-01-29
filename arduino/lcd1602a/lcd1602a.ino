// lcd1602a - LCD handler

// Pins
#define RS  12
#define EN  11
#define D4  5
#define D5  4
#define D6  3
#define D7  2

#define NEXT 
#define PREV
#define SCRL_R
#define SCRL_L
#define DEL

#define ETX 0x03
#define EOT 0x04

#define LS  7

#include <LiquidCrystal.h>

class message{
public: 
  char sender[17];
  char content[255];
  uint8_t contentLen;
  message* next;
  message* prev;
};

char displayMessage[18];

int page = 0;
uint8_t offset = 0;
int waitCounter;

message firstMessage;
message *newMessage;
message *thisMessage;

typedef void (*buttonCallback)(void);

LiquidCrystal lcd(RS,EN,D4,D5,D6,D7);

void setup() {

  // Open serial port
  Serial.begin(9600);

  // Begin LCD screen
  lcd.begin(16,2);

  // Buttons
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);

  // 
  pinMode(13,OUTPUT);

  // Print menu screen
  lcd.clear();

}

void loop() {

  // Wait for new data
  if (Serial.available()>1){

    // Iterate to last item in linked list
    newMessage = &firstMessage;
    while (newMessage->next != NULL){
      newMessage = newMessage->next;
    }

    // Add a new message to the end of the linked list
    newMessage->next = new message;

    // Add the previous last message as a previous message
    newMessage->next->prev = newMessage;
    
    // Go to that newMessage to fill in the data
    newMessage = newMessage->next;

    // Read until unit separator
    int len = 0;
    len = Serial.readBytesUntil(ETX,newMessage->sender,15);
    
    newMessage->sender[len] = ':';
    newMessage->sender[len+1] = '\0';
    replaceSpaces(newMessage->sender);

    // Read until endchar
    len = Serial.readBytesUntil(EOT,newMessage->content,254);
    newMessage->content[len] = '\0';
    replaceSpaces(newMessage->content);
    
    newMessage->contentLen = len;
    newMessage->next = NULL;

    // Automatically display new message
    thisMessage = newMessage;

    Serial.flush();

    offset = 0;
    

    beep(13,880,1000);
    lcd.clear();

  }

  else if (Serial.available()){
    Serial.read();
  }

  // Display
  delay(50);
  
  // Create substring to display
  for (int i=0;i<16;i++){
    if (i+offset < thisMessage->contentLen){
      displayMessage[i] = thisMessage->content[i+offset];
    }
    else{
      displayMessage[i] = ' ';
    }
  }
  displayMessage[17] = '\0';

  // Print display message
  lcd.setCursor(0,0);
  lcd.print(thisMessage->sender);
  lcd.setCursor(0,1);
  lcd.print(displayMessage);

  if (!offset){
    waitCounter++;
    if (waitCounter>=200){
      waitCounter = 0;
      offset++;
    }
  }
  else if (offset > (thisMessage->contentLen)){
    offset = 0;
  }
  else {
    waitCounter++;
    if (waitCounter >= 10){
      waitCounter=0;
      offset++;
    }
  }

  //pollButton(6,&);
  pollButton(7,&nextMessage);
  //pollButton(8,);
  pollButton(9,&prevMessage);
  pollButton(10,&deleteMessage);

}

// Poll buttons
void pollButton(int pin, buttonCallback callback){
  
  if(!digitalRead(pin)){
    callback();
  }

}

// Button handlers
void nextMessage(){
  if (thisMessage->next != NULL){
    beep(13,880,100);
    thisMessage=thisMessage->next;
    waitCounter = 0;
  }
  else{
    beep(13,880,40);
    delay(40);
    beep(13,880,40);
  }
}

void prevMessage(){
  if ((thisMessage->prev != NULL)&(thisMessage->prev->prev != NULL)){
    beep(13,880,100);
    thisMessage=thisMessage->prev;
    waitCounter = 0;
  }
  else{
    beep(13,880,40);
    delay(40);
    beep(13,880,40);
  }
}

void deleteMessage(){
  beep(13,880,200);
  delay(100);
  beep(13,880,200);
  // Change pointers in linked list
  if (thisMessage->prev != NULL){
    thisMessage->prev->next = thisMessage->next;
  }

  if (thisMessage->next != NULL){
    thisMessage->next->prev = thisMessage->prev;
  }

  // Get pointer to message being deleted
  message* deleteMessage = thisMessage;

  // Make thisMessage either previous or next message
  if((thisMessage->prev != NULL)&(thisMessage->prev->prev != NULL)){

    thisMessage = thisMessage->prev;
    delete(deleteMessage);
    
  }
  else if(thisMessage->next != NULL){

    thisMessage = thisMessage->next;
    firstMessage = *thisMessage;
    delete(deleteMessage);
    
  }
  else{

    // If this is the only message
    for (int i=0;i<17;i++){
      thisMessage->sender[i] = ' ';
    }

    for (int i=0;i<255;i++){
      thisMessage->content[i] = ' ';
    }

    lcd.setCursor(0,0);
    lcd.print("                ");
    //lcd.clear();
  }
  
  waitCounter = 0;
}

// Other
void beep(int pin, float hz, float dur) {
  int halfT = 500000 / hz;
  for (int i = 0; i < (500 * dur / halfT) ; i++) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(halfT);
    digitalWrite(pin, LOW);
    delayMicroseconds(halfT);
  }
}

void replaceSpaces(char string[]){
  int i=0;
  while (string[i] != '\0'){

    if(string[i] == '+'){
      string[i] = 0x20;
    }

    i++;
    
  }
  
}
