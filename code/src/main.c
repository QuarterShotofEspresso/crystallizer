/*	Author: Ratnodeep Bandyopadhyay
 */
#include <avr/io.h>
#include <util/delay.h>

// custom macros
#define true 1



// global variables
unsigned short gADCsample;
unsigned char gHundredsDisplay;
unsigned char gTensDisplay;
unsigned char gOnesDisplay;

unsigned char gConversionComplete = 0;
enum e_states{START, PROCESS, DISPLAY_ONES, DISPLAY_TENS, DISPLAY_HUNDREDS, ERROR} gState = ERROR;

// additional functions
unsigned char toHexData( unsigned char sum );
void stateMachine( void );
void computeData( void );
void tick( void );

int main( void ) {
    /* Insert DDR and PORT initializations */
    DDRC = 0xFF; PORTC = 0x00;// set to output
    DDRD = 0xFF; PORTD = 0x00;


    //disabling jtag   
/*    unsigned char jtagCounter = 0;

    while( jtagCounter < 10 ) {
        PORTC = ~PORTC;
        _delay_ms(1000);
        ++jtagCounter;
    }
    
    MCUCR |= (1 << JTD);
    MCUCR |= (1 << JTD);
    //jtag disabled

    jtagCounter = 0;
    while( jtagCounter < 10 ) {
        PORTC = ~PORTC;
        _delay_ms(1000);
        ++jtagCounter;
    }
*/
    // initialize the ADC
    ADMUX =  (1 << MUX2) | (1 << MUX1) | (1 << MUX0); // select PINA7's ADC
    // Enable ADC, Enable Auto Trigger in Free Running Mode, Enable Interrupt, Set prescaler factor to 128
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
    DIDR0 =  (1 << ADC7D); // disable digital input on PINA7
    ADCSRA = (1 << ADSC); // begin sampling

    // forever
    while( true ) {
        tick();
    }

    return 1;
}


void computeData( void ) {

    unsigned short milliamps = (( gADCsample >> 1 ) * 125 ) >> 7; // NOTE:: I realized something neat. So I had to delve a bit deeper: https://www.desmos.com/calculator/nephumyfrw
    unsigned char hundreds = milliamps / 100;
    unsigned char tens =  (( milliamps % 100 ) / 10 );
    unsigned char ones =  (( milliamps % 100 ) % 10 ) / 1;

    gHundredsDisplay = toHexData( hundreds );
    gTensDisplay = toHexData( tens );
    gOnesDisplay = toHexData( ones );

    return;
}


void tick( void ) {
   
    // transitions 
    switch( gState ) {

        case PROCESS:
            gState = DISPLAY_HUNDREDS;
            break;

        case DISPLAY_HUNDREDS:
            gState = DISPLAY_TENS;
            break;

        case DISPLAY_TENS:
            gState = DISPLAY_ONES;
            break;

        case DISPLAY_ONES:
            if( gConversionComplete ) {
                gConversionComplete = 0;
                gState = PROCESS;
            } else {
                gState = DISPLAY_HUNDREDS;
            }
            break;

        case START:
            gState = PROCESS;
            break;

        default:
            gState = ERROR;
            break;

    }


    // actions
    switch( gState ) {

        case PROCESS:
            computeData();
            break;

        case DISPLAY_HUNDREDS:
            PORTA = 0x00;
            PORTC = gHundredsDisplay;
            PORTA = 0x01;
            break;

        case DISPLAY_TENS:
            PORTA = 0x00;
            PORTC = gTensDisplay;
            PORTA = 0x02;
            break;

        case DISPLAY_ONES:
            PORTA = 0x00;
            PORTC = gOnesDisplay;
            PORTA = 0x04;
            break;

        case START:
            break;

        default:
            PORTA = 0x01;
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
    	default: sum = 0x40; break;

    }

    return sum;
}


ISR( ADC_vect ) { 
    gADCsample = ADC;
    gConversionComplete = 1;
}
