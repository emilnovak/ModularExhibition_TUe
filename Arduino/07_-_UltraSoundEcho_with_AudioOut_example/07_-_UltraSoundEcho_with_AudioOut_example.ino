//CAUTION! serial communication corrupts audio generation
//#define SERIAL_COMM_ENABLED 1
#define AUDIO_ENABLED 1     //make sure to only generate audio if Serial is OFF


#define trigPin 4
#define echoPin 3
#define speakerPin 5

#define trigInterval 50000
#define trigDuration 1000

#define commInterval 1500000    //time period between serial messages
#define processLengthInterval 12000   //time period between length processing

int trigState = LOW;
int speakerState = LOW;

int cm = 0;       //distance in cm.
unsigned long mm = 0; //distance in mm

unsigned long currentMicros = 0;        //elapsed time in microseconds
unsigned long previousMicrosTrig = 0;           //last time we sent a trigger
unsigned long previousMicrosComm = 0;           //last time we sent out Serial packets
unsigned long previousMicrosProcessLength = 0;  //last time we processed length
unsigned long measuredTime = 0;         //ping time in microseconds

unsigned long previousSpeakerTriggerMicros = 0;

//default pitch
unsigned long speakerInterval = 1000;



void echoEvent() {
  //Serial.print("echo-capture");
  //Serial.println();
  measuredTime = currentMicros - previousMicrosTrig;
  //Serial.print(measuredTime);
  //Serial.println();
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

long microsecondsToMillimeters(long microseconds) {
  // The speed of sound is 340 m/s or 3 microseconds per millimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 3 / 2;
}

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);

  //set pin interrupt on echoPin, calling echoEvent()
  pinMode(echoPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(echoPin), echoEvent, FALLING);

  //make sure trigPin is on LOW state
  digitalWrite(trigPin, LOW);
  
#ifdef SERIAL_COMM_ENABLED
  Serial.begin(9600);
  Serial.print("init ok");
  Serial.println();
#endif

}

void loop() {
  
  currentMicros = micros();
  
  //Timing of ping to be triggered
  if ((trigState == LOW) && (currentMicros - previousMicrosTrig >= trigInterval)) {
    previousMicrosTrig = currentMicros;
    //Serial.print("on");
    //Serial.println();
    trigState = HIGH;
    digitalWrite(trigPin,trigState);
  }
  else if ((trigState == HIGH) && (currentMicros - previousMicrosTrig >= trigDuration)) {
    previousMicrosTrig = currentMicros;
    //Serial.print("off");
    //Serial.println();
    trigState = LOW;
    digitalWrite(trigPin,trigState);
  }

  if (currentMicros - previousMicrosProcessLength >= processLengthInterval) {
    //cm = microsecondsToCentimeters(measuredTime);
    mm = microsecondsToMillimeters(measuredTime);
#ifdef AUDIO_ENABLED
    //speakerInterval = map(cm, 5, 100, 500, 6000);
    speakerInterval = map(mm, 20, 1000, 500, 6000);
#endif
    previousMicrosProcessLength = currentMicros;
  }
  
#ifdef SERIAL_COMM_ENABLED
  //Writing to serial (if havind valid reading)
  if ((measuredTime <= 6000) && (measuredTime > 10) && (currentMicros - previousMicrosComm >= commInterval)){
    //cm = microsecondsToCentimeters(measuredTime);

    Serial.println(mm);
    //Serial.print(cm);
    //Serial.print(" cm");
    //Serial.println();

    previousMicrosComm = currentMicros;
  }
#endif

#ifdef AUDIO_ENABLED
  if (currentMicros - previousSpeakerTriggerMicros >= speakerInterval) {
    speakerState = !speakerState;
    digitalWrite(speakerPin, speakerState);
    previousSpeakerTriggerMicros = currentMicros;
  }
#endif

}
