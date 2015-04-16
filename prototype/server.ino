#include <Servo.h>

// Determine whether an RFID is allowed in
bool access;

// The only currently allowed RFID
byte allowed[13] = { 48, 48, 48, 52, 55, 51, 51, 50, 57, 48, 13, 10 }; 

// Read in RFID codes here
byte readin[13];

// Servo object, just one to control the door
Servo servo;

// Time, so we can restart the RFID radio regularly
unsigned long starttime;

// Pins
#define SERVO 6
#define RFIDPOWER 12
#define RFID 10
#define RFIDRESET 11



  
void setup() {
  
  // Attach the servo and close the door
  servo.attach(SERVO);
  servo.write(0);
  
  // Set pins
  pinMode(RFID, INPUT);
  pinMode(RFIDPOWER, OUTPUT);
  
  
  // Flip the door open
  delay(1000);
  servo.write(255);
 
  // Start counting
  starttime = millis();
  
  // Turn on the serial and power up the RFID radio
  Serial.begin(9600);
  delay(500);
  digitalWrite(RFIDPOWER, HIGH);
     
  // Be nice to your user
  Serial.println("Initialised serial connection, ready for card-reading action.\n");
}





void loop() {
  
  // If we receive some serial communication
  if(Serial.available() > 0) {
   
    // By default, we have access, this is the right mouse
    access = 1;


    // Read in twelve bytes into readin and print to serial monitor
    Serial.println("Read a card ! ID :");
    
    for (int i = 0; i < 12; ++i) {
      
      readin[i] = Serial.read();
      
      // If any of these bytes are wrong, deny access, this is the wrong mouse
      if (readin[i] != allowed[i]) {
        access = 0;
      }     
      
      Serial.print(readin[i]);
      Serial.print(" ");
    }
    
    
    // If we don't want this particular mouse, deny access
    if (digitalRead(RFID) == LOW)
      access = 0;
      
      
  
    // If it's the right mouse, tell the user and close the door
    if (access) {
      servo.write(0);
      Serial.println("\nACCESS GRANTED !\n");
    }

    else {
      Serial.println("\nAccess denied !\n");
    }

  }
  
  else {
    delay(100); // wait 100 ms between reads
    
    // If we trigger the reset pin, then open the door
    if (digitalRead(RFIDRESET) == HIGH)
      servo.write(255);
    
    
    // If it's been one minute, restart the RFID reader, it tends to freeze
    if (millis() - starttime > 60000) {
      digitalWrite(RFIDPOWER, LOW);
      delay(1000);
      digitalWrite(RFIDPOWER, HIGH);
      starttime = millis();
    }
    
  }
  
}


