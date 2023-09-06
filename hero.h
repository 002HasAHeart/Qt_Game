#ifndef HERO_H
#define HERO_H

#include<Config.h>
#include<bullet.h>
#include<QPixmap>

class Hero
{
public:
    Hero();
    QRect getRect() { return rect_;}
    void setRect(QRect next) {rect_ = next;}
    void pixLoad(QPixmap pix_) {hero_pix = pix_;}
    void mirrored() {hero_pix = hero_pix.transformed(QTransform().scale(-1, 1));}
    void scaled(int xt, int yt);
    QPixmap getpix() {return hero_pix;}
    int getSpeed() {return Speed;}
    int getCurrHP() {return HPcurrent;}
    int getHP() {return HPsum;}
    void beAttacked(int attack) {HPcurrent -= attack;}
    int get_Attack() {return Attack;}

    //局内强化 游戏重新开始时消失， 全局强化考虑在构造函数时传入
    void recover(int num) {HPcurrent += num; if(HPcurrent >= HPsum) HPcurrent = HPsum;}
    void enhance_HP(int num) {HPsum += num;}
    void enhance_Speed(int num) {Speed += num;}
    void enhance_attack(int num) {Attack += num;}
    bool check_again() {return alive_again;}
    void set_again(bool t) {alive_again = t;}
    void set_HPsum(int sum) {HPsum = sum;}
    void set_HPcurrent(int cur) {HPcurrent = cur;}
    void set_Speed(int sp) {Speed = sp;}
    void set_Attack(int atk) {Attack = atk;}
    void consume() {alive_again = false;}
    void newlife() {HPcurrent = HPsum;}
    bool isInjure = false;
protected:
    QRect rect_;
    QPixmap hero_pix;
    int Speed;
    int HPsum;
    int HPcurrent;
    int Attack;
    bool alive_again = false;
};

class Hero1 :public Hero    //基础参数有所不同
{
public:
    Hero1(bool again, bool HpEnhance, bool AttackEnhance);
};

class Hero2 :public Hero
{
public:
    Hero2(bool again, bool HpEnhance, bool AttackEnhance);
};

#endif // HERO_H
