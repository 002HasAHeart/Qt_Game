#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <Config.h>
#include <map.h>
#include <hero.h>
#include <bullet.h>
#include <monster.h>
#include <falling_item.h>
#include <QWidget>
#include <QTimer>
#include <QMovie>
#include <QLabel>
#include <QPushButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>

//QT_BEGIN_NAMESPACE
//namespace Ui { class MainScene; }
//QT_END_NAMESPACE

typedef struct Enhance_in_store{
    bool again;   // 复活一次
    bool HP;      //加50点血量
    bool Attack;   //加10点攻击力
}store_enhance;

// 存档信息结构体
struct GameSaveData {
    //三个商店相关
    bool again;
    bool e_HP;
    bool e_Attack;
    //三个经验相关
    int level;
    int exp;
    int level_up_need;
    //金币要加进来吗？
    int coin;
    //人物相关
    int cnt; //存活时间
    int option;
    int HPsum;
    int HPcurrent;
    int Speed;
    int Attack;
    int x;
    int y;
};

class Start : public QWidget  //登录界面
{
    Q_OBJECT
public:
    Start();
    void paintEvent(QPaintEvent *);
public slots:
    void start_to_option(); //点击开始后进入选择角色类型的界面
    void start_to_store();  //进入商店
    void startGame();   //进入游戏画面
private:
    bool optionMode = false;   //是否进入选择界面
    bool play_option;
    QPushButton start;
    QPushButton quitGame;
    QPushButton enterStore;
    QPushButton back; //商店界面的返回
    QPushButton option1;
    QPushButton option2;
    QPixmap start_pixmap;
    QPixmap hero_option1;
    QPixmap hero_option2;

    QPixmap cola;
    QPixmap pepsi;
    QPixmap fanta;
    QTimer update_time;
    QPushButton store1;
    QPushButton store2;
    QPushButton store3;
    bool in_store = false;   //在商店里面重绘
    //存档
    QPushButton load;
};

class MainScene : public QWidget
{
    Q_OBJECT
public:
    MainScene(QWidget *parent = nullptr, store_enhance se = {0, 0, 0}, bool option = false);
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *event);
    void detection_bullet_and_attack_and_item();
    bool detection_obs(QRect new_rect);
    bool boundary_detection(QRect new_rect);
    bool boundary_detection_for_fireman(QRect new_rect);
    void handleMovement(int key);
    bool hero_clla(QRect item);
    void shoot();
    void fireball_shoot();
    void check_mon_is_alive();
    void transfer_timeout();
    void generate();
    void bomb_fix_move();
    void fireman_random_move();
    int get_relative_pos(Monster2 *m);
    int get_distance(Monster2 *m);
    void check_win_and_lose();
    void falling_item_generate();
    void Time_froze();
    void Time_start();
    void ButtonHide();
    void level_update();
    void level_up_reward();
    void SaveGame(const GameSaveData& data);
    void loadGame();
    ~MainScene();
private:
    int count = 0;  //累计时间
    bool play_option;
    Map game_map;
    Hero *Hero_play;
    Bullet *m_bullet[Bullet_num];
    Bullet *m_ball[Bullet_num];
    Monster1 *bomb_m[bomb_mNum];
    Monster2 *fire_m[fire_mNum];
    Falling_item *f_item[itemNum];
    bool map_is_draw = false;
    bool hero_dir = false;  //false 为右, 一开始方向都为右
    QTimer update_time;    //局内画面更新
    QTimer *attack1_time;    //角色攻击时间
    QTimer *transfer_time;   //第一个角色
    QTimer *attack2_time;
    QTimer game_time;       //存活时间
    QTimer generate_time;   //怪物产生的时间
    QTimer ball_shoot_time;
    QVector<QRect> obs_positon;
    bool game_win;
    bool game_lose;
    //经验与等级
    int level = 1;
    int exp_current = 0;
    int levelUP_need = 50;    //升到下一等级需要的经验值，随等级生长而增加
    QPushButton Hp;
    QPushButton Speed;
    QPushButton Attack;
    QPushButton Save;
    //音乐相关
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
};

class end_widget : public QWidget
{
    Q_OBJECT
public:
    end_widget(int type);
    void paintEvent(QPaintEvent *);
private:
    QPixmap end_pix;
    QPushButton choose;
};

#endif // MAINSCENE_H
