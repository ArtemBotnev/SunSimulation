//
// Created by Artem Botnev on 10/16/2018
//

#ifndef SUNSIMULATION_H
#define SUNSIMULATION_H

#define PWM_MAX 255

class TimeSet {
public:
    TimeSet(short hour, short minute, short duration);

    short getHour();

    short getMinute();

    short getDuration();

private:
    short _hour;
    short _minute;
    short _duration;
};

#endif