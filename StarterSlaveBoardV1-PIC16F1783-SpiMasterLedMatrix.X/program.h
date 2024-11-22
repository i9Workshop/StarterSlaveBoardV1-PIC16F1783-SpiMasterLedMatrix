#ifndef program_H
#define	program_H

#include <xc.h>
#include <stdbool.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
    
#define _User_FOSC      32000000 // CPU Clock Frequency
    
#define led_VccColumn1      LATAbits.LATA0
#define led_VccColumn2      LATAbits.LATA1
#define led_VccColumn3      LATAbits.LATA2
#define led_GndRow1         LATAbits.LATA3
#define led_GndRow2         LATAbits.LATA4
#define led_GndRow3         LATAbits.LATA5
    
#define spi_SS1         LATCbits.LATC2 // SPI slave select pin

    
    void delay_x1o5us(uint8_t delay);
    void delay_x24o25us(uint16_t delay);
    void delay_ms(uint32_t delay);
    
    void programInitialize(void);
    void programLoop(void);
    
    void ledMatrix_AllOff(void);
    void ledMatrix_SetDisplay(uint16_t ledWord);
    
    void spi_Initialize(uint32_t fosc, uint16_t baudrate);
    uint8_t spi_TransferByteSlave1(uint8_t data);
    

#ifdef	__cplusplus
}
#endif

#endif	/* program_H */
