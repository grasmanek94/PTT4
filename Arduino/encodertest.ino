int encoderA = 4;
int encoderB = 6;

int previousA;
int previousB;

int count;
long lastTime;
int skip;

void setup() {
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  Serial.begin(9600);
  lastTime = millis();
}

void loop() {
  int A = digitalRead(encoderA);
  int B = digitalRead(encoderB);

  // need to skip one edge because 
  // 1 puls has 2 edges
  if (A != previousA)
  {
    if (skip)
    {
      skip = 0;
    }
    else
    {
      ++count;
      skip = 1;
    }
  }
  else if (B != previousB)
  {
    if (skip)
    {
      skip = 0;
    }
    else
    {
      --count;
      skip = 1;
    }
  }

  previousA = A;
  previousB = B;

  long currentTime = millis();
  if ((lastTime + 250) > currentTime)
  {
    lastTime = currentTime;
    Serial.println(count);
  }
}
