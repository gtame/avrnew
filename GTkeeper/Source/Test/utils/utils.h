/*
 * utils.h
 *
 * Created: 24/03/2018 7:49:48
 *  Author: Gabi
 */ 

 #include <Time.h>
 
#ifndef UTILS_H_
#define UTILS_H_

time_t  GetTime(int hr,int min,int sec,int day, int month, int yr);
void FijarFecha(int year, uint8_t month ,uint8_t day);


#endif /* UTILS_H_ */