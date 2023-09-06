#ifndef MONSTER_H
#define MONSTER_H

#include <QPixmap>

class Monster   //其实可以写一个总的 Monster 与 Hero 的父类，但是...
{
public:
    Monster();
    virtual ~Monster() {}
    QPixmap get_pix() {return mon_pix;}
    QRect get_rect() {return mon_rect;}
    void setRect(QRect next) {mon_rect = next;}
    void pixLoad(QPixmap pix_) {mon_pix = pix_;}
    void mirrored() {mon_pix = mon_pix.transformed(QTransform().scale(-1, 1));}
    void scaled(int xt, int yt);
    int getHP() {return HPsum;}
    int getcurrHP() {return HPcurrent;}
    int getSpeed() {return Speed;}
    void beAttacked(int attack) {HPcurrent -= attack;}
    int get_Attack() {return Attack;}
    bool is_alive = false;
    bool isInjure = false;
protected:
    int HPsum;
    int HPcurrent;
    int Speed;
    int Attack;
    QPixmap mon_pix;
    QRect mon_rect;
};

class Monster1 : public Monster   //定向移动策略
{
public:
    Monster1();
    int getdir() {return dir;}
    void set_dir(int dir_) {dir = dir_;}
    int dir;
    void change() {is_bomb ^= 1;}
    bool check() {return is_bomb;}
private:
    bool is_bomb = false;
};

class Monster2 : public Monster   //随机移动策略
{
public:
    Monster2();
    void newlife() {HPcurrent = HPsum;}
    bool is_fell = false;
};

#endif // MONSTER_H
