//
// Created by Artem Botnev on 10/16/2018
//

#include "SunSimulation.h"

TimeSet::TimeSet(uint8_t hour, uint8_t minute, uint16_t duration) {
    _hour = hour;
    _minute = minute;
    _duration = duration;
}

uint8_t TimeSet::getHour() {
    return _hour;
}

uint8_t TimeSet::getMinute() {
    return _minute;
}

uint16_t TimeSet::getDuration() {
    return _duration;
}

SunSimulation::SunSimulation(TimeSet *sunRise, TimeSet *sunSet) {
    _sunRise = sunRise;
    _sunSet = sunSet;
}

char *SunSimulation::regimenInit() {
    if (haveTimeError(_sunRise) || haveTimeError(_sunSet)) {
        return "time input error\0";
    }

    // get time (seconds) since sunrise start
    _endRise = _sunRise->getDuration() * 60;
    _startSet = getSecondsFromStart(_sunRise, _sunSet->getHour(), _sunSet->getMinute(), 0);

    if (_startSet < 60) {
        return "time input error, too small a gap between sunrise and sunset\0";
    }

    _endSet = _startSet + _sunSet->getDuration() * 60;

    if (_endSet > DAY_SECONDS || _endRise > _startSet) {
        return "time input error, intersection of sunrise and sunset regiments\0";
    }

    _optionsOk = true;
    _startPoint = _sunRise;

    return "successful initialization\0";
}

bool SunSimulation::haveTimeError(TimeSet *set) {
    return set->getHour() > 23 || set->getHour() < 0 || set->getMinute() > 59 ||
           set->getMinute() < 0 || set->getDuration() > 120 || set->getDuration() < 5;
}

uint32_t SunSimulation::
getSecondsFromStart(TimeSet *sunRise, uint32_t curHour, uint32_t curMinute, uint32_t curSecond) {
    // seconds
    uint32_t gap;

    if (sunRise->getHour() < curHour) {
        gap = ((curHour - sunRise->getHour()) * 60 + curMinute - sunRise->getMinute()) * 60
              + curSecond;
    } else if (sunRise->getHour() > curHour) {
        gap = ((24 - sunRise->getHour() + curHour) * 60 + curMinute - sunRise->getMinute())
              * 60 + curSecond;
    } else {
        if (sunRise->getMinute() <= curMinute) {
            gap = curMinute * 60 + curSecond - sunRise->getMinute() * 60;
        } else {
            gap = (24 * 60 - ((sunRise->getMinute() - curMinute))) * 60 - curSecond;
        }
    }

    return gap;
}

