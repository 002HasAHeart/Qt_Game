#ifndef BULLET_H
#define BULLET_H

#include <Config.h>
#include <QPixmap>
class Bullet
{
public:
    bool is_shot;
    bool is_bomb;
    Bullet(QRect start, int dir_);
    void move();
    void pixload(QString path) {bullet_pix = QPixmap(path);}
    QPixmap get_pix() {return bullet_pix;}
    QRect get_rect() {return bullet_rect;}
    void setRect(QRect next) {bullet_rect = next;}
    void set_dir(int dir_){dir = dir_;}
    void mirrored() {bullet_pix = bullet_pix.transformed(QTransform().scale(-1, 1));}
private:
    int Speed;
    int dir;
    QPixmap bullet_pix;
    QRect bullet_rect;
};

#endif // BULLET_H
