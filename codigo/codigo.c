// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "pic16F877AConfig1.h"
#define DEBOUNCE_MS 20
#define _XTAL_FREQ 20000000

void set_percent(unsigned char percent){
    if(percent > 100) percent = 100;
    
    unsigned int max_counts = 4 * (PR2 + 1);
    unsigned long dc = ((unsigned long)percent * max_counts) / 100;

    CCPR1L = (unsigned char)(dc >> 2);
    CCP1CONbits.CCP1X = (dc >> 1) & 1;
    CCP1CONbits.CCP1Y = dc & 1;
}

void pwm_init(void){
    TRISCbits.TRISC2 = 0; // CCP1 PWM output
    CCP1CON = 0b00001100; // PWM mode

    PR2 = 249;       // 1 kHz con TMR2 prescaler 4
    T2CON = 0b00000101; // TMR2 ON, prescaler 4
    
    CCPR1L = 0;
}

void main(void){
    TRISB = 0xFF;
    TRISA = 0x00;

    pwm_init();
    unsigned char velocity = 0;

    while(1){
        if(PORTBbits.RB1){ //aumentar "velocidad"
            if(velocity < 100) velocity++;
            set_percent(velocity);
            __delay_ms(20);
        }
        if(PORTBbits.RB2){ //disminuir "velocidad"
            if(velocity > 0) velocity--;
            set_percent(velocity);
            __delay_ms(20);
        }
        
        if(PORTBbits.RB3){
            PORTA = 0x01;
        }
        
        if(PORTBbits.RB4){
            PORTA = 0x02;
        }
        
        if(PORTBbits.RB5){ //Stop
            PORTA = 0x00;
        }
    }
}

