#include "falling_item.h"
#include <Config.h>
#include <QTime>

Falling_item::Falling_item()
{
    srand(QTime::currentTime().msec());   //好像不太智能，到时候生成的时候再继续随机
    int type = rand() % 2;  //随机生成
    if(type)
        item_pix.load(item_blood_path);
    else
        item_pix.load(item_exp_path);
    item_rect = QRect(-50, -50, 50, 50);  //设置在界外
}
