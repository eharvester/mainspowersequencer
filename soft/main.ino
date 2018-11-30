#define RELAY_CALIBRATION 0

// DIP28 pin# <> Arduino
//  1 A6
//  2 D0 
//  3 D1
//  4 D2
//  5 D3 
//  6 D4
//  7 VCC
//  8 GND
//  9 X1
// 10 X2
// 11 D5
// 12 D6 
// 13 D7
// 14 D8
// 15 D9 
// 16 D10
// 17 D11
// 18 D12
// 19 D13
// 20 AVCC
// 21 AREF
// 22 GND
// 23 A0
// 24 A1
// 25 A2 
// 26 A3
// 27 A4
// 28 A5 


// PCB PIN names
#define LED1 5 // 11
#define LED2 6 // 12

#define P1 4 //6
#define P2 3 //5
#define P3 2 //4
#define P4 1 //3
#define P5 0 //2

#define SW A5 // 28

#define PWRDET A0 // 23

// software pin names

#define BASSAMP P5
#define MIDAMP P4
#define TWEETAMP P3
#define XOVER P2
#define PREAMP P1

#define LEFTLED LED2
#define RIGHTLED LED1

#define PWRBUTTON SW

#define ACSENSE PWRDET 


//relay compensations values in us to achieve zero crossing switching
#define BASSAMP_ON_DELAY 6500 
#define BASSAMP_OFF_DELAY 1600

#define MIDAMP_ON_DELAY 6500 
#define MIDAMP_OFF_DELAY 8200

#define TWEETAMP_ON_DELAY 6600 
#define TWEETAMP_OFF_DELAY 6400

#define XOVER_ON_DELAY 6500 
#define XOVER_OFF_DELAY 1900

#define PREAMP_ON_DELAY 6700 
#define PREAMP_OFF_DELAY 1900


// control macros
#define delus delayMicroseconds

#define BASSAMP_ON {AcSense(); delus(BASSAMP_ON_DELAY); digitalWrite(BASSAMP, 1);}   
#define BASSAMP_OFF {AcSense(); delus(BASSAMP_OFF_DELAY); digitalWrite(BASSAMP, 0);}  

#define MIDAMP_ON {AcSense(); delus(MIDAMP_ON_DELAY); digitalWrite(MIDAMP, 1);}    
#define MIDAMP_OFF {AcSense(); delus(MIDAMP_OFF_DELAY); digitalWrite(MIDAMP, 0);} 

#define TWEETAMP_ON {AcSense(); delus(TWEETAMP_ON_DELAY); digitalWrite(TWEETAMP, 1);}   
#define TWEETAMP_OFF {AcSense(); delus(TWEETAMP_OFF_DELAY); digitalWrite(TWEETAMP, 0);}  

#define XOVER_ON {AcSense(); delus(XOVER_ON_DELAY); digitalWrite(XOVER, 1);}   
#define XOVER_OFF {AcSense(); delus(XOVER_OFF_DELAY); digitalWrite(XOVER, 0);}   

#define PREAMP_ON {AcSense(); delus(PREAMP_ON_DELAY); digitalWrite(PREAMP, 1);} 
#define PREAMP_OFF {AcSense(); delus(PREAMP_OFF_DELAY); digitalWrite(PREAMP, 0);}  


#define LEFTLED_ON digitalWrite(LEFTLED, 1)
#define LEFTLED_OFF digitalWrite(LEFTLED, 0) 

#define RIGHTLED_ON digitalWrite(RIGHTLED, 1) 
#define RIGHTLED_OFF digitalWrite(RIGHTLED, 0) 

#define LEDS_ON {LEFTLED_ON; RIGHTLED_ON;}
#define LEDS_OFF {LEFTLED_OFF; RIGHTLED_OFF;}

unsigned int FlashLedCnt=0;

#define FLASHPERIODSLOW 100
#define FlashLedsSlow {if (FlashLedCnt>=FLASHPERIODSLOW) {FlashLedCnt=0;LEDS_ON;} else {FlashLedCnt++;LEDS_OFF;}}

#define FLASHPERIODALT 8
#define FLASHPERIODALT_H 4
#define FlashLedsAlt {if (FlashLedCnt>=FLASHPERIODALT) {FlashLedCnt=0;LEDS_OFF} else if (FlashLedCnt<FLASHPERIODALT_H) {FlashLedCnt++;LEFTLED_ON;RIGHTLED_OFF;} else {FlashLedCnt++;LEFTLED_OFF;RIGHTLED_ON;}}

// timing definitions
// tick 30 ms (relay cycle consumes max ~16 ms) 
#define TICKS_MS 30UL
// auto off after 8 h 
#define AUTOOFF_MS 28800000UL

// button sensing in 30ms ticks
// push >= 150ms
#define KEYSTABLEMIN 5
// hold >= 600ms
#define KEYSTABLELONGMIN 20 

// power sequence steps in 30ms
#define PWRUPTIME 33
#define PWRDOWNTIME 20


// waits on rising edge of AC power
void AcSense(void) {
 cli();
 while (digitalRead(ACSENSE));
 while (!digitalRead(ACSENSE));
 sei();
}


// FSMs
// control FSM
#define INIT 0
#define PWROFF 1
#define PREAMPON 2
#define XOVERON 3
#define TWEETON 4
#define MIDON 5 
#define BASSON 6
#define PREAMPOFF 7 
#define XOVEROFF 8
#define TWEETOFF 9
#define MIDOFF 10
#define BASSOFF 11
#define SPEAKER 12
#define HEADPHONES 13
byte PwrSqFsm = INIT;


// button FSM
#define UNKNOWN 0
#define OFF 1
#define ON 2
#define LONGON 3
byte ButtonFsm = UNKNOWN;


int Ticks = 0;

unsigned int keyopensample = 0;
unsigned int keyclosedsample = 0;

unsigned long ThenMs;
unsigned long PowerOnTimeMs;

void setup() 
{ 

 pinMode(ACSENSE,INPUT);

 digitalWrite(LED1, 0);   
 digitalWrite(LED2, 0);   
 digitalWrite(P1, 0);   
 digitalWrite(P2, 0);   
 digitalWrite(P3, 0);   
 digitalWrite(P4, 0);   
 digitalWrite(P5, 0);   

 
 pinMode(LED1, OUTPUT);   
 pinMode(LED2, OUTPUT);   
 pinMode(P1, OUTPUT);   
 pinMode(P2, OUTPUT);   
 pinMode(P3, OUTPUT);   
 pinMode(P4, OUTPUT);   
 pinMode(P5, OUTPUT);   
 
 ThenMs = millis();

} 


void loop() 
{ 


#if RELAY_CALIBRATION==1
 LEFTLED_ON; RIGHTLED_ON;
 PREAMP_ON; AcSense();AcSense();AcSense(); PREAMP_OFF; delay(100);

 XOVER_ON; AcSense();AcSense();AcSense(); XOVER_OFF;  delay(100);

 TWEETAMP_ON; AcSense();AcSense(); AcSense(); TWEETAMP_OFF; delay(100);

 MIDAMP_ON; AcSense();AcSense();AcSense(); MIDAMP_OFF; delay(100);
 
 BASSAMP_ON; AcSense();AcSense();AcSense(); BASSAMP_OFF; delay(100);
 LEFTLED_OFF; RIGHTLED_OFF;
 delay(1000);

#else
 byte sample; 
 unsigned long NowMs = millis();

// FSM
 switch (PwrSqFsm) {
  case INIT:
   LEDS_OFF;
   if (ButtonFsm==OFF) PwrSqFsm = PWROFF;
   break;

  case PWROFF:
   FlashLedsSlow;
   if (ButtonFsm==ON) {PREAMP_ON; PwrSqFsm = PREAMPON; PowerOnTimeMs = ThenMs;}
   break;

  case PREAMPON:
   FlashLedsAlt;
   if (Ticks == PWRUPTIME) {Ticks = 0; if (ButtonFsm==LONGON) PwrSqFsm = HEADPHONES; else {XOVER_ON; PwrSqFsm = XOVERON;}}
   else Ticks++;
   break;

  case HEADPHONES:
   LEFTLED_ON; RIGHTLED_OFF;
   if ((ButtonFsm==ON) || ((unsigned long)(ThenMs-PowerOnTimeMs) >= AUTOOFF_MS)) {PREAMP_OFF; PwrSqFsm = PREAMPOFF;}
   break;

  case XOVERON:
   FlashLedsAlt;
   if (Ticks == PWRUPTIME) {Ticks=0; TWEETAMP_ON; PwrSqFsm = TWEETON;}
   else Ticks++;
   break;

  case TWEETON:
   FlashLedsAlt;
   if (Ticks == PWRUPTIME) {Ticks=0; MIDAMP_ON; PwrSqFsm = MIDON;}
   else Ticks++;
   break;

  case MIDON:
   FlashLedsAlt;
   if (Ticks == PWRUPTIME) {Ticks=0; BASSAMP_ON; PwrSqFsm = BASSON;}
   else Ticks++;
   break;

  case BASSON:
   FlashLedsAlt;
    if (Ticks == PWRUPTIME) {Ticks=0; PwrSqFsm = SPEAKER;}
    else Ticks++;
   break;

  case SPEAKER:
   LEDS_ON;
   if ((ButtonFsm==ON) || ((unsigned long)(ThenMs-PowerOnTimeMs) >= AUTOOFF_MS)) {TWEETAMP_OFF; PwrSqFsm = TWEETOFF;} 
   break;

  case TWEETOFF:
   FlashLedsAlt;
   if (Ticks == PWRDOWNTIME) {Ticks=0; MIDAMP_OFF; PwrSqFsm = MIDOFF;}
   else Ticks++;
   break;
        
  case MIDOFF:
   FlashLedsAlt;
  if (Ticks == PWRDOWNTIME) {Ticks=0; BASSAMP_OFF; PwrSqFsm = BASSOFF;}
   else Ticks++;
   break;       

  case BASSOFF:
   FlashLedsAlt;
   if (Ticks == PWRDOWNTIME) {Ticks=0; XOVER_OFF; PwrSqFsm = XOVEROFF;}
   else Ticks++;
   break;  
  
  case XOVEROFF:
   FlashLedsAlt;
   if (Ticks == PWRDOWNTIME) {Ticks = 0; if (ButtonFsm==LONGON) {PwrSqFsm = HEADPHONES; PowerOnTimeMs = ThenMs;} else {PREAMP_OFF; PwrSqFsm = PREAMPOFF;}}
   else Ticks++;
   break;  

  case PREAMPOFF:
   FlashLedsAlt;
   if (Ticks == PWRDOWNTIME) {Ticks=0; PwrSqFsm = INIT;}
   else Ticks++;
   break;  

  default: 
   break;
 } // switch      


// Button FSM
 switch (ButtonFsm) {
  case UNKNOWN:
   if (keyopensample >= KEYSTABLEMIN) ButtonFsm = OFF;
  case OFF: 
   if (keyclosedsample >= KEYSTABLEMIN) ButtonFsm = ON;
  case ON:
   if (keyclosedsample >= KEYSTABLELONGMIN) ButtonFsm = LONGON;
  case LONGON:
   if (keyopensample >= KEYSTABLEMIN) ButtonFsm = OFF;
 }
// sense button
 sample = digitalRead(PWRBUTTON);
 keyopensample = sample?(keyopensample<KEYSTABLEMIN)?keyopensample+1:keyopensample:0; // low active key
 keyclosedsample = sample?0:(keyclosedsample<KEYSTABLELONGMIN)?keyclosedsample+1:keyclosedsample; // low active key

// wait on next TICKS_MS period
 do {NowMs = millis();} while ((unsigned long)(NowMs - ThenMs) < TICKS_MS);
 ThenMs = millis();

#endif
} 
