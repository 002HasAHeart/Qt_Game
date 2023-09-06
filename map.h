#ifndef MAP_H
#define MAP_H

#include<Config.h>
#include<QPixmap>
class Map
{
public:
    Map();
    QPixmap get_map() {return gamemap;}
    QPixmap get_obs() {return obs_pix;}
private:
    QPixmap gamemap;
    QPixmap obs_pix;
};

#endif // MAP_H
