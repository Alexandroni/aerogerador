//Codigo de controle do inversor monofásico, rastreando sensor de tensão AC

#include <msp430.h>

unsigned int FT=0;

int main(void) {

   //Stop whatchdog timer
   WDTCTL = WDTPW | WDTHOLD;

   //Clock configuration
   CALBC1_1MHZ==0xFF;
   DCOCTL = 0;            
   BCSCTL1 = CALBC1_1MHZ;  
   DCOCTL = CALDCO_1MHZ;  

   //interruption
   _BIS_SR(GIE);

   //Outputs
   P1DIR |= BIT6; // PWM Out
   P1SEL |= BIT6;
   P2DIR |= BIT1; // PWM Out
   P2SEL |= BIT1;

   //Timer A
   TACCR0  = 60;      //period
   TA1CCR0  = 60;      //period
   TACCTL1 = OUTMOD_7;
   TA1CCTL1 = OUTMOD_7;
   TACCR1  = 0;        // duty cycle
   TA1CCR1  = 0;        // duty cycle
   TACTL   = TASSEL_2 + MC_1;
   TA1CTL   = TASSEL_2 + MC_1;

   //ADC configuration
   ADC10CTL0 = SREF_0 + ADC10SHT_0 + ADC10ON;
   ADC10AE0  = BIT5;
   ADC10CTL1 = INCH_5 + ADC10DIV_0 + ADC10SSEL_3 + CONSEQ_0 + SHS_0;
   
while(1){

   //ADC
   ADC10CTL0 |= ENC + ADC10SC;
   while((ADC10CTL0 & ADC10IFG)==0);

   //PWM Adjust  
   if(ADC10MEM>840) ADC10MEM=840;

   if(ADC10MEM < 420)
   {
      FT = ((840 - ADC10MEM) - 420)/6;
      TACCR1 = 0;     // Read ADC Value
      TA1CCR1 = FT;     // Read ADC Value
    }
    else
    {
      FT = ((ADC10MEM) - 420)/6;
      TA1CCR1 = 0;
      TACCR1 = FT;
    }

}//end while

return 0;

}//end main
