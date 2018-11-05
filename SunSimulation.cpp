// SunSimulation
// Copyright Artem Botnev 2018
// MIT License

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

SunSimulation::SunSimulation(TimeSet *sunRise, TimeSet *sunSet, uint8_t *brightness)
:   SunSimulation(sunRise, sunSet) {
   _brightness = brightness;
}

const char *SunSimulation::regimenInit() {
    if (haveTimeError(_sunRise) || haveTimeError(_sunSet)) {
        return "time input error";
    }

    // get time (seconds) since sunrise start
    _endRise = _sunRise->getDuration() * 60;
    _startSet = getSecondsFromStart(_sunRise, _sunSet->getHour(), _sunSet->getMinute(), 0);

    if (_startSet < 60) {
        return "time input error, too small a gap between sunrise and sunset";
    }

    _endSet = _startSet + _sunSet->getDuration() * 60;

    if (_endSet > DAY_SECONDS || _endRise > _startSet) {
        return "time input error, intersection of sunrise and sunset regiments";
    }

    _optionsOk = true;
    _startPoint = _sunRise;

    return "successful initialization";
}

const char *SunSimulation::reloadRegimen(TimeSet *sunRise, TimeSet *sunSet) {
    delete _sunRise;
    delete _sunSet;

    _sunRise = sunRise;
    _sunSet = sunSet;

    return regimenInit();
}

uint8_t SunSimulation::changeBrightness(uint8_t hour, uint8_t minute, uint8_t second, uint8_t *brightness) {
    if (!_optionsOk) {
        // errors of initialisation
        return 0;
    }

    uint32_t currentSecond = getSecondsFromStart(_startPoint, hour, minute, second);

    if (currentSecond > _endRise && currentSecond < _startSet) {
        *brightness = PWM_MAX;
    } else if (currentSecond > _endSet) {
        *brightness = 0;
    } else if (currentSecond <= _endRise) {
        *brightness = PWM_MAX * currentSecond / _endRise;
    } else if (currentSecond >= _startSet && currentSecond <= _endSet) {
        uint32_t curFromStart = currentSecond - _startSet;
        uint32_t setDuration = _endSet - _startSet;
        *brightness = PWM_MAX - (PWM_MAX * curFromStart / setDuration);
    }

    return *brightness;
}

uint8_t SunSimulation::changeBrightness(uint8_t hour, uint8_t minute, uint8_t second) {
    return changeBrightness(hour, minute, second, _brightness);
}

bool SunSimulation::haveTimeError(TimeSet *set) {
    return set->getHour() > 23 || set->getMinute() > 59 || set->getDuration() > 120 || set->getDuration() < 5;
}

uint32_t SunSimulation::getSecondsFromStart(TimeSet *sunRise, uint32_t curHour, uint32_t curMinute, uint32_t curSecond) {
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

