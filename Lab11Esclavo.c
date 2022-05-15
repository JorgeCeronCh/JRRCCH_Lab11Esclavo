/*
 * File:   Lab11Esclavo.c
 * Author: jorge
 *
 * Created on 11 de mayo de 2022, 04:54 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>             // int8_t, unit8_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _XTAL_FREQ 1000000
#define FLAG_SPI 0xFF

uint8_t cont = 0;                   // Variable contador en PORTB
uint8_t val_temporal = 0;           // Variable temporal

void __interrupt() isr (void){
    if(INTCONbits.RBIF){        // Verificar si se activa la bandera de interrupcion de PORTB 
        if(!PORTBbits.RB0){     // Verificar si fue actividad por presionar RB0
            cont++;             // Incrementar en 1  
        }
        else if(!PORTBbits.RB1){// Verificar si se activa la bandera de interrupcion de PORTB 
            cont--;             // Decrementar en 1 
        }
        INTCONbits.RBIF = 0;    // Limpiar la bandera de interrupcion de PORTB
    }
    if(PIR1bits.SSPIF){                 // Interrupcion si el esclavo recibi� la informaci�n
        val_temporal = SSPBUF;          // Cargar el valor del maestro a variable temporAL
        //if (val_temporal != FLAG_SPI){  // Revisa si es un dato
            SSPBUF = cont;              // Cargar cont a enviar a Maestro
            PORTD = val_temporal;       // Mostrar el dato en PORTD
        //}
        PIR1bits.SSPIF = 0;             // Limpiar la bandera de SPI
    }
    return;
}


void setup(void){
    ANSEL = 0;                         
    ANSELH = 0;                 // I/O digitales
    
    TRISA = 0b00100000;         // RA5 como entradas
    TRISB = 0b00000011;         // RB0 y RB1 como entradas
    TRISC = 0b00011000;         // SD1 entrada, SCK y SD0 como salida
    TRISD = 0;                  // PORTD como salida
    PORTA = 0;                  // Se limpia PORTA
    PORTB = 0;                  // Se limpia PORTB
    PORTC = 0;                  // Se limpia PORTC
    // Configuraci�n del oscilador
    OSCCONbits.IRCF = 0b0100;   // 1 MHz
    OSCCONbits.SCS = 1;         // Oscilador interno
    // Configuraci�n de interrupciones
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitar interrupciones de perif�ricos
    INTCONbits.RBIE = 1;        // Habilitar interrupciones de PORTB
    INTCONbits.RBIF = 0;        // Limpiar la bandera de interrupcion de PORTB
    // Configuraci�n de PORTB
    IOCBbits.IOCB0 = 1;         // Habilitar interrupciones de cambio de estado en RB0
    IOCBbits.IOCB1 = 1;         // Habilitar interrupciones de cambio de estado en RB1
    OPTION_REGbits.nRBPU = 0;   // Habilitar Pull-ups
    WPUBbits.WPUB0 = 1;         // Pull-up en RB0
    WPUBbits.WPUB1 = 1;         // Pull-up en RB0

    // Configuraci�n de Esclavo
    // Configuraci�n de SPI
    SSPCONbits.SSPM = 0b0100;   // SPI Maestro, Reloj -> FOSC/4  (250 KBIS/s)
    SSPCONbits.CKP = 0;         // Reloj inactivo
    SSPCONbits.SSPEN = 1;       // Habilitar pines de SPI
    SSPSTATbits.CKE = 1;        // Dato enviado cada flanco de subida
    SSPSTATbits.SMP = 0;        // Dato al final del pulso de reloj

        
    PIR1bits.SSPIF = 0;         // Limpiar bandera de SPI
    PIE1bits.SSPIE = 1;         // Habilitar interrupcion de SPI
    INTCONbits.GIE = 1;         // Habilitar interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitar interrupciones de perif�ricos
}

void main(void) {
    setup();
    while(1){        
        // Envio y recepcion de datos en maestro
    }
    return;
}
