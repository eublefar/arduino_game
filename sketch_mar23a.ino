#include <DueTimer.h>
#include <SH1106_SPI.h>
#define null_entity (entity*)-1
#include <ISADefinitions.h>
SH1106_SPI_FB lcd;

uint16_t x_axis=0;
uint16_t y_axis=0;



class entity {
   public:
    entity() {}
    virtual void update() = 0;
    virtual void collide(entity &entity_arg) = 0;
};

class player:public entity {
  uint8_t x=60;
  uint8_t y=7;
  uint16_t sensitivity = 300;
  public:
  uint8_t bmp[9]={B11000000,
                  B11110000,
                  B11111000,
                  B11111100,
                  B11111111,
                  B11111100,
                  B11111000,
                  B11110000,
                  B11000000};
  void update() {
      lcd.writeBitmap(bmp,x,y,9,1);
      if((x_axis-500)<(-sensitivity)) {
        moveLeft();
      }else if((x_axis-500)>sensitivity) {
        moveRight();
      }

      if((y_axis-500)<-sensitivity) {
        moveDown();
      }else if((y_axis-500)>sensitivity) {
        moveUp();
      }
  }

  void moveRight() {
    if(x<120) {
      x++;
    }
  }

  void moveLeft() {
    if(x>0) {
      x--;
    }
  }
  void moveUp() {
    if(y>0) {
      --y;
    }
  }
  void moveDown() {
    if(y<7) {
      ++y;
    }
  }
  void shoot() {

  }

  virtual void collide(entity &entity_arg) {

  }

};

class enemy:public entity {
  uint8_t x,y;
  uint8_t lim_right, lim_left;
  bool reverse=false;
  public:
    enemy(uint8_t x_p, uint8_t y_p,uint8_t lim_r, uint8_t lim_l):x(x_p),y(y_p),lim_right(lim_r),lim_left(lim_l){}
    uint8_t bmp[9]={B00000011,
                    B00001111,
                    B00011111,
                    B00111111,
                    B11111111,
                    B00111111,
                    B00011111,
                    B00001111,
                    B00000011};
    void update() {
        if(!reverse) {
            if(x<(lim_right)) {
              ++x;
              lcd.writeBitmap(bmp,x,y,9,1);
            } else {
              reverse = !reverse;
            }
        }
        if(reverse) {
            if(x>(lim_left)) {
              --x;
              lcd.writeBitmap(bmp,x,y,9,1);
            } else {
              reverse = !reverse;
            }
        }
    }
    void shoot() {

    }

    virtual void collide(entity &entity_arg) {

    }
};


class shot:public entity {
  uint8_t x,y;
  uint8_t bmp_up[3]= {B11100000,
                    B11111111,
                    B11100000};
  uint8_t bmp_down[3]= {B00000111,
                      B11111111,
                      B00000111};
  bool dir;
  shot(bool dir, uint8_t x_p, uint8_t y_p):x(x_p),y(y_p)  {

  }

  void update() {
    if(dir) {
      lcd.writeBitmap(bmp_up,x,y,9,1);
    } else {
      lcd.writeBitmap(bmp_down,x,y,9,1);
    }
  }

  void collide(entity &entity_arg) {

  }
};


entity *entities[10] = {null_entity,null_entity,null_entity,null_entity,null_entity,null_entity,null_entity,null_entity,null_entity,null_entity};
uint8_t entity_num = 0;



void push_entity(entity* ent) {
  entities[entity_num]= ent;
  entity_num++;
}

void controllerRead() {

    x_axis = analogRead(JOY1X) ;
    y_axis = analogRead(JOY1Y);

//    Serial.println(x_axis);
//    Serial.println(y_axis);
}


//int last_measured=0;
void setup(){
  lcd.begin();
  lcd.print("test");
  lcd.renderAll();
  Serial.begin(9600);
  enemy *ship = new enemy(116, 2, 120, 0);
  enemy *ship1 = new enemy(4, 1, 120, 0);
  player * pl = new player();
  push_entity(ship);
  push_entity(ship1);
  push_entity(pl);


}

void loop(){

    lcd.clear();
    controllerRead();
    for(int i =0; i<entity_num;++i) {
      entities[i]->update();
    }
    lcd.renderAll();

}

