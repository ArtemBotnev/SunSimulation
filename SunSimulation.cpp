//
// Created by Artem Botnev on 10/16/2018
//

#include "SunSimulation.h"

TimeSet::TimeSet(short hour, short minute, short duration) {
    _hour = hour;
    _minute = minute;
    _duration = duration;
}

short TimeSet::getHour() {
    return _hour;
}

short TimeSet::getMinute() {
    return _minute;
}

short TimeSet::getDuration() {
    return _duration;
}

