#define LED LED_BUILTIN

#define MIN_VALUE 100
#define MAX_VALUE 500

uint16_t currentValue = 0;
uint16_t delta = 0;
long prevTime = 0;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

}


void loop() {
  currentValue = clip(analogRead(0), MIN_VALUE, MAX_VALUE);
  Serial.println(analogRead(0));
  delta = map(currentValue, MIN_VALUE, MAX_VALUE, 1000, 0);


  if(millis() - prevTime >= delta) {
    prevTime = millis();
    digitalWrite(LED, HIGH - digitalRead(LED));
  }
}

int clip(int value, int _min, int _max) {
  if (_min > value)
    return _min;
  if (_max < value)
    return _max;
  return value;
}
