#include "hero.h"
#include <QDebug>

Hero::Hero()
{
    rect_ = QRect(256, 256, 100, 96);   //像素为单位
}

void Hero::scaled(int xt, int yt)
{
    hero_pix = hero_pix.scaled(hero_pix.width() / hero_pix.width() * xt, hero_pix.height() / hero_pix.height() * yt);
}

Hero1::Hero1(bool again, bool HpEnhance, bool AttackEnhance)
{
    //qDebug() << "1111";
    Speed = 15;
    Attack = 30;
    HPsum = HPcurrent = 1000;
    alive_again = again;
    HPsum += HpEnhance * 300;
    HPcurrent += HpEnhance * 50;
    Attack += AttackEnhance * 10;
    pixLoad(QPixmap(hero1_path));  //为初始状态
    scaled(100, 96);
}

Hero2::Hero2(bool again, bool HpEnhance, bool AttackEnhance)
{
    //qDebug() << "2222";
    Speed = 15;
    Attack = 35;
    HPsum = HPcurrent = 800;
    alive_again = again;
    HPsum += HpEnhance * 300;
    HPcurrent += HpEnhance * 50;
    Attack += AttackEnhance * 10;
    pixLoad(QPixmap(hero2_path));
    scaled(96, 96);
}
