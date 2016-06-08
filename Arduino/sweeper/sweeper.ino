#include <Servo.h>
#include <Arduino.h>

class MarblePassThrough
{
    long time_start;
    long time_end;
    int pos_start;
    int pos_now;
    int pos_end;
    Servo servo;
    bool move_state;
    long move_time;
        
    int detector_pin;
    long last_detect_time;
    long detect_time;
public:
    MarblePassThrough(int servoPin, int detectorPin)
    {
        //pinMode(detectorPin, INPUT_PULLUP);   
        servo.attach(servoPin);
        
        move_state = true;
        detector_pin = detectorPin;
        move_time = 5000;
        detect_time = 1000;
        last_detect_time = 0;   
        pos_now = 0;
             
        Begin();
    }

    ~MarblePassThrough()
    {
        
    }

    bool Tick()
    {
        servo.write(100);
        delay(5000);
        long time_now = millis();
        if(time_now >= time_start && time_now <= time_end)
        {
            long time_diff = time_now - time_start;
            float percentage = ((float)time_diff) / ((float)(move_time));
            int pos_calculated = pos_start + (int)(((float)(pos_end - pos_start)) * percentage);
            if(abs(pos_calculated - pos_now) >= 10)
            {
                pos_now = pos_calculated;
                servo.write(100);
                delay(50);
                Serial.println(pos_now);
            }
            
            return false;
        }
        else
        {
            servo.write(pos_end);
            return true;
        }
    }

    void Begin()
    {
        pos_start = 130;
        pos_end = 0;
        time_start = millis();
        time_end = time_start + move_time;
        move_state = true;
    }

    void End()
    {
        pos_start = 0;
        pos_end = 130;   
        time_start = millis();
        time_end = time_start + move_time;      
        move_state = false;       
    }

    void Switch()
    {
        if(!move_state)
        {
            Begin();
        }
        else
        {
            End();
        }
    }
    
    bool GetMoveState()
    {
        return move_state;
    }

    bool DetectedMarble()
    {
        long time_now = millis();
        if(time_now < (last_detect_time + detect_time))
        {
            return false;
        }
        
        bool ballPassed = digitalRead(detector_pin);        
        if(!ballPassed)
        {
            return false;
        }
        
        last_detect_time = time_now;
        return true;       
    }
};

MarblePassThrough passer(3, 2);
bool lightning = false;

void setup() 
{
    Serial.begin(9600);
    pinMode(13, OUTPUT);
}

void loop() 
{
    if(passer.Tick())
    {
        passer.Switch();
        Serial.println("S");
    }

    if(passer.DetectedMarble())
    {
        lightning ^= 1;
        digitalWrite(13, lightning);
        Serial.println("DM");
    }
}

