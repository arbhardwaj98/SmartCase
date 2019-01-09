#include "stdafx.h"
#include <stdio.h>
#include <intrinsics.h>
#include <iostm8s103f3.h>
#include <stm8s_exti.h>
#include <stdafx.h>
#include <LIS3DH.h>

void init_acc(){
  
  uint8_t dataToWrite = 0; //Start Fresh!
  
  //dataToWrite |= 0x4 << 4; //ODR of 50Hz
  dataToWrite |= 0x5 << 4; //ODR of 100Hz
  dataToWrite |= 0x7; //Enable all axes
  SPI_write(LIS3DH_CTRL_REG1, dataToWrite);
  if(SPI_read(LIS3DH_CTRL_REG1) !=  dataToWrite)
    printf("ERROR1");
  
  dataToWrite = 0x80; // Range of 2g + other parameters
  SPI_write(LIS3DH_CTRL_REG4, dataToWrite);
  if(SPI_read(LIS3DH_CTRL_REG4) !=  dataToWrite)
    printf("ERROR2");
  
  dataToWrite = 0x80; //ADC enable
  SPI_write(LIS3DH_TEMP_CFG_REG, dataToWrite);
  if(SPI_read(LIS3DH_TEMP_CFG_REG) !=  dataToWrite)
    printf("ERROR3");
  
}

void config_ff(){
  SPI_write(0x0F, 0x33);
  SPI_write(0x1F, 0x00);
  SPI_write(0x20, 0x5F);
  SPI_write(0x21, 0x00);
  SPI_write(0x22, 0x40);
  SPI_write(0x23, 0x80);
  SPI_write(0x24, 0x08);
  SPI_write(0x25, 0x00);
  SPI_write(0x26, 0x00);
  SPI_write(0x2E, 0x00);
  SPI_write(0x30, 0x95);
  SPI_write(0x32, 100);
  SPI_write(0x33, 0x01);
  SPI_write(0x34, 0x00);
  SPI_write(0x36, 0x00);
  SPI_write(0x37, 0x00);
  SPI_write(0x38, 0x00);
  SPI_write(0x3A, 0x00);
  SPI_write(0x3B, 0x00);
  SPI_write(0x3C, 0x00);
  SPI_write(0x3D, 0x00);
  SPI_write(0x3E, 0x00);
  SPI_write(0x3F, 0x00);
  
//  SPI_write(0x20, 57);
//  SPI_write(0x21, 0);
//  SPI_write(0x22, 40);
//  SPI_write(0x23, 0);
//  SPI_write(0x24, 8);
//  SPI_write(0x32, 16);
//  SPI_write(0x33, 3);
//  SPI_write(0x30, 95);
  
//  SPI_write(0x20, 0x57);
//  SPI_write(0x21, 0x00);
//  SPI_write(0x22, 0x40);
//  SPI_write(0x23, 0x00);
//  SPI_write(0x24, 0x08);
//  SPI_write(0x32, 0x16);
//  SPI_write(0x33, 0x03);
//  SPI_write(0x30, 0x95);
  
}


void read_acc(){
  
  // read in the 3 axis data, each one is 14 bits
  // print the data to terminal
  uint8_t result[6];
  result[0] = SPI_read(0x28);  
  result[1] = SPI_read(0x29);  
  result[2] = SPI_read(0x2A);  
  result[3] = SPI_read(0x2B);  
  result[4] = SPI_read(0x2C);  
  result[5] = SPI_read(0x2D); 
  
  int x = (int16_t)result[0] | (int16_t)(result[1] << 8) ;
  float x1 = x / 4096.0/4;
  
  int y = (int16_t)result[2] | (int16_t)(result[3] << 8);
  float y1 = y / 4096.0/4;
  
  int z = (int16_t)result[4] | (int16_t)(result[5] << 8);
  float z1 = z / 4096.0/4;
  
//  if(x1 < 0.2 && y1 < 0.2 && z1 < 0.2)
//  {
//    if(x1 > -0.2 && y1 > -0.2 && z1 > -0.2)
  printf("x : %.2f      y : %.2f        z : %.2f\n", x1, y1, z1);
// }
}
  
void tap2()
{
    SPI_write(48, 0);
    SPI_write(50, 0);
    SPI_write(51, 0);
    SPI_write(56, 21);
    SPI_write(57, 0);
    SPI_write(58, 80);
    SPI_write(59, 8);
    SPI_write(60, 8);
    SPI_write(61, 16);
    SPI_write(38, 8);
    SPI_write(34, 128);
    SPI_write(37, 0);
    SPI_write(31, 0);
    SPI_write(32, 71);
    SPI_write(35, 136);
    SPI_write(46, 0);
    SPI_write(36, 8);
}

void freefall()
{
    uint8_t dataToWrite = 0;
    
    SPI_write(LIS3DH_CTRL_REG1, 0x77);          // ODR = 400 Hz  // HP mode //ODR3 ODR2 ODR1 ODR0 LPen Zen Yen Xen
    SPI_write(LIS3DH_CTRL_REG2, 0);             // High-pass filter disabled
    
    
    //LIS3DH_CTRL_REG3
    //Choose source for pin 1
    dataToWrite = 0;
    //dataToWrite |= 0x80; //Click detect on pin 1
    dataToWrite |= 0x40; //AOI1 event (Generator 1 interrupt on pin 1)
    //dataToWrite |= 0x20; //AOI2 event ()
    //dataToWrite |= 0x10; //Data ready
    //dataToWrite |= 0x04; //FIFO watermark
    //dataToWrite |= 0x02; //FIFO overrun
    SPI_write(LIS3DH_CTRL_REG3,  0x40);
    
    
    SPI_write(LIS3DH_CTRL_REG4, 0); // FS = ±2 g
    
    
    //LIS3DH_CTRL_REG5
    //Int1 latch interrupt and 4D on  int1 (preserve fifo en)
    dataToWrite = 0;
    dataToWrite |= 0x08; //Latch interrupt (Cleared by reading int1_src)
    //dataToWrite |= 0x04; //Pipe 4D detection from 6D recognition to int1?
    SPI_write(LIS3DH_CTRL_REG5,  0x08);

    
    //  //LIS3DH_CTRL_REG6
    //  //Choose source for pin 2 and both pin output inversion state
    //  dataToWrite = 0;
    //  dataToWrite |= 0x80; //Click int on pin 2
    //  //dataToWrite |= 0x40; //Generator 1 interrupt on pin 2
    //  //dataToWrite |= 0x10; //boot status on pin 2
    //  //dataToWrite |= 0x02; //invert both outputs
    //  SPI_write(LIS3DH_CTRL_REG6, dataToWrite);
    
    //SPI_write(LIS3DH_REFERENCE, 8);
    //SPI_write(LIS3DH_FIFO_CTRL_REG, 0);
    
    
    //LIS3DH_INT1_CFG
    dataToWrite = 0;
    dataToWrite |= 0x80;//AOI, 0 = OR 1 = AND
    //dataToWrite |= 0x40;//6D, 0 = interrupt source, 1 = 6 direction source
    //Set these to enable individual axes of generation source (or direction)
    // -- high and low are used generically
    //dataToWrite |= 0x20;//Z high
    dataToWrite |= 0x10;//Z low
    //dataToWrite |= 0x08;//Y high
    dataToWrite |= 0x04;//Y low
    //dataToWrite |= 0x02;//X high
    dataToWrite |= 0x01;//X low
    SPI_write(LIS3DH_INT1_CFG, 0x95);
    
    
    //LIS3DH_INT1_THS
    dataToWrite = 0;
    //Provide 7 bit value, 0x7F always equals max range by accelRange setting
    dataToWrite |= 0x0C; // 1/8 range
    SPI_write(LIS3DH_INT1_THS, 0x0C);
    
    
    //LIS3DH_INT1_DURATION
    dataToWrite = 0;
    //minimum duration of the interrupt
    //LSB equals 1/(sample rate)
    dataToWrite |= 0x08; // 36 * 1/400 s = 90ms
    SPI_write(LIS3DH_INT1_DURATION, 0x24);
}

void freefall_electicimp(){
  
  uint8_t dataToWrite = 0;
  
  SPI_writeBit(LIS3DH_CTRL_REG3, 6, 1);
  
  int _range = 2; 
    
  uint8_t threshold = (((0.5 * 1.0) / (_range)) * 127);
  dataToWrite = 0;
  dataToWrite = (threshold & 0x7f);
  SPI_write(LIS3DH_INT1_THS, dataToWrite);
  
  dataToWrite = 0;
  dataToWrite = (5 & 0x7f);
  SPI_write(LIS3DH_INT1_DURATION, dataToWrite);
  
  uint8_t options = LIS3DH_AOI | LIS3DH_X_LOW | LIS3DH_Y_LOW | LIS3DH_Z_LOW;
  SPI_write(LIS3DH_INT1_CFG, options);
}

void SPI_writeBit(int reg, int bit, int state) {
        uint8_t val = SPI_read(reg);
        if (state == 0) {
            val = val & ~(0x01 << bit);
        } else {
            val = val | (0x01 << bit);
        }
        SPI_write(reg, val);
    }