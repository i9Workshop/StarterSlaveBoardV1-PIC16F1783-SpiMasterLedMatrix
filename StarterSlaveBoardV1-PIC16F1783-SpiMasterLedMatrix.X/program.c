#include "program.h"


// Delay x1.5us
void delay_x1o5us(uint8_t delay) {
    for(uint8_t i=0; i<delay; i++) NOP();
}

// Delay x24.25us
void delay_x24o25us(uint16_t delay) {
    for(uint16_t i=0; i<delay; i++) delay_x1o5us(15);
}

// Delay x1ms
void delay_ms(uint32_t delay) {
    for(uint32_t i=0; i<delay; i++) delay_x24o25us(41);
}


uint8_t counter = 0; // Variable for demo data transfer


void programInitialize(void) {
    TRISA = 0;
    ANSELA = 0;
    LATA = 0;
    
    // SCk pin
    TRISCbits.TRISC3 = 0;
    APFCONbits.SCKSEL = 0;
    
    // SDI pin
    TRISCbits.TRISC4 = 1;
    APFCONbits.SDISEL = 0;
    
    // SDO pin
    TRISCbits.TRISC5 = 0;
    APFCONbits.SDOSEL = 0;
    
    // SS pin
    TRISCbits.TRISC2 = 0;
    spi_SS1 = 1;
    
    spi_Initialize(_User_FOSC, 1000); // Set bit rate x8bit/baud = 8kHz
}

void programLoop(void) {
    uint8_t i = 0; // Variable for delay
    uint8_t dataReturn = 0; // Variable for data returned from slave
    bool transferDoneFlag = false; // Variable to check status data had been transfered
    
    for(i=0; i<50; i++) {
        if(!transferDoneFlag) { // Condition if data had not yet transfered
            dataReturn = spi_TransferByteSlave1(counter++); // Transfer counter data to slave device 1
            transferDoneFlag = true; // Set flag after data was transfered
        }
        
        ledMatrix_SetDisplay(dataReturn); // Display LEDs matrix from variable dataReturn
    }
}


void ledMatrix_AllOff(void) { // Turn off all LEDs
    led_VccColumn1 = 0;
    led_VccColumn2 = 0;
    led_VccColumn3 = 0;
    led_GndRow1 = 0;
    led_GndRow2 = 0;
    led_GndRow3 = 0;
}

void ledMatrix_SetDisplay(uint16_t ledWord) { // Set LED matrix using 9bit binary word
    uint8_t delay = 200;
    
    // First row
    
    ledMatrix_AllOff();
    
    led_VccColumn1 = (bool)(ledWord & 0b100000000);
    led_VccColumn2 = (bool)(ledWord & 0b010000000);
    led_VccColumn3 = (bool)(ledWord & 0b001000000);
    led_GndRow1 = 0;
    led_GndRow2 = 1;
    led_GndRow3 = 1;
    
    delay_x24o25us(delay);
    
    // Second row
    
    ledMatrix_AllOff();
    
    led_VccColumn1 = (bool)(ledWord & 0b000100000);
    led_VccColumn2 = (bool)(ledWord & 0b000010000);
    led_VccColumn3 = (bool)(ledWord & 0b000001000);
    led_GndRow1 = 1;
    led_GndRow2 = 0;
    led_GndRow3 = 1;
    
    delay_x24o25us(delay);
    
    // Third row
    
    ledMatrix_AllOff();
    
    led_VccColumn1 = (bool)(ledWord & 0b000000100);
    led_VccColumn2 = (bool)(ledWord & 0b000000010);
    led_VccColumn3 = (bool)(ledWord & 0b000000001);
    led_GndRow1 = 1;
    led_GndRow2 = 1;
    led_GndRow3 = 0;
    
    delay_x24o25us(delay);
}

void spi_Initialize(uint32_t fosc, uint16_t baudrate) {
    SSPSTATbits.SMP = 1; // Set input data sampled at end of data output time
    
    SSPCON1bits.SSPM = 10; // Set MSSP module for SPI master mode with clock = FOSC/(4*(SSPADD+1)) - Page 306
    
    SSPADD = (uint8_t)((fosc/baudrate) / 4 - 1); // Set baud rate generator register - Page 309
	
    SSPCON1bits.SSPEN = 1; // Set enable MSSP module - Page 306
    
    delay_x1o5us(2); // Wait for MSSP module configuration - Page 373 from I/O pin timing
                     // Tioz = 2us
}

uint8_t spi_TransferByteSlave1(uint8_t data) {
    // Single byte data transmit and receive - Page 262
    
    spi_SS1 = 0; // Select slave device 1
    
    uint8_t rxData = 0; // Variable to store received data
    
    NOP();
    
    SSPBUF = data; // Transmit data to slave - Page 266
    
    while(!PIR1bits.SSP1IF); // Wait transmission to slave complete - Page 262 and 266
    PIR1bits.SSP1IF = 0; // Clear MSSP module interrupt
    
    while(!SSPSTATbits.BF); // Wait data from slave - Page 305
    
    rxData = SSPBUF; // Read data received
    
    spi_SS1 = 1; // Unselect slave device 1
    
    return rxData; // Return received data
}
