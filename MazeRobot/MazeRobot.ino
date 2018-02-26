
#include <Ultrasonic.h>
#include <AFMotor.h>

int TRIG_PIN1 = 50;
int ECHO_PIN1 = 46;
int TRIG_PIN2 = 26;
int ECHO_PIN2 = 24;

Ultrasonic frontPing(TRIG_PIN1, ECHO_PIN1);
Ultrasonic sidePing(TRIG_PIN2, ECHO_PIN2);

AF_DCMotor motor1(1, MOTOR12_8KHZ);
AF_DCMotor motor2(2, MOTOR12_8KHZ);

int debug = 0;
int etaisyysEdessa = 12;
int etaisyysSivulla = 18;
int nopeus = 150;

void setup() {
  Serial.begin(9600);
  motor1.setSpeed(nopeus - 20);
  motor2.setSpeed(nopeus);
}

void loop() {
  float frontDistance, sideDistance;
  float frontmicrosec = frontPing.timing();
  frontDistance = frontPing.CalcDistance(frontmicrosec,Ultrasonic::CM);
  float sidemicrosec = sidePing.timing();
  sideDistance = sidePing.CalcDistance(sidemicrosec,Ultrasonic::CM);

  if (debug == 1) {
    Serial.print("frontDistance: ");
    Serial.println(frontDistance);
    Serial.print("sideDistance: ");
    Serial.println(sideDistance);
  }

  // Korjataan suuntaa
  if (sideDistance <= 6) {
    pysahdy();
    kaannyVasemmalle(20);
  }

  // Jos este edessä, käännytään sivulle.
  // Jos ei seinää oikealla, käännytään oikealle
  // muuten vasemmalle
  if (frontDistance <= etaisyysEdessa) {
    pysahdy();
    delay(10);
    if (sideDistance >= etaisyysSivulla) {
      kaannyOikealle(260);
    } else {
      kaannyVasemmalle(260);
    }
  }

  // Jos ei seinää oikealla käännytään sinne
  if (sideDistance >= etaisyysSivulla) {
    delay(150);
    pysahdy();
    delay(10);
    kaannyOikealle(260);
  }
  
  eteenpain();
  delay(100);
}

void eteenpain() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void pysahdy() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void kaannyVasemmalle(int ms) {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(250);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  delay(ms);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(250);
  eteenpain();
  delay(100);
  pysahdy();
}

void kaannyOikealle(int ms) {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(250);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  delay(ms);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  delay(250);
  eteenpain();
  delay(100);
  pysahdy();
}
