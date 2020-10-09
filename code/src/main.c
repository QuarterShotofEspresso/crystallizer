/*
 * Author: Ratnodeep Bandyopadhyay
 * 
 * All rights reserved.
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>

// custom defines
#define true         1
#define false        0
#define RC_CLOCK_CAL 0x7F
#define TOTAL_TASKS  2
#define TICK_GCF     10
#define TICK_SAMPLE_INTERVAL  500
#define TICK_DISPLAY_INTERVAL 10

// debugging defines
#define _ENABLE_AD_
#define _ENABLE_TICK_
#define _ENABLE_DELAY_

// global variables
unsigned short gADCsample = 0;
unsigned char gHundredsDisplay = 0x00;
unsigned char gTensDisplay = 0x00;
unsigned char gOnesDisplay = 0x00;
unsigned char gTicksPassed = 0;
unsigned short gTimeElapsed = 0;

unsigned char gConversionComplete = 0;
enum e_states{START, DISPLAY_ONES, DISPLAY_TENS, DISPLAY_HUNDREDS, ERROR} gState = START;

// additional functions
unsigned char toHexData( unsigned char sum );
void execute      ( void );
void tick_sample  ( void );
void tick_display ( void );
void initTasks    ( void );

struct task {
    void (*tick) (void);
    unsigned char tickInterval;
    unsigned char ticksPassed;
} gTasks[TOTAL_TASKS];


int main( void ) {

    // port initialization and data direction
    DDRA = 0x00; PORTA = 0x00;
    DDRB = 0x00; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0x07; PORTD = 0x00;

    // calibrate internal RC clock
    OSCCAL = RC_CLOCK_CAL;

  #ifdef _ENABLE_AD_
    // initialize the ADC
    PRR0  =   (0 << PRADC); // disable power reduction for adc
    ADMUX =   (1 << MUX2) | (1 << MUX1) | (1 << MUX0); // select PINA7's ADC
    DIDR0 =   (1 << ADC7D); // disable digital input on PINA7
    // Enable ADC, Enable Auto Trigger in Free Running Mode, Enable Interrupt, Set prescaler factor to 128
    ADCSRA |= (1 << ADSC) | (1 << ADEN);
  #endif


  #ifdef _ENABLE_DELAY_
    // setup timer0 
    // ensure countera0 is not doing anything
    TCCR0A = 0x00;
    // and timer0b with 1024 prescaler
    TCCR0B = (1 << CS00); //| (1 << CS00);
    // enable timrsk0 timer overflow interrupt
    TIMSK0 = (1 << TOIE0);
  #endif

    // set the global interrupt enable
    SREG |= (1 << 7);

    initTasks();

    while( true ) {
      #ifdef _ENABLE_TICK_
        executeTasks();
      #endif
      #ifdef _ENABLE_DELAY_
        while(gTimeElapsed < TICK_GCF);
        gTimeElapsed = 0;
      #endif
    }

    return 1;
}



/*
 *  initTasks:
 * initialize Tasks stored in gTasks
 *
 */
void initTasks( void ) {
    gTasks[0].tick = tick_sample;
    gTasks[0].tickInterval = TICK_SAMPLE_INTERVAL;
    gTasks[0].ticksPassed = 0;

    gTasks[1].tick = tick_display;
    gTasks[1].tickInterval = TICK_DISPLAY_INTERVAL;
    gTasks[1].ticksPassed = 0;

    return;
}


/*
 *  executeTasks
 * execute all tasks in gTasks
 *
 */
void executeTasks( void ) {

    for( unsigned char i = 0; i < TOTAL_TASKS; i++ ) {
        if( gTasks[i].ticksPassed == gTasks[i].tickInterval ) {
            gTasks[i].tick();
            gTasks[i].ticksPassed = 0;
        } else {
            gTasks[i].ticksPassed += TICK_GCF;
        }
    }

    return;
}



/*
 *  tick_sample
 * FSM controlling the sampling and evaluation of the ADC.
 *
 */
void tick_sample( void ) {

    // NOTE:: I realized something neat. So I had to delve a bit deeper: https://www.desmos.com/calculator/nephumyfrw
    
    gADCsample = ADC;
    unsigned short milliamps = (( gADCsample >> 1 ) * 125 ) >> 7;
    unsigned char hundreds = milliamps / 100;
    unsigned char tens =  (( milliamps % 100 ) / 10 );
    unsigned char ones =  (( milliamps % 100 ) % 10 ) / 1;

    gHundredsDisplay = toHexData( hundreds );
    gTensDisplay = toHexData( tens );
    gOnesDisplay = toHexData( ones );

    ADCSRA |= 1 << ADSC;

    return;
}


/*  tick_display:
 * FSM controlling the seven-segment display
 *
 */
void tick_display( void ) {
   
    // transitions
    switch( gState ) {

        case DISPLAY_HUNDREDS:
            gState = DISPLAY_TENS;
            break;

        case DISPLAY_TENS:
            gState = DISPLAY_ONES;
            break;

        case DISPLAY_ONES:
            gState = DISPLAY_HUNDREDS;
            break;

        case START:
            gState = DISPLAY_HUNDREDS;
            break;

        default:
            gState = ERROR;
            break;

    }


    // actions
    switch( gState ) {

        case DISPLAY_HUNDREDS:
            PORTD = 0x00;
            PORTC = gHundredsDisplay;
            PORTD = 0x01;
            break;

        case DISPLAY_TENS:
            PORTD = 0x00;
            PORTC = gTensDisplay;
            PORTD = 0x02;
            break;

        case DISPLAY_ONES:
            PORTD = 0x00;
            PORTC = gOnesDisplay;
            PORTD = 0x04;
            break;

        case START:
            break;

        default:
            PORTD = 0x01;
            PORTC = toHexData( 10 );
            break;

    }

    return;
}


/*  toHexData
 * Convert a numerical value *sum* to
 * to the 8-bit representation of  the *sum* on a seven-seg
 *
 */
unsigned char toHexData( unsigned char sum ) {

    switch( sum ) {
        case 0: sum = ~0x3F; break;
    	case 1: sum = ~0x06; break;
    	case 2: sum = ~0x5B; break;
    	case 3: sum = ~0x4F; break;
    	case 4: sum = ~0x66; break;
        case 5: sum = ~0x6D; break;
    	case 6: sum = ~0x7D; break;
    	case 7: sum = ~0x07; break;
    	case 8: sum = ~0x7F; break;
    	case 9: sum = ~0x67; break;
    	default: sum = ~0x80; break;
    }

    return sum;
}


// Interrupt Subroutine
//Wait till the timer interrupt is reached to delay for a particular duration
ISR( TIMER0_OVF_vect ) {
    gTimeElapsed++;
}

