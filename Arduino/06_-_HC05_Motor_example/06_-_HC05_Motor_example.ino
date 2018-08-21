#include <Servo.h>

#define MOTOR1_PIN1 3
#define MOTOR1_PIN2 5
#define MOTOR2_PIN1 6
#define MOTOR2_PIN2 9

#define SERVO 10

String data;

int motor1Target = 0;
int motor1Speed = 0;
int motorStep = 10;

Servo servo;

void setup() {
  Serial.begin(9600);

  servo.attach(10);
  servo.write(90);

  digitalWrite(MOTOR1_PIN1, 0);
  digitalWrite(MOTOR1_PIN2, 0);
  digitalWrite(MOTOR2_PIN1, 0);
  digitalWrite(MOTOR2_PIN2, 0);

  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);

}

void loop() {
  if (Serial.available()) {
    delay(100);
    data = "";
    while (Serial.available()) {
      char c = Serial.read();
      data += c;
    }

    if (data == "upPress") {
      forward();
    }
    else if (data == "upRelease") {
      Stop();
    }
    else if (data == "downPress") {
      backward();
    }
    else if (data == "downRelease") {
      Stop();
    }
    else if (data == "leftPress") {
      servo.write(0);
    }
    else if (data == "leftRelease") {
      servo.write(90);
    }
    else if (data == "rightPress") {
      servo.write(180);
    }
    else if (data == "rightRelease") {
      servo.write(90);
    }
  }

  updateMotors();

}

void forward() {
  motor1Target = 255;
}

void Stop() {
  motor1Target = 0;
}

void backward() {
  motor1Target = -255;
}

void updateMotors() {
  if (motor1Target > motor1Speed + motorStep) {
    motor1Speed += motorStep;
  } else if (motor1Target < motor1Speed - motorStep) {
    motor1Speed -= motorStep;
  } else {
    motor1Speed = motor1Target;
  }

  if (motor1Speed > 0) {
    digitalWrite(MOTOR1_PIN1, LOW);
    analogWrite(MOTOR1_PIN2, motor1Speed);
  } else if (motor1Speed < 0) {
    digitalWrite(MOTOR1_PIN2, LOW);
    analogWrite(MOTOR1_PIN1, abs(motor1Speed));
  } else {
    digitalWrite(MOTOR1_PIN1, LOW);
    digitalWrite(MOTOR1_PIN2, LOW);
  }
}

