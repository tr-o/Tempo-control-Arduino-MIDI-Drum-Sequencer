// Arduino MIDI Drum sequencer 
// Sending MIDI messages to the host PC.
// DAW: Waveform Free
//
// Author: Tara-chang
// 
// Revision History:
// Date        |    Author    |  Change
// ---------------------------------------------------
// 2021-05-09  |  Tara-chang  |  Initial Release
// 2021-05-09  |  Tara-chang  |  Added LCD and Trimpot to tempo control 
// 2021-05-16  |  Tara-chang  |  Added BUTTONS to change channel 
// 2021-05-19  |  Tara-chang  |  Sixteenth note available 

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <MIDI.h> 
MIDI_CREATE_DEFAULT_INSTANCE();

#include <avr/io.h>
#include <avr/interrupt.h>

#define CRUSH 49 //C#3
#define KICK 36 //C2
#define CLAP 39 //D#2
#define SNARE 38 //D2
#define RIDE 51 //D#3
#define HITOM 50 //D3
#define MIDTOM 48 //C3
#define LOWTOM 45 //A2
#define OPHIHAT 46 //A#2
#define CLHIHAT 42//F#2

byte DRUM[] = {SNARE, KICK, CLHIHAT, OPHIHAT, CRUSH, RIDE};
byte VELOCITY[] = {127, 100, 70, 70, 60, 60};
int BPMLIST[] = {175, 146, 85, 90, 150};
const byte NUM_DRUM = sizeof(DRUM)/sizeof(DRUM[0]);

byte CHANNELS[] = {0, 1, 2, 3, 4};
byte BUTPIN[] =   {4, 5, 6, 8, 7};
byte LEDPIN[] =   {9,11,12,13};

boolean RTM0[6][8][4] = {//the number of instruments, note, beat
 //1          2         3         4         5         6         7         8
  {{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0}},//SNARE
  {{1,0,0,0},{0,0,1,0},{1,0,0,0},{0,0,1,0},{0,0,1,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},//KICK
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CLHIHAT
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//OPHIHAT
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CRUSH
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}//RIDE
};

boolean RTM1[6][8][4] = {//the number of instruments, note, beat
 //1          2         3         4         5         6         7         8
  {{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0}},//SNARE
  {{1,0,1,0},{1,0,1,0},{1,0,1,0},{1,0,1,0},{1,0,1,0},{1,0,0,0},{1,1,1,1},{1,0,0,0}},//KICK
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CLHIHAT
  {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},//OPHIHAT
  {{1,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CRUSH
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}//RIDE
};


boolean RTM2[6][8][4] = {//the number of instruments, note, beat
 //1          2         3         4         5         6         7         8
  {{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0}},//SNARE
  {{1,0,0,0},{0,0,1,0},{1,0,0,0},{0,0,1,0},{0,0,1,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},//KICK
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CLHIHAT
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//OPHIHAT
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CRUSH
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}//RIDE
};

boolean RTM3[6][8][4] = {//the number of instruments, note, beat
 //1          2         3         4         5         6         7         8
  {{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0}},//SNARE
  {{1,0,0,0},{0,0,1,0},{1,0,0,0},{0,0,1,0},{0,0,1,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},//KICK
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CLHIHAT
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//OPHIHAT
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CRUSH
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}//RIDE
};

boolean RTM4[6][8][4] = {//the number of instruments, note, beat
 //1          2         3         4         5         6         7         8
  {{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0}},//SNARE
  {{1,0,0,0},{0,0,1,0},{1,0,0,0},{0,0,1,0},{0,0,1,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},//KICK
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CLHIHAT
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//OPHIHAT
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},//CRUSH
  {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}//RIDE
};

byte NUM_BEAT[] = {//beat = ♪数
  sizeof(RTM0[0][0])/sizeof(RTM0[0][0][0]),
  sizeof(RTM1[0][0])/sizeof(RTM1[0][0][0]),
  sizeof(RTM2[0][0])/sizeof(RTM2[0][0][0]),
  sizeof(RTM3[0][0])/sizeof(RTM3[0][0][0]),
  sizeof(RTM4[0][0])/sizeof(RTM4[0][0][0])};
  
byte NUM_NOTE[] = {//note = 拍数
  sizeof(RTM0[0])/sizeof(RTM0[0][0]),
  sizeof(RTM1[0])/sizeof(RTM1[0][0]),
  sizeof(RTM2[0])/sizeof(RTM2[0][0]),
  sizeof(RTM3[0])/sizeof(RTM3[0][0]),
  sizeof(RTM4[0])/sizeof(RTM4[0][0])};

byte nt = 0;
byte in = 0;
byte bt = 0;
byte i = 0;
byte BPM = 0; //X beats/60sec = 60/X sec/beat
byte interval;// = 60/double(BPM)*(4/double(NUM_BEAT))*1000; // 

boolean changed = 1;

const byte NUM_CHANNELS = sizeof(CHANNELS)/sizeof(CHANNELS[0]);
const byte NUM_BUTPIN = sizeof(BUTPIN)/sizeof(BUTPIN[0]);
const byte NUM_LEDPIN = sizeof(LEDPIN)/sizeof(LEDPIN[0]);

byte NEXT_CHANNEL = 1;
byte CHANNEL = 0;
byte BUTTON_INT0 = 2;
byte BUTTON_INT1 = 3;

byte firstline;
byte secondline;
byte nowchannel;

void setup()
{ 
  Serial.begin(115200);
  MIDI.begin(4); //

  for (i=0;i<NUM_BUTPIN;i++){   
    pinMode(BUTPIN[i], INPUT);
    digitalWrite(BUTPIN[i], HIGH);     
  }
  for (i=0;i<NUM_LEDPIN;i++){   
    pinMode(LEDPIN[i], OUTPUT);
    digitalWrite(LEDPIN[i], LOW);     
  }
  
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(BUTTON_INT0, INPUT);
  digitalWrite(BUTTON_INT0, HIGH);    // Enable pullup resistor
  pinMode(BUTTON_INT1, INPUT);
  digitalWrite(BUTTON_INT1, HIGH); 
  
  EICRA |= (1 << ISC01);    // Trigger on falling edge
  EIMSK |= (1 << INT0);     // Enable external interrupt INT0 = pin2
  EICRA |= (1 << ISC11);    // Trigger on falling edge
  EIMSK |= (1 << INT1);     // Enable external interrupt INT1 = pin3
  sei();                    // Enable global interrupts

}

ISR(INT0_vect)//pin2
{
  BPMLIST[CHANNEL]++;
  changed =1;      
}

ISR(INT1_vect)//pin3
{
  changed =1;
  BPMLIST[CHANNEL]--;      
}


void loop(){    
  for (bt = 0; bt < NUM_BEAT[CHANNEL] ; bt++){
    for (nt = 0; nt < NUM_NOTE[CHANNEL] ; nt++){
      digitalWrite(LEDPIN[nt/(NUM_NOTE[CHANNEL]/4)], HIGH);
      for (in = 0; in < NUM_DRUM; in++){    
             
        if (GetRTM(CHANNEL, in, nt, bt)){
        MIDI.sendNoteOn(DRUM[in],VELOCITY[in],1);  // (pitch, velocity, channel)         
        MIDI.sendNoteOff(DRUM[in],0,1);   //        
        }
      }
      NEXT_CHANNEL = Getchannel(CHANNEL);
      if((changed)){
        LCDisplay(BPMLIST[NEXT_CHANNEL],NEXT_CHANNEL);
        changed=0;
      }
      delay(interval);
      digitalWrite(LEDPIN[nt/(NUM_NOTE[CHANNEL]/4)], LOW);     
      }
  }

  CHANNEL = NEXT_CHANNEL;
  BPM = BPMLIST[NEXT_CHANNEL];
  interval = 60/double(BPM)*(4/double(NUM_BEAT[CHANNEL]))*1000;
            

}
byte Getchannel(byte oldchannel){
    for (i=0;i<NUM_BUTPIN;i++){
      if ((digitalRead(BUTPIN[i])==LOW)){
        nowchannel = CHANNELS[i];
        changed=1;
      }
      else{
      }    
    }
    return nowchannel;
}

boolean GetRTM(byte nowchannel, byte in, byte nt, byte bt){
    if(nowchannel==0){return RTM0[in][nt][bt];}
    else if(nowchannel==1){return RTM1[in][nt][bt];}
    else if(nowchannel==2){return RTM2[in][nt][bt];}
    else if(nowchannel==3){return RTM3[in][nt][bt];}
    else if(nowchannel==4){return RTM4[in][nt][bt];}
    else{return 0;}
}

void LCDisplay(byte firstline, byte secondline){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tempo:"+String(firstline));
    lcd.setCursor(0, 1);
    lcd.print("Ch:"+String(secondline));
}
