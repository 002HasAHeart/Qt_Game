#ifndef FALLING_ITEM_H
#define FALLING_ITEM_H

#include <QPixmap>

class Falling_item
{
public:
    Falling_item();
    QPixmap getPix() {return item_pix;}
    QRect getRect() {return item_rect;}
    void pixLoad(QPixmap p) {item_pix = p;}
    void setRect(QRect next) {item_rect = next;}
    bool check() {return is_generated;}
    void change() {is_generated ^= 1;}
    int expUP() {return 25;}
    int bloodUP() {return 25;}
    int UPtype() {return type;}
    void setType(int t) {type = t;}
private:
    QPixmap item_pix;
    QRect item_rect;
    bool is_generated = false;
    int type = 0;
};

#endif // FALLING_ITEM_H
