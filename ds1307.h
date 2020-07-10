#ifndef DS1307_H
#define DS1307_H

#include "I2C.h"
#include "function.h"
#include "config.h"
#include "ds1307.c"

//     DS1307_ADDRES 0xD0

#define		RTCC_SEC_REG						0x00
#define		RTCC_MIN_REG						0x01
#define		RTCC_HOUR_REG						0x02
#define		RTCC_DAY_REG						0x03
#define		RTCC_DATE_REG						0x04
#define		RTCC_MONTH_REG						0x05
#define		RTCC_YEAR_REG						0x06


typedef struct {
    unsigned char day;
    unsigned char sec;
    unsigned char min;
    unsigned char hour;
    unsigned char date;
    unsigned char month;
    unsigned char year;
} RTCCTimeDate;


RTCCTimeDate rtc; // Instancia de Objeto RTC.




void setDS1307TimeDate(RTCCTimeDate* ptrToUserVal) {

    I2C_Start(); //Start condition
    I2C_Write(0xD0);
    I2C_Write(0x00);
    I2C_Write(DECtoBCD(ptrToUserVal-> sec));
    I2C_Write(DECtoBCD(ptrToUserVal-> min));
    I2C_Write(DECtoBCD(ptrToUserVal-> hour));
    I2C_Write(DECtoBCD(ptrToUserVal-> day));
    I2C_Write(DECtoBCD(ptrToUserVal-> date));
    I2C_Write(DECtoBCD(ptrToUserVal-> month));
    I2C_Write(DECtoBCD(ptrToUserVal-> year));
    I2C_Stop();
    //delay(20);
}

void getDS1307TimeDate(RTCCTimeDate* ptrToUserVal) {

    I2C_Start();
    I2C_Write(0xD0);
    I2C_Write(0x00);
    I2C_Restart();
    I2C_Write(0xD1);
    ptrToUserVal->sec = BCDtoDEC(I2C_Read(1));
    ptrToUserVal-> min = BCDtoDEC(I2C_Read(1));
    ptrToUserVal-> hour = BCDtoDEC(I2C_Read(1));
    ptrToUserVal-> day = BCDtoDEC(I2C_Read(1));
    ptrToUserVal-> date = BCDtoDEC(I2C_Read(1));
    ptrToUserVal-> month = BCDtoDEC(I2C_Read(1));
    ptrToUserVal-> year = BCDtoDEC(I2C_Read(0));
    I2C_Stop();
    //delay(20);

}

void getDS1307Time(RTCCTimeDate* ptrToUserVal) {

    I2C_Start();
    I2C_Write(0xD0);
    I2C_Write(0x00);
    I2C_Restart();
    I2C_Write(0xD1);
    ptrToUserVal-> sec = BCDtoDEC(I2C_Read(1));
    ptrToUserVal-> min = BCDtoDEC(I2C_Read(1));
    ptrToUserVal-> hour = BCDtoDEC(I2C_Read(0));
    I2C_Stop();
    //delay(20);

}

void setDS1307Time(RTCCTimeDate* ptrToUserVal) {

    I2C_Start(); //Start condition
    I2C_Write(0xD0);
    I2C_Write(0x00);
    I2C_Write(DECtoBCD(ptrToUserVal-> sec));
    I2C_Write(DECtoBCD(ptrToUserVal-> min));
    I2C_Write(DECtoBCD(ptrToUserVal->hour));
    I2C_Stop();
    //delay(20);
}

void RTC_Init(void) {
    I2C_Start();
    I2C_Write(0xD0);
    I2C_Write(0x07);
    I2C_Write(0x00);
    I2C_Stop();
}


validaRTC() {
    if (rtc.date > 31) rtc.date = 0;
    if (rtc.hour > 23) rtc.hour = 0;
    if (rtc.min > 59) rtc.min = 0;
    if (rtc.month > 12) rtc.month = 0;
    if (rtc.sec > 59) rtc.sec = 0;
    if (rtc.year > 99) rtc.year = 0;
}


#endif
