#ifndef I2C_H
#define I2C_H


void I2C_Master_Init(const unsigned long c);
void I2C_Wait();
void I2C_Start();
void I2C_Restart();
void I2C_Stop();
void I2C_Write(unsigned d);
unsigned short I2C_Read(unsigned short a);

#endif
