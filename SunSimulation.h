// SunSimulation
// Copyright Artem Botnev 2018
// MIT License

#ifndef SUNSIMULATION_H
#define SUNSIMULATION_H

#include "inttypes.h"

#define START_RISE 0
#define PWM_MAX 255
#define DAY_SECONDS 86400

class TimeSet {
public:
    TimeSet(uint8_t hour, uint8_t minute, uint16_t duration);

    uint8_t getHour();

    uint8_t getMinute();

    uint16_t getDuration();

private:
    uint8_t _hour;
    uint8_t _minute;
    uint16_t _duration;
};

class SunSimulation {
public:
    SunSimulation(TimeSet *sunRise, TimeSet *sunSet);
    
    SunSimulation(TimeSet *sunRise, TimeSet *sunSet, uint8_t *brightness);
    
    char *regimenInit();
    
    char *reloadRegimen(TimeSet *sunRise, TimeSet *sunSet);
    
    uint8_t changeBrightness(uint8_t hour, uint8_t minute, uint8_t second, uint8_t *brightness);
    
    uint8_t changeBrightness(uint8_t hour, uint8_t minute, uint8_t second);

private:
    TimeSet *_sunRise;
    TimeSet *_sunSet;
    TimeSet *_startPoint;
    // seconds
    uint32_t _endRise;
    uint32_t _startSet;
    uint32_t _endSet;
    // true if there is not initialization errors
    bool _optionsOk;
    uint8_t *_brightness;

    bool haveTimeError(TimeSet *set);
    
    uint32_t getSecondsFromStart(TimeSet *sunRise, uint32_t curHour, uint32_t curMinute, uint32_t curSecond);
};

#endif