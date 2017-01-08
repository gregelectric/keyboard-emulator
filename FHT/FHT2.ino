#include <Wire.h> 

// FHT defaults - refer to the Open Music Labs documentation at openmusiclabs.com
#define LOG_OUT 1         // use the log output function
#define FHT_N 256         // set to 256 point fht
#define OCTAVE 1
#define OCT_NORM 0
#define TOP_OCTAVE_DIVIDE true 
#include <FHT.h>

#define STATUS 13

int bias_threshold[8] = {152, 153, 128, 128, 128, 128, 128, 128};
int leds = 0;
int red; 
int green;
int blue;
int counter;

int r1 = 2;
int r2 = 3;
int r3 = 4;
int r4 = 5;
int r5 = 6;
int r6 = 7;
int r7 = 8;
int r8 = 9;

void setup() 
{ 
  pinMode     (r1, OUTPUT);
  pinMode     (r2, OUTPUT);
  pinMode     (r3, OUTPUT);
  pinMode     (r4, OUTPUT);
  pinMode     (r5, OUTPUT);
  pinMode     (r6, OUTPUT);
  pinMode     (r7, OUTPUT);
  pinMode     (r8, OUTPUT);

  pinMode     (STATUS, OUTPUT);

  // A/D setup for ATMEGA328P
  TIMSK0 = 0;     // turn off timer0 for lower jitter
  ADCSRA = 0xe5;  // set the adc to free running mode
  ADMUX = 0x40;   // use adc0
  DIDR0 = 0x01;   // turn off the digital input for adc0
}

void loop() 
{
  while(1) 
  { 
    if(counter >= 64)
    {
      digitalWrite(STATUS, HIGH);
      counter = 0;
    }
    else
    {
      digitalWrite(STATUS, LOW);
    }
    counter++;
    
    cli();                              // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FHT_N ; i++) 
    { // save 256 samples
      while(!(ADCSRA & 0x10));          // wait for adc to be ready
      ADCSRA = 0xf5;                    // restart adc
      byte m = ADCL;                    // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m;             // form into an int
      k -= 0x0200;                      // form into a signed int
      k <<= 6;                          // form into a 16b signed int
      fht_input[i] = k;                 // put real data into bins
    }
    fht_window();                       // window the data for better frequency response
    fht_reorder();                      // reorder the data before doing the fht
    fht_run();                          // process the data in the fht
    fht_mag_octave();                   // take the output of the fht
    sei();
    frequencyGraph(fht_oct_out[0], 
                   fht_oct_out[1], 
                   fht_oct_out[2], 
                   fht_oct_out[3], 
                   fht_oct_out[4], 
                   fht_oct_out[5], 
                   fht_oct_out[6], 
                   fht_oct_out[7]
                   );
  }
}

// Hanning weight
unsigned int bitCount (unsigned int value) 
{
    unsigned int count = 0;
    while (value > 0) 
    {           // until all bits are zero
        if ((value & 1) == 1)     // check lower bit
            count++;
        value >>= 1;              // shift bits, removing lower bit
    }
    return count;
}

void frequencyGraph(int x1, int x2, int x3, int x4, int x5, int x6, int x7, int x8) 
{

  if (x1 < 0) x1 = 0;
  if (x2 < 0) x2 = 0;
  if (x3 < 0) x3 = 0;
  if (x4 < 0) x4 = 0;
  if (x5 < 0) x5 = 0;
  if (x6 < 0) x6 = 0;
  if (x7 < 0) x7 = 0;
  if (x8 < 0) x8 = 0;
  
  if (x1 > bias_threshold[0]) 
  {
    digitalWrite(r1, HIGH);  
  } 
  else 
  {  
    digitalWrite(r1, LOW);   
  }
  
  if (x2 > bias_threshold[1]) 
  {
    digitalWrite(r2, HIGH);   
  } 
  else 
  {  
    digitalWrite(r2, LOW);   
  }
  
  if (x3 > bias_threshold[2]) 
  {
    digitalWrite(r3, HIGH);
  } 
  else 
  {  
    digitalWrite(r3, LOW);   
  }
  
  if (x4 > bias_threshold[3]) 
  {
    digitalWrite(r4, HIGH);
  } 
  else 
  {  
    digitalWrite(r4, LOW);   
  }
  
  if (x5 > bias_threshold[4]) 
  {
    digitalWrite(r5, HIGH);
  } 
  else 
  {  
    digitalWrite(r5, LOW);   
  }

  if (x6 > bias_threshold[5]) 
  {
    digitalWrite(r6, HIGH);
  } 
  else 
  {  
    digitalWrite(r6, LOW);   
  }
  
  if (x7 > bias_threshold[6]) 
  {
    digitalWrite(r7, HIGH); 
  } 
  else 
  {  
    digitalWrite(r7, LOW);   
  }
  
  if (x8 > bias_threshold[7]) 
  {
   
   digitalWrite(r8, HIGH);
  } 
  else 
  {  
   digitalWrite(r8, LOW);   
  }
}

