#include "bullet.h"

Bullet::Bullet(QRect start, int dir_)
{
    is_shot = false;
    is_bomb = false;
    Speed = 15;
    bullet_pix.load(ball_path);
    bullet_pix = bullet_pix.scaled(35, 35);
    bullet_rect = start;
    dir = dir_;
    if(!dir)
        bullet_pix = bullet_pix.transformed(QTransform().scale(-1, 1));
}

void Bullet::move()
{
    if(dir == 1) //左
        bullet_rect.moveTo(bullet_rect.x() - Speed, bullet_rect.y());
    else if(dir == 0) //右
        bullet_rect.moveTo(bullet_rect.x() + Speed, bullet_rect.y());
    else if(dir == 2) //上
        bullet_rect.moveTo(bullet_rect.x(), bullet_rect.y() - Speed);
    else   //下
        bullet_rect.moveTo(bullet_rect.x(), bullet_rect.y() + Speed);
}
