/*	Author: Ratnodeep Bandyopadhyay
 */
#include <avr/io.h>
#include <util/delay.h>

// custom macros
#define true 1
#define TUNE_FACTOR 0xFF
#define TOTAL_TASKS 2
#define TICK_GCF 1
#define TICK_SAMPLE_INTERVAL 10
#define TICK_DISPLAY_INTERVAL 1

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

unsigned char gConversionComplete = 0;
enum e_states{START, DISPLAY_ONES, DISPLAY_TENS, DISPLAY_HUNDREDS, ERROR} gState = START;

// additional functions
unsigned char toHexData( unsigned char sum );
void execute      ( void );
//void computeData  ( void );
void tick_sample  ( void );
void tick_display ( void );
void initTasks    ( void );


struct task {
    void (*tick) (void);
    unsigned char tickInterval;
    unsigned char ticksPassed;
} gTasks[TOTAL_TASKS];


int main( void ) {

    DDRA = 0x00; PORTA = 0x00;
    DDRB = 0x00; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0x07; PORTD = 0x00;

    // calibrate internal RC clock
    OSCCAL = TUNE_FACTOR;

    // setting the clock prescaler
    //CLKPR = 0x80;
    //CLKPR = 0x80;


    #ifdef _ENABLE_AD_
      // initialize the ADC
      PRR0 =    (0 << PRADC); // set PRR0 to 0
      ADMUX =   (1 << MUX2) | (1 << MUX1) | (1 << MUX0); // select PINA7's ADC
      DIDR0 =   (1 << ADC7D); // disable digital input on PINA7
      // Enable ADC, Enable Auto Trigger in Free Running Mode, Enable Interrupt, Set prescaler factor to 128
      ADCSRA |= (1 << ADSC) | (1 << ADEN); //| (1 << ADATE);
    #endif

    initTasks();

    while( true ) {
        #ifdef _ENABLE_TICK_
          execute();
        #endif
        #ifdef _ENABLE_DELAY_
          _delay_ms( TICK_GCF );
        #endif
    }

    return 1;
}


void initTasks( void ) {
    gTasks[0].tick = tick_sample;
    gTasks[0].tickInterval = TICK_SAMPLE_INTERVAL;
    gTasks[0].ticksPassed = 0;

    gTasks[1].tick = tick_display;
    gTasks[1].tickInterval = TICK_DISPLAY_INTERVAL;
    gTasks[1].ticksPassed = 0;

    return;
}

void execute( void ) {

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

/*
ISR( ADC_vect ) { 
    gADCsample = ADC;
    gConversionComplete = 1;
}*/
