#define JOY_X A0
#define JOY_Y A1
#define THROTTLE A2

#define RUDDER_L 4
#define RUDDER_R 5

#define BRAKE_BTN 6
#define GEAR_BTN 7
#define FLAPS_DN 8
#define FLAPS_UP 9

int rudder = 512;
const int step = 12;
const int maxDef = 300;

// last states
bool lastBrake = HIGH;
bool lastGear  = HIGH;
bool lastFlapDn = HIGH;
bool lastFlapUp = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(RUDDER_L, INPUT_PULLUP);
  pinMode(RUDDER_R, INPUT_PULLUP);
  pinMode(BRAKE_BTN, INPUT_PULLUP);
  pinMode(GEAR_BTN, INPUT_PULLUP);
  pinMode(FLAPS_DN, INPUT_PULLUP);
  pinMode(FLAPS_UP, INPUT_PULLUP);
}

void loop() {
  int roll = analogRead(JOY_X);
  int pitch = analogRead(JOY_Y);
  int throttle = analogRead(THROTTLE);

  // ---------- RUDDER ----------
  bool left  = !digitalRead(RUDDER_L);
  bool right = !digitalRead(RUDDER_R);

  if (left && !right) rudder -= step;
  else if (right && !left) rudder += step;
  else {
    if (rudder > 512) rudder -= step;
    if (rudder < 512) rudder += step;
  }

  rudder = constrain(rudder, 512 - maxDef, 512 + maxDef);

  // ---------- BUTTON EDGE DETECTION ----------
  bool brakeNow  = digitalRead(BRAKE_BTN);
  bool gearNow   = digitalRead(GEAR_BTN);
  bool flapDnNow = digitalRead(FLAPS_DN);
  bool flapUpNow = digitalRead(FLAPS_UP);

  int brakePulse  = (lastBrake  == HIGH && brakeNow  == LOW);
  int gearPulse   = (lastGear   == HIGH && gearNow   == LOW);
  int flapDnPulse = (lastFlapDn == HIGH && flapDnNow == LOW);
  int flapUpPulse = (lastFlapUp == HIGH && flapUpNow == LOW);

  lastBrake  = brakeNow;
  lastGear   = gearNow;
  lastFlapDn = flapDnNow;
  lastFlapUp = flapUpNow;

  // ---------- SEND DATA ----------
  Serial.print(roll);
  Serial.print(",");
  Serial.print(pitch);
  Serial.print(",");
  Serial.print(throttle);
  Serial.print(",");
  Serial.print(rudder);
  Serial.print(",");
  Serial.print(brakePulse);
  Serial.print(",");
  Serial.print(gearPulse);
  Serial.print(",");
  Serial.print(flapDnPulse);
  Serial.print(",");
  Serial.println(flapUpPulse);

  delay(20);
}
