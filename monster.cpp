#include "monster.h"

Monster::Monster()
{
    mon_rect = QRect(-1, -1, 1, 1); //先设置在界外
    Speed = 12;
}

void Monster::scaled(int xt, int yt)
{
    mon_pix = mon_pix.scaled(mon_pix.width() / mon_pix.width() * xt, mon_pix.height() / mon_pix.height() * yt);
}

Monster1::Monster1()
{
    //基础参数各有不同
    HPsum = HPcurrent = 0;
    Attack = 25;
}

Monster2::Monster2()
{
    HPsum = HPcurrent = 200;
    Attack = 20;
}
