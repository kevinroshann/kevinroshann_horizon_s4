#include <Servo.h>

Servo servos[6];
int initialangles[6] = {90, 90, 90, 90, 90, 90}; 
int targetAngles[6];

void setup() {
    int servoPins[6] = {3, 5, 6, 9, 10, 11};
    for (int i = 0; i < 6; i++) {
        servos[i].attach(servoPins[i]);
        servos[i].write(initialangles[i]);
    }
    Serial.begin(9600);
}

void loop() {
    if (Serial.available() > 0) {
        for (int i = 0; i < 6; i++) {
            targetAngles[i] = Serial.parseInt(); 
            if (targetAngles[i] < 0 || targetAngles[i] > 180) targetAngles[i] = 90;
        }

        bool moving = true;
        while (moving) {
            moving = false;
            for (int i = 0; i < 6; i++) {
                if (initialangles[i] < targetAngles[i]) {
                    initialangles[i]++;
                    moving = true;
                } else if (initialangles[i] > targetAngles[i]) {
                    initialangles[i]--;
                    moving = true;
                }
                servos[i].write(initialangles[i]);
            }
           // we can add a delay if needed for smoother movement but i am not doing it nor
        }
    }
}
