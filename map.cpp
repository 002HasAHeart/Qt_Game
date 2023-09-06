#include "map.h"

Map::Map()
{
    gamemap.load(gamemap_path);
    obs_pix.load(obs_path);
    obs_pix = obs_pix.scaled(obs_pix.width() / obs_pix.width() * 64, obs_pix.height() / obs_pix.height() * 64);
}
