#include <LiquidCrystal.h>

#define BEEPER 40
#define CONTROLLER_POT A10
#define CONTROLLER_B 18
#define FRAMES_PER_SECOND 25

LiquidCrystal lcd1(22,23,24,25,26,27);
LiquidCrystal lcd2(28,29,30,31,32,33);

unsigned long lastFrameMicros = 0; //stores micros() value of when last frame drawn

class Bullet {
  public:
    int bulletcounter = -1;
    int bulletHorCoarsePos = 0;
    int bulletHorFinePos = 0;
    int bulletVertCoarsePos = 0;
    int bulletVertFinePos = 0;
};

class BulletSprite{
  public:
    int horizontalPos = -1;
    int verticalPos = -1;
    byte sprite[8] = {
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000,
      B00000
    };
};

class SpaceInvaders{
  public:
    const byte playercharacter0[8] = {
    B00000,
    B00100,
    B00100,
    B01110,
    B11111,
    B10001,
    B00000,
    B00000
  };
  
  const byte playercharacter1[8] = {
    B00000,
    B00010,
    B00010,
    B00111,
    B01111,
    B01000,
    B00000,
    B00000
  };
  
  const byte playercharacter2[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B10000,
    B10000,
    B00000,
    B00000
  };
  
  const byte playercharacter3[8] = {
    B00000,
    B00001,
    B00001,
    B00011,
    B00111,
    B00100,
    B00000,
    B00000
  };
  
  const byte playercharacter4[8] = {
    B00000,
    B00000,
    B00000,
    B10000,
    B11000,
    B01000,
    B00000,
    B00000
  };
  
  const byte playercharacter5[8] = {
    B00000,
    B00000,
    B00000,
    B00001,
    B00011,
    B00010,
    B00000,
    B00000
  };
  
  const byte playercharacter6[8] = {
    B00000,
    B10000,
    B10000,
    B11000,
    B11100,
    B00100,
    B00000,
    B00000
  };
  
  const byte playercharacter7[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00001,
    B00001,
    B00000,
    B00000
  };
  
  const byte playercharacter8[8] = {
    B00000,
    B01000,
    B01000,
    B11100,
    B11110,
    B00010,
    B00000,
    B00000
  };

  volatile bool playerBulletFired = false;
  byte bulletWaitFrames = 0; //if value is 0, a new bullet can be fired; if value > 0, the value will decrement per frame drawn

  Bullet* bulletArray[6] = {new Bullet, new Bullet, new Bullet, new Bullet, new Bullet, new Bullet};


  // functions
  void drawPlayer(int playerpos){
    const int coarsepos = playerpos/5;
    const int finepos = playerpos%5;
    if(finepos == 0){
      lcd2.createChar(0, playercharacter0);
      lcd2.setCursor(coarsepos,1);
      lcd2.write(byte(0));
    }
    else if(finepos == 1){
      lcd2.createChar(0, playercharacter1);
      lcd2.createChar(1, playercharacter2);
      lcd2.setCursor(coarsepos,1);
      lcd2.write(byte(0));
      lcd2.setCursor(coarsepos+1,1);
      lcd2.write(byte(1));
    }
    else if(finepos == 2){
      lcd2.createChar(0, playercharacter3);
      lcd2.createChar(1, playercharacter4);
      lcd2.setCursor(coarsepos,1);
      lcd2.write(byte(0));
      lcd2.setCursor(coarsepos+1,1);
      lcd2.write(byte(1));
    }
    else if(finepos == 3){
      lcd2.createChar(0, playercharacter5);
      lcd2.createChar(1, playercharacter6);
      lcd2.setCursor(coarsepos,1);
      lcd2.write(byte(0));
      lcd2.setCursor(coarsepos+1,1);
      lcd2.write(byte(1));
    }
    else if(finepos == 4){
      lcd2.createChar(0, playercharacter7);
      lcd2.createChar(1, playercharacter8);
      lcd2.setCursor(coarsepos,1);
      lcd2.write(byte(0));
      lcd2.setCursor(coarsepos+1,1);
      lcd2.write(byte(1));
    }
  }
  
  void drawbullet(){
    BulletSprite* bsArray[6] = {new BulletSprite, new BulletSprite, new BulletSprite, new BulletSprite, new BulletSprite, new BulletSprite};
    //use lcd chars 2 to 7 (up to 6 bullets)
    for(Bullet* b : bulletArray) {
      
      if(!(b->bulletcounter < 0)){
    
        b->bulletVertCoarsePos = b->bulletcounter/8;
        b->bulletVertFinePos = b->bulletcounter%8;
    
        byte spriteHorByte = B00000;
        if(b->bulletHorFinePos == 0) spriteHorByte |= B00100;
        else if(b->bulletHorFinePos == 1) spriteHorByte |= B00010;
        else if(b->bulletHorFinePos == 2) spriteHorByte |= B00001;
        else if(b->bulletHorFinePos == 3) spriteHorByte |= B10000;
        else if(b->bulletHorFinePos == 4) spriteHorByte |= B01000;
        
        for(BulletSprite* bs : bsArray){
          if(((bs->horizontalPos == -1) || (bs->verticalPos == -1)) || ((bs->horizontalPos == b->bulletHorCoarsePos) && (bs->verticalPos == b->bulletVertCoarsePos))){
            bs->horizontalPos = b->bulletHorCoarsePos;
            bs->verticalPos = b->bulletVertCoarsePos;
            bs->sprite[b->bulletVertFinePos] |= spriteHorByte;
            break;
          }
        }
    
        b->bulletcounter--;
        
      }
    }  
      
    for(int i = 0; i < sizeof(bsArray)/sizeof(bsArray[0]); i++){
      BulletSprite* bs = bsArray[i];
      //lcd1.print(bs->verticalPos);
      if(bs->verticalPos == 6){
        lcd2.createChar(2+i, bs->sprite);
        lcd2.setCursor(bs->horizontalPos,0);
        lcd2.write(byte(2+i));
      }
      else if(bs->verticalPos == 1){
        lcd1.createChar(2+i, bs->sprite);
        lcd1.setCursor(bs->horizontalPos,1);
        lcd1.write(byte(2+i));
      }
      else if(bs->verticalPos == 0){
        lcd1.createChar(2+i, bs->sprite);
        lcd1.setCursor(bs->horizontalPos,0);
        lcd1.write(byte(2+i));
      }
    }
  
    for(BulletSprite* bs : bsArray) delete bs;
  }
  
  void firebullet(){
    playerBulletFired = true;
  }
  
  int bulletArrayAvailableSpace() {
    int i = 0;
    for (Bullet* b : bulletArray) {
      if(b->bulletcounter < 0) i++;
    }
    return i;
  }

  void callFrame() {
    //"blanking period"
    if((playerBulletFired) && ((bulletArrayAvailableSpace() > 0)) && (bulletWaitFrames == 0)){    
      Bullet* b;
      
      for(Bullet* tempB : bulletArray) {
        if(tempB->bulletcounter < 0) { b = tempB; break;}
      }
      
      b->bulletcounter = 55;
      int playerpos = 75-constrain(map(analogRead(CONTROLLER_POT),0,1023,0,75),0,75);
      b->bulletHorCoarsePos = playerpos/5;
      b->bulletHorFinePos = playerpos%5;
      if (b->bulletHorFinePos > 2) b->bulletHorCoarsePos++;
      playerBulletFired = false;
      bulletWaitFrames = 6; //wait 6 frames until next bullet can be drawn
      digitalWrite(BEEPER, HIGH);
    } else {
      playerBulletFired = false;
      if (bulletWaitFrames > 0) bulletWaitFrames--;
      digitalWrite(BEEPER, LOW);
    }
  
    //grapics
    lcd1.clear();
    lcd2.clear();  
  
    drawPlayer(75-constrain(map(analogRead(CONTROLLER_POT),0,1023,0,75),0,75));
  
    drawbullet();
  }
};

SpaceInvaders* game = new SpaceInvaders;

void setup() {
  // put your setup code here, to run once:

  pinMode(CONTROLLER_B, INPUT_PULLUP);
  pinMode(BEEPER, OUTPUT);

  lcd1.begin(16,2);
  lcd2.begin(16,2);

  attachInterrupt(digitalPinToInterrupt(CONTROLLER_B), buttonInterrupt, FALLING);

  //serial monitor debugging [uncomment to use]
  //Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  //if microseconds between drawing of last frame and drawing of current frame is >= 1mil / FPS, draw a frame
  if(micros() - lastFrameMicros >= 1000000 / FRAMES_PER_SECOND) {
    game->callFrame();
    lastFrameMicros = micros();
  }

}

void buttonInterrupt(){
  game->firebullet();
}
