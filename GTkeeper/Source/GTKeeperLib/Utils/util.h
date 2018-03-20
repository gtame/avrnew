/*
 * util.h
 *
 * Created: 20/03/2018 9:26:48
 *  Author: gtame
 */ 

#include <inttypes.h>
#include <Arduino.h>
#include <Time.h>

#ifndef UTIL_H_
#define UTIL_H_

#define  TIME_WITHOUT_SECONDS(X) X - (X %  60) //Retorna un time_t pero sin segundos, si se le pasa 12:50:17 , devuelve  12:50:00

bool isValidNumber(char* str);

#endif /* UTIL_H_ */