//MSP430
#include <msp430.h>

int main(void) {

    //whatchdog
    WDTCTL = WDTPW | WDTHOLD;

    //Clock Config
    CALBC1_1MHZ==0xFF;
    DCOCTL = 0;             
    BCSCTL1 = CALBC1_1MHZ;  
    DCOCTL = CALDCO_1MHZ;   
    
    //interruption
    _BIS_SR(GIE);

    //I/O
    P1DIR |= BIT6; // PWM Out
    P1SEL |= BIT6; 

    //Timer A
    TACCR0  = 1000;      //period
    TACCTL1 = OUTMOD_7; 
    TACCR1  = 0;        // duty cycle
    TACTL   = TASSEL_2 + MC_1; 

    //Config Timer A
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE + MSC;

    int voltage = 0;
    int current = 0;
    int power   = 0;

    int lastPower   = 0
    int lastVoltage = 0;
    int lastCurrent = 0;


    int increase = 1;
    //Loop
    while(1){
        //ADC 1
        ADC10AE0  |= BIT4;                           
        ADC10CTL1  = INCH_4;
        ADC10CTL0 |= ENC + ADC10SC;  
        //Wait for the value           
        while(ADC10CTL1 & ADC10BUSY);
        //read and print the value
        ADC10CTL0 &= ~ENC;
        voltage     = ADC10MEM;


        //ADC 2
        ADC10AE0  |= BIT5;                           
        ADC10CTL1  = INCH_5;
        ADC10CTL0 |= ENC + ADC10SC;  
        //Wait for the value           
        while(ADC10CTL1 & ADC10BUSY);
        //read and print the value
        ADC10CTL0 &= ~ENC;
        current     = ADC10MEM;


        //MPPT
        //low current condition <<<<-------------------------
        if (current < 1){
            current = 1;
        }

        //Power
        power = voltage*current;

        if (power > lastPower){
            TACCR1 = TACCR1 + 1;
        }else{
            TACCR1 = TACCR1 - 1;
        }

        //update
        lastCurrent = current;
        lastPower = power;
        lastVoltage = voltage;


    }//end while

    return 0;

}//end main