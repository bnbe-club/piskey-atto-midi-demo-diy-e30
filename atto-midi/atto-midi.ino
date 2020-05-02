/*************************************************************************************************************************************************
 *  TITLE: Controlling Ableton Live With Atto
 *
 *  By Frenoy Osburn
 *  YouTube Video: https://youtu.be/zZXCA7Eh0sk
 *  BnBe Post: https://www.bitsnblobs.com/controlling-ableton-live-with-atto
 *************************************************************************************************************************************************/
#define  midichannel 1;                              // MIDI channel from 0 to 15 (+1 in "real world")

int pinRead;
byte PadNote[4] = { // MIDI notes from 0 to 127 (Mid C = 60)
  36, 38, 40, 42
};         
        
int MaxPlayTime[4] = { // Cycles before a 2nd hit is allowed
  90, 90, 90, 90
};               

boolean activePad[4] = {
  0, 0, 0, 0
};                   // Array of flags of pad currently playing
int PinPlayTime[4] = {
  0, 0, 0 , 0
};                     // Counter since pad started to play
byte status1;
int pin = 0;
int hitavg = 0;

void setup()
{
  Serial.begin(115200);  // connect to the serial port 115200
  pinMode(17, INPUT);
  digitalWrite(17, HIGH);
  pinMode(15, INPUT);
  digitalWrite(15, HIGH);
  pinMode(16, INPUT);
  digitalWrite(16, HIGH);
  pinMode(14, INPUT);
  digitalWrite(14, HIGH);
}

void loop()
{
    for(int pin=0; pin <= 3; pin++)                       
    {
      boolean on  = false;
      if(pin == 0)
         on = digitalRead(17);
      else if(pin == 1)
         on = digitalRead(15);
      else if(pin == 2)
         on = digitalRead(16);
      else if(pin == 3)
         on = digitalRead(14);
  
      if(!on)
      {
        if((activePad[pin] == false))
        {
          hitavg = 127;
          MIDI_TX(144,PadNote[pin],hitavg); //note on
          PinPlayTime[pin] = 0;
          activePad[pin] = true;
        }
        else
        {
          PinPlayTime[pin] = PinPlayTime[pin] + 1;
        }
      }
      else if((activePad[pin] == true))
      {
        PinPlayTime[pin] = PinPlayTime[pin] + 1;
        if(PinPlayTime[pin] > MaxPlayTime[pin])
        {
          activePad[pin] = false;
          MIDI_TX(128,PadNote[pin],0);
        }
      }
    }
}

//*******************************************************************************************************************
// Transmit MIDI Message
//*******************************************************************************************************************
void MIDI_TX(byte MESSAGE, byte PITCH, byte VELOCITY)
{
    status1 = MESSAGE + midichannel;
    Serial.write(status1);
    Serial.write(PITCH);
    Serial.write(VELOCITY);
}
