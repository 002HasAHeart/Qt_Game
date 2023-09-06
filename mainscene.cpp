#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include <QLabel>
#include <QtGui>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>

int coin = 500;
store_enhance init_enhance{0,0,0};  //商店传入的结构体
QRect obs[22] = {            //障碍物的位置(碰撞检测)
    QRect(0, 0, 64, 64),
    QRect(1, 0, 64, 64),
    QRect(0, 1, 64, 64),
    QRect(38, 0, 64, 64),
    QRect(39, 0, 64, 64),
    QRect(39, 1, 64, 64),
    QRect(0, 23, 64, 64),
    QRect(0, 24, 64, 64),
    QRect(1, 24, 64, 64),
    QRect(38, 24, 64, 64),
    QRect(39, 23, 64, 64),
    QRect(39, 24, 64, 64)
};

QList<int> movements;   //控制wasd移动
//对于hero_option1 近战
bool is_attacking = false;
QVector <QPixmap> pix_node;
int index = 0;

Start::Start()
{
    update_time.start(3);
    connect(&update_time, &QTimer::timeout, [=](){
        this->repaint();
        if(in_store)
            setWindowTitle(QString("Pepsi(300金币)  Cola(200金币)  Fanta(200金币)  您现在拥有金币: %1").arg(coin));
    });
    cola.load(":/picture/cola.gif");
    pepsi.load(":/picture/Pepsi.gif");
    fanta.load(":/picture/Orange.gif");
    start_pixmap.load(startMap_path);
    hero_option1.load(hero1_path);
    hero_option2.load(hero2_path);
    update();
    this->setFixedSize(1200, 800);
    this->setWindowTitle("开始新的游戏，或者读取上一个存档");

    start.setParent(this);
    enterStore.setParent(this);
    quitGame.setParent(this);
    start.move(this->width() * 1 / 8, this->height() * 2 / 3);
    quitGame.move(this->width() * 2 / 3 + 50, this->height() * 2 / 3);
    enterStore.move(this->width() * 1 / 8 + 350, this->height() * 2 / 3);
    start.resize(200, 100);
    enterStore.resize(200, 100);
    quitGame.resize(200, 100);

    //以下是商店界面的布局
    back.setParent(this);
    back.setText("返回");
    back.resize(100, 100);
    back.setFont(QFont("Times New Roman", 12, QFont::Bold));
    back.setStyleSheet("border-radius: 25px; background-color: white;");
    back.move(50, 50);
    back.hide();
    store1.setParent(this);
    store1.resize(200, 100);
    store1.setText("复活");
    store1.setFont(QFont("Times New Roman", 12, QFont::Bold));
    store1.setStyleSheet("border-radius: 25px; background-color: white;");
    store1.move(120, 600);
    store1.hide();
    store2.setParent(this);
    store2.resize(200, 100);
    store2.setText("血量");
    store2.setFont(QFont("Times New Roman", 12, QFont::Bold));
    store2.setStyleSheet("border-radius: 25px; background-color: white;");
    store2.move(505, 600);
    store2.hide();
    store3.setParent(this);
    store3.resize(200, 100);
    store3.setText("攻击力");
    store3.setFont(QFont("Times New Roman", 12, QFont::Bold));
    store3.setStyleSheet("border-radius: 25px; background-color: white;");
    store3.move(880, 600);
    store3.hide();
    connect(&back, &QPushButton::clicked, [=](){
        setWindowTitle("开始新的游戏，或者读取上一个存档");
        in_store = false;
        optionMode = false;
        back.hide();
        store1.hide();
        store2.hide();
        store3.hide();
        option1.hide();
        option2.hide();
        start.show();
        enterStore.show();
        quitGame.show();
        load.show();
    });
    connect(&store1, &QPushButton::clicked, [=](){
        if(coin >= 300) {
            coin -= 300;
            QMessageBox::information(nullptr, "提示", "您已完成购买");
            init_enhance.again = 1;
            store1.setText("MAX");
            store1.setEnabled(false);
        }
        else {
            QMessageBox::warning(nullptr, "警告", "您的金币不足");
        }
    });
    connect(&store2, &QPushButton::clicked, [=](){
        if(coin >= 200) {
            coin -= 200;
            QMessageBox::information(nullptr, "提示", "您已完成购买");
            init_enhance.HP = 1;
            store2.setText("MAX");
            store2.setEnabled(false);
        }
        else {
            QMessageBox::warning(nullptr, "警告", "您的金币不足");
        }
    });
    connect(&store3, &QPushButton::clicked, [=](){
        if(coin >= 200) {
            coin -= 200;
            QMessageBox::information(nullptr, "提示", "您已完成购买");
            init_enhance.Attack = 1;
            store3.setText("MAX");
            store3.setEnabled(false);
        }
        else {
            QMessageBox::warning(nullptr, "警告", "您的金币不足");
        }
    });
    //
    option1.resize(300, 100);
    option2.resize(300, 100);
    option1.setParent(this);
    option2.setParent(this);
    option1.hide();
    option2.hide();
    start.setText("开始游戏");
    enterStore.setText("进入商店");
    quitGame.setText("离开游戏");
    option1.setText("近战(绑定-铁拳)");
    option2.setText("远程(绑定-机枪)");
    start.setStyleSheet("border-radius: 25px; background-color: white;");
    start.setFont(QFont("Times New Roman", 12, QFont::Bold));
    enterStore.setStyleSheet("border-radius: 25px; background-color: white;");
    enterStore.setFont(QFont("Times New Roman", 12, QFont::Bold));
    quitGame.setStyleSheet("border-radius: 25px; background-color: white;");
    quitGame.setFont(QFont("Times New Roman", 12, QFont::Bold));
    option1.setStyleSheet("border-radius: 25px; background-color: white;");
    option1.setFont(QFont("Times New Roman", 12, QFont::Bold));
    option2.setStyleSheet("border-radius: 25px; background-color: white;");
    option2.setFont(QFont("Times New Roman", 12, QFont::Bold));
    connect(&this->start, &QPushButton::clicked, [=](){
        back.show();
        start_to_option();
    });
    connect(&this->enterStore, &QPushButton::clicked, this, &Start::start_to_store);
    connect(&this->option1, &QPushButton::clicked, this, [=](){
        play_option = 0;
        startGame();
    });
    connect(&this->option2, &QPushButton::clicked, this, [=](){
        play_option = 1;
        startGame();
    });
    connect(&this->quitGame, &QPushButton::clicked, [=](){
        this->close();
    });
    //读取存档键的相关设置
    load.setParent(this);
    load.setText("读取存档");
    load.resize(200, 100);
    load.move(this->width() * 2 / 3 + 50, this->height() * 2 / 3 - 150);
    load.setFont(QFont("Times New Roman", 12, QFont::Bold));
    load.setStyleSheet("border-radius: 25px; background-color: white;");
    connect(&load, &QPushButton::clicked, [=](){
        MainScene *last = new MainScene(nullptr, init_enhance, play_option);
        this->hide();
        last->loadGame();
        last->repaint();
        last->show();
    });
}

void Start::paintEvent(QPaintEvent *)
{

    QPainter p(this);
    p.drawPixmap(this->rect(), start_pixmap);
    if(optionMode)
    {
        hero_option1 = hero_option1.scaled(hero_option1.width() * 350 / hero_option1.width(), hero_option1.height() * 350 / hero_option1.height());
        hero_option2 = hero_option2.scaled(hero_option2.width() * 380 / hero_option2.width(), hero_option2.height() * 380 / hero_option2.height());
        p.drawPixmap(120, 175, hero_option1);
        p.drawPixmap(780, 180, hero_option2);
    }
    if(in_store) {
        pepsi = pepsi.scaled(pepsi.width() * 250 / pepsi.width(), pepsi.height() * 300 / pepsi.height());
        cola = cola.scaled(cola.width() * 300 / cola.width(), cola.height() * 350 / cola.height());
        fanta = fanta.scaled(fanta.width() * 230 / fanta.width(), fanta.height() * 300 / fanta.height());
        p.drawPixmap(100, 260, pepsi);
        p.drawPixmap(460, 225, cola);
        p.drawPixmap(860, 260, fanta);
    }
}

void Start::start_to_option()
{
    optionMode = true;
    start.hide();
    enterStore.hide();
    quitGame.hide();
    load.hide();
    option1.show();
    option2.show();
    option1.move(this->width() * 1 / 16, this->height() * 2 / 3);
    option2.move(this->width() * 2 / 3 + 25, this->height() * 2 / 3);
    this->setWindowTitle("请选择使用近战/远程英雄");
    repaint();
}

void Start::start_to_store()
{
    in_store = true;
    start.hide();
    enterStore.hide();
    quitGame.hide();
    load.hide();
    back.show();
    store1.show();
    store2.show();
    store3.show();
    if(!init_enhance.again)
        store1.setEnabled(true);
    if(!init_enhance.HP)
        store2.setEnabled(true);
    if(!init_enhance.Attack)
        store3.setEnabled(true);
}

void Start::startGame()
{
    MainScene *new_game = new MainScene(nullptr, init_enhance, play_option);
    this->hide();
    new_game->show();
}

MainScene::MainScene(QWidget *parent, Enhance_in_store se, bool option)
    : QWidget(parent), play_option(option)
{
    //窗口顶部设置定时器
    game_time.start(1000);
    connect(&game_time, &QTimer::timeout, [=](){
        count++;
        coin += 5;
    });
    map_is_draw = false;
    this->setFixedSize(2560, 1600);
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - this->width()) / 2;
    int y = (screenGeometry.height() - this->height()) / 2 - 80;
    this->move(x, y);

    movements << Qt::Key_W << Qt::Key_S << Qt::Key_A << Qt::Key_D;
    pix_node << QPixmap(":/picture/hero_status1.png")<<
            QPixmap(":/picture/hero_status2.png")<<
            QPixmap(":/picture/hero_status3.png")<<
            QPixmap(":/picture/hero_status4.png")<<
            QPixmap(":/picture/hero_status5.png")<<
            QPixmap(":/picture/hero_status6.png")<<
            QPixmap(":/picture/hero_status7.png");

    if(play_option)
        Hero_play = new Hero2(se.again, se.HP, se.Attack);
    else
        Hero_play = new Hero1(se.again, se.HP, se.Attack);
    //new 子弹对象
    for(int i = 0; i < Bullet_num; i++)
    {
        //角色2子弹
        m_bullet[i] = new Bullet(QRect(-35, -35, 35, 35), 0);
        m_bullet[i]->pixload(bullet_path);
        //篮球(怪物2子弹)
        m_ball[i] = new Bullet(QRect(i * 64, i * 64, 35, 35), 0);
        m_ball[i]->pixload(ball_path);
    }
    //怪物对象创建
    for(int i = 0; i < bomb_mNum; i++)
    {
        bomb_m[i] = new Monster1;
        bomb_m[i]->pixLoad(QPixmap(mon1_path));
    }
    for(int i = 0; i < fire_mNum; i++)
    {
        fire_m[i] = new Monster2;
        fire_m[i]->pixLoad(QPixmap(mon2_path));
    }
    for(int i = 0; i < itemNum; i++) {  //掉落物全部新建
        f_item[i] = new Falling_item;
    }

    update_time.start(30);
    connect(&update_time, &QTimer::timeout, [=](){
        this->repaint();        //更新游戏画面
        this->setWindowTitle(QString("类幸存者   Level: %1     升到下一等级需要经验: %2     当前血量: %4     当前速度: %5     当前攻击力: %6     You have survived for %3 second!     金币: %7").arg(level).arg(levelUP_need - exp_current).arg(count).arg(Hero_play->getCurrHP()).arg(Hero_play->getSpeed()).arg(Hero_play->get_Attack()).arg(coin));
        check_win_and_lose();  //胜负检测
        level_update();        //更新经验值
        detection_bullet_and_attack_and_item();     //检测子弹碰撞 与攻击  掉落物检测
        this->bomb_fix_move();    //炸弹人怪物定向移动
        this->check_mon_is_alive();   //存活检测
        for(int i = 0; i < Bullet_num; i++)   //已射出子弹的移动
        {
            if(m_bullet[i]->is_shot)
                m_bullet[i]->move();
            if(m_ball[i]->is_shot)
                m_ball[i]->move();
        }
    });
    attack1_time = new QTimer(this);
    attack1_time->start(500);
    attack2_time = new QTimer(this); //角色的攻击时间间隔
    attack2_time->start(200);
    transfer_time = new QTimer(this);
    transfer_time->start(30);
    if(play_option)
        connect(attack2_time, &QTimer::timeout, [=](){
            this->shoot();
        });
    else
    {
        connect(attack1_time, &QTimer::timeout, [=](){
            is_attacking ^= 1;
        });
        connect(transfer_time, &QTimer::timeout, [=](){
            this->transfer_timeout();
        });
    }
    generate_time.start(100);      //怪物生成的时间
    connect(&generate_time, &QTimer::timeout, [=](){
        this->generate();
        this->fireman_random_move();  //火人随机移动
    });
    ball_shoot_time.start(1000);
    connect(&ball_shoot_time, &QTimer::timeout, [=](){
        this->fireball_shoot();
    });
    //升级奖励按钮初始化与隐藏
    Hp.setParent(this);
    Speed.setParent(this);
    Attack.setParent(this);
    Hp.resize(400, 200);
    Speed.resize(400, 200);
    Attack.resize(400, 200);
    Hp.move(1000, 200);
    Speed.move(1000, 700);
    Attack.move(1000, 1200);
    Hp.setText("HP up!");
    Speed.setText("Speed up!");
    Attack.setText("Attack up!");
    Hp.setStyleSheet("border-radius: 25px; background-color: white;");
    Hp.setFont(QFont("Times New Roman", 25, QFont::Bold));
    Speed.setStyleSheet("border-radius: 25px; background-color: white;");
    Speed.setFont(QFont("Times New Roman", 25, QFont::Bold));
    Attack.setStyleSheet("border-radius: 25px; background-color: white;");
    Attack.setFont(QFont("Times New Roman", 25, QFont::Bold));
    ButtonHide();
    connect(&Hp, &QPushButton::clicked, [=](){
        Time_start();
        Hero_play->enhance_HP(50 * (level - 1));
        Hero_play->recover(50 * (level - 1));
        ButtonHide();
    });
    connect(&Speed, &QPushButton::clicked, [=](){
        Time_start();
        Hero_play->enhance_Speed(2 * (level - 1));
        ButtonHide();
    });
    connect(&Attack, &QPushButton::clicked, [=](){
        Time_start();
        Hero_play->enhance_attack(10 * (level - 1));
        ButtonHide();
    });
    //存档按钮初始化与隐藏
    Save.setParent(this);
    Save.setText("存档");
    Save.setFixedSize(400, 200);
    Save.move(1000, 700);
    Save.setStyleSheet("border-radius: 25px; background-color: white;");
    Save.setFont(QFont("Times New Roman", 25, QFont::Bold));
    Save.hide();
    connect(&Save, &QPushButton::clicked, [=](){
        SaveGame(GameSaveData{init_enhance.again, init_enhance.HP, init_enhance.Attack, level, exp_current, levelUP_need, coin, count, play_option, Hero_play->getHP(),
                 Hero_play->getCurrHP(), Hero_play->getSpeed(), Hero_play->get_Attack(), Hero_play->getRect().x(), Hero_play->getRect().y()});
        Save.hide();
        //qDebug() << init_enhance.again;
        Time_start();
    });
}

void MainScene::ButtonHide()
{
    Hp.hide();
    Speed.hide();
    Attack.hide();
}

void MainScene::level_update()
{
    if(exp_current >= levelUP_need)
    {
        exp_current -= levelUP_need;
        level++;
        levelUP_need += (level - 1) * 100;
        level_up_reward();
    }
}

void MainScene::level_up_reward()  //升级奖励   +Hp +Attack +Speed 三选一
{
    //先把计数器全部暂停
    Time_froze();
    Hp.show();
    Speed.show();
    Attack.show();
}

void MainScene::generate()  //怪物的生成
{
    srand(QTime::currentTime().msec());
    for(int i = 0; i < bomb_mNum; i++)
    {
        if(!bomb_m[i]->is_alive)
        {
            int xg = 0, yg = 0;
            int dir = rand() % 4;  //上0下1   //左2右3
            switch(dir) {
                case 0:
                    yg = 0;
                    xg = rand() % 2560;
                    while(detection_obs(QRect(xg, yg, 64, 64)))
                        xg = rand() % 2560;
                    bomb_m[i]->setRect(QRect(xg, yg, 64, 64));
                    bomb_m[i]->set_dir(1);
                    break;
                case 1:
                    yg = 1600 - 64;
                    xg = rand() % 2560;
                    while(detection_obs(QRect(xg, yg, 64, 64)))
                        xg = rand() % 2560;
                    bomb_m[i]->setRect(QRect(xg, yg, 64, 64));
                    bomb_m[i]->set_dir(0);
                    break;
                case 2:
                    xg = 0;
                    yg = rand() % 1600;
                    while(detection_obs(QRect(xg, yg, 64, 64)))
                        yg = rand() % 1600;
                    bomb_m[i]->setRect(QRect(xg, yg, 64, 64));
                    bomb_m[i]->set_dir(3);
                    break;
                case 3:
                    xg = 2560 - 64;
                    yg = rand() % 1600;
                    while(detection_obs(QRect(xg, yg, 64, 64)))
                        yg = rand() % 1600;
                    bomb_m[i]->setRect(QRect(xg, yg, 64, 64));
                    bomb_m[i]->set_dir(2);
                    break;
            }
            bomb_m[i]->is_alive = true;
            break;
        }
    }
    for(int i = 0; i < fire_mNum; i++)
    {
        if(!fire_m[i]->is_alive)
        {
            int xg = 0, yg = 0;
            int dir = rand() % 4;  //上0下1   //左2右3
            switch(dir) {
                case 0:
                    yg = 64;
                    xg = rand() % 2560;
                    fire_m[i]->setRect(QRect(xg, yg, 64, 64));
                    break;
                case 1:
                    yg = 1600 - 64 - 64;
                    xg = rand() % 2560;
                    fire_m[i]->setRect(QRect(xg, yg, 64, 64));
                    break;
                case 2:
                    xg = 64;
                    yg = rand() % 1600;
                    fire_m[i]->setRect(QRect(xg, yg, 64, 64));
                    break;
                case 3:
                    xg = 2560 - 64 - 64;
                    yg = rand() % 1600;
                    fire_m[i]->setRect(QRect(xg, yg, 64, 64));
                    break;
            }
            fire_m[i]->is_alive = true;
            fire_m[i]->newlife();  //重新设置血量
            fire_m[i]->is_fell = false;
            fire_m[i]->isInjure = false;
            break;
        }
    }
}

void MainScene::bomb_fix_move()
{
    for(int i = 0; i < bomb_mNum; i++)
    {
        QRect next;
        if(bomb_m[i]->is_alive)
            switch (dynamic_cast<Monster1*>(bomb_m[i])->getdir()) {
            case 0:
                next = QRect(bomb_m[i]->get_rect().x(), bomb_m[i]->get_rect().y() - bomb_m[i]->getSpeed(), 64, 64);
                if(!detection_obs(next) && boundary_detection(next))
                    bomb_m[i]->setRect(next);
                else
                {
                    bomb_m[i]->is_alive = false;
                    bomb_m[i]->change();
                }
                break;
            case 1:
                next = QRect(bomb_m[i]->get_rect().x(), bomb_m[i]->get_rect().y() + bomb_m[i]->getSpeed(), 64, 64);
                if(!detection_obs(next) && boundary_detection(next))
                    bomb_m[i]->setRect(next);
                else
                {
                    bomb_m[i]->is_alive = false;
                    bomb_m[i]->change();
                }
                break;
            case 2:
                next = QRect(bomb_m[i]->get_rect().x() - bomb_m[i]->getSpeed(), bomb_m[i]->get_rect().y(), 64, 64);
                if(!detection_obs(next) && boundary_detection(next))
                    bomb_m[i]->setRect(next);
                else
                {
                    bomb_m[i]->is_alive = false;
                    bomb_m[i]->change();
                }
                break;
            case 3:
                next = QRect(bomb_m[i]->get_rect().x() + bomb_m[i]->getSpeed(), bomb_m[i]->get_rect().y(), 64, 64);
                if(!detection_obs(next) && boundary_detection(next))
                    bomb_m[i]->setRect(next);
                else
                {
                    bomb_m[i]->is_alive = false;
                    bomb_m[i]->change();
                }
                break;
            }
        if(hero_clla(next)) {
            Hero_play->beAttacked(bomb_m[i]->get_Attack());
            Hero_play->isInjure = true;
            bomb_m[i]->is_alive = false;
            bomb_m[i]->change();
        }
    }
}

void MainScene::transfer_timeout()
{
    if(is_attacking)
    {
        index = (index + 1) % 7;
        Hero_play->pixLoad(pix_node[index]);
        if(hero_dir) //左
            Hero_play->mirrored();
    }
    else {
        Hero_play->pixLoad(pix_node[0]);
        if(hero_dir)
            Hero_play->mirrored();
    }   //未在攻击状态时回到初始
}

void MainScene::shoot()
{
    for(int i = 0; i < Bullet_num; i++)
    {
        if(!m_bullet[i]->is_shot)   //0.75 秒发射一次
        {
            m_bullet[i]->is_shot = true;
            m_bullet[i]->is_bomb = false;
            QRect start;
            if(hero_dir) //左
                start = QRect(Hero_play->getRect().center().x() - 20, Hero_play->getRect().center().y() - 15, 35, 35);
            else
                start = QRect(Hero_play->getRect().center().x() + 20, Hero_play->getRect().center().y() - 15, 35, 35);
            m_bullet[i]->setRect(start);
            if(hero_dir)
                m_bullet[i]->mirrored();
            m_bullet[i]->set_dir(hero_dir);
            repaint();
            return;
        }
    }
}

void MainScene::fireball_shoot()
{
    srand(QTime::currentTime().msec());
    for(int i = 0; i < fire_mNum; i++)
    {
        if(fire_m[i]->is_alive && fire_m[i])    //上下左右随机 射出一个方向
        {
            for(int j = 0; j < Bullet_num; j++)
            {
                if(!m_ball[j]->is_shot && m_ball[j])
                {
                    int dir = rand() % 4;
                    m_ball[j]->is_shot = true;
                    QRect start;
                    switch (dir) {
                        case 0:  //上
                            start = QRect(fire_m[i]->get_rect().center().x() - 20, fire_m[i]->get_rect().center().y() - 40, 35, 35);
                            m_ball[j]->setRect(start);
                            m_ball[j]->set_dir(2);
                            break;
                        case 1:   //下
                            start = QRect(fire_m[i]->get_rect().center().x() - 20, fire_m[i]->get_rect().center().y() + 40, 35, 35);
                            m_ball[j]->setRect(start);
                            m_ball[j]->set_dir(3);
                            break;
                        case 2:   //左
                            start = QRect(fire_m[i]->get_rect().center().x() - 40, fire_m[i]->get_rect().center().y() - 20, 35, 35);
                            m_ball[j]->setRect(start);
                            m_ball[j]->set_dir(1);
                            break;
                        case 3:   //右
                            start = QRect(fire_m[i]->get_rect().center().x() + 40, fire_m[i]->get_rect().center().y() - 20, 35, 35);
                            m_ball[j]->setRect(start);
                            m_ball[j]->set_dir(0);
                            break;
                        default:
                            break;
                    }
                    repaint();
                    break;
                }
            }
        }
    }
}

int MainScene::get_relative_pos(Monster2 *m)   //判断相对位置
{
    QPoint p2 = Hero_play->getRect().center();
    QPoint p1 = m->get_rect().center();
    if(p1.x() <= p2.x()) {
        if(p1.y() <= p2.y()) {
            return 1;
        }
        else {
            return 3;
        }
    }
    else {
        if(p1.y() <= p2.y()) {
            return 2;
        }
        else {
            return 4;
        }
    }
}

int MainScene::get_distance(Monster2 *m)
{
    QPoint p2 = Hero_play->getRect().center();
    QPoint p1 = m->get_rect().center();
    return abs(p1.x() - p2.x()) + abs(p1.y() - p2.y());
}

void MainScene::fireman_random_move()
{
    srand(QTime::currentTime().msec());
    for(int i = 0; i < fire_mNum; i++)
    {
        if(fire_m[i]->is_alive)
        {
            QRect next;
            int dir = rand() % 2;
            int ret = get_relative_pos(fire_m[i]);
//            qDebug() << get_distance(fire_m[i]);
            switch (ret) {
            case 1:  //向右或者向下
                if(dir == 0)  //右
                    next = QRect(fire_m[i]->get_rect().x() + fire_m[i]->getSpeed(), fire_m[i]->get_rect().y(), 64, 64);
                else
                    next = QRect(fire_m[i]->get_rect().x(), fire_m[i]->get_rect().y() + fire_m[i]->getSpeed(), 64, 64);
                if(boundary_detection_for_fireman(next) && get_distance(fire_m[i]) >= 200) {
                    fire_m[i]->setRect(next);
                }
                break;
            case 2:  //向左或者向下
                if(dir == 0)  //左
                    next = QRect(fire_m[i]->get_rect().x() - fire_m[i]->getSpeed(), fire_m[i]->get_rect().y(), 64, 64);
                else
                    next = QRect(fire_m[i]->get_rect().x(), fire_m[i]->get_rect().y() + fire_m[i]->getSpeed(), 64, 64);
                if(boundary_detection_for_fireman(next) && get_distance(fire_m[i]) >= 200) {
                    fire_m[i]->setRect(next);
                }
                break;
            case 3:   //向右或者向上
                if(dir == 0)  //右
                    next = QRect(fire_m[i]->get_rect().x() + fire_m[i]->getSpeed(), fire_m[i]->get_rect().y(), 64, 64);
                else
                    next = QRect(fire_m[i]->get_rect().x(), fire_m[i]->get_rect().y() - fire_m[i]->getSpeed(), 64, 64);
                if(boundary_detection_for_fireman(next) && get_distance(fire_m[i]) >= 200) {
                    fire_m[i]->setRect(next);
                }
                break;
            case 4:    //向左或者向下
                if(dir == 0)  //左
                    next = QRect(fire_m[i]->get_rect().x() - fire_m[i]->getSpeed(), fire_m[i]->get_rect().y(), 64, 64);
                else
                    next = QRect(fire_m[i]->get_rect().x(), fire_m[i]->get_rect().y() + fire_m[i]->getSpeed(), 64, 64);
                if(boundary_detection_for_fireman(next) && get_distance(fire_m[i]) >= 200) {
                    fire_m[i]->setRect(next);
                }
                break;
            default:
                break;
            }
        }
    }
}

void MainScene::check_mon_is_alive() //检测怪物生存状态
{
    for(int i = 0; i < fire_mNum; i++)
    {
        if(fire_m[i]->getcurrHP() <= 0 && !fire_m[i]->is_fell)
        {
            //随机生成掉落物吧
            falling_item_generate();
            fire_m[i]->is_fell = true;
            fire_m[i]->is_alive = false;
            fire_m[i]->setRect(QRect(-1, -1, 1, 1));   //死亡的怪物移到界外
        }
    }
}

void MainScene::falling_item_generate()
{
    srand(QTime::currentTime().msec());
    int xg = rand() % (39 * 64);
    int yg = rand() % (24 * 64);
    while(obs_positon.contains(QRect(xg, yg, 64, 64)))
    {
        xg = rand() % (39 * 64);
        yg = rand() % (24 * 64);
    }
    for(int i = 0; i < itemNum; i++)
    {
        if(!f_item[i]->check())
        {
            int type = QRandomGenerator::global()->bounded(2);
            f_item[i]->setType(type);
            if(type)
                f_item[i]->pixLoad(QPixmap(item_blood_path));
            else
                f_item[i]->pixLoad((QPixmap(item_exp_path)));
            f_item[i]->setRect(QRect(xg, yg, 50, 50));
            f_item[i]->change();
            return;
        }
    }
}

void MainScene::detection_bullet_and_attack_and_item()
{
    for(int i = 0; i < Bullet_num; i++)
    {
        if(m_bullet[i])
        {
            if(!boundary_detection(m_bullet[i]->get_rect()))   //子弹出界回收
                m_bullet[i]->is_shot = false;
            for(int j = 0; j < fire_mNum; j++)
                if(m_bullet[i]->is_shot && m_bullet[i]->get_rect().intersects(fire_m[j]->get_rect())) {
                    fire_m[j]->beAttacked(Hero_play->get_Attack());
                    fire_m[j]->isInjure = true;
                    m_bullet[i]->is_bomb = true;
                    m_bullet[i]->is_shot = false;
                }
            for(int j = 0; j < obs_positon.size(); j++)
                if(m_bullet[i]->get_rect().intersects(obs_positon[j]) && m_bullet[i]->is_shot) {
                    m_bullet[i]->is_bomb = true;
                    m_bullet[i]->is_shot = false;
                }
        }
        if(m_ball[i])
        {
            if(!boundary_detection(m_ball[i]->get_rect()))   //篮球的出界回收
                m_ball[i]->is_shot = false;
            if(m_ball[i]->is_shot && m_ball[i]->get_rect().intersects(Hero_play->getRect()))   //篮球与玩家碰撞检测
            {
                Hero_play->beAttacked(fire_m[0]->get_Attack());
                Hero_play->isInjure = true;
                m_ball[i]->is_shot = false;
            }
        }
    }
    for(int j = 0; j < fire_mNum; j++)   //近战
        if(!play_option && is_attacking)
            if(fire_m[j]->get_rect().intersects(Hero_play->getRect())) {
                fire_m[j]->beAttacked(Hero_play->get_Attack());
                fire_m[j]->isInjure = true;
            }
    for(int i = 0; i < itemNum; i++)
    {
        if(f_item[i]->check() && f_item[i]->getRect().intersects(Hero_play->getRect()))
        {
            //下面是提升类型
            if(f_item[i]->UPtype())
                Hero_play->recover(f_item[i]->bloodUP());
            else
                exp_current += f_item[i]->expUP();
            f_item[i]->change();
        }
    }
}

void MainScene::paintEvent(QPaintEvent *)
{
    QPainter *p = new QPainter(this);
    //画地图，如何避免重复绘制？？？
    for(int i = 0; i < 40; i++)
    {
        for(int j = 0; j < 25; j++)
        {
            p->drawPixmap(i * 64, j * 64, game_map.get_map());
            for(int k = 0; k < 12; k++)
                if(obs[k].x() == i && obs[k].y() == j)
                    p->drawPixmap(i * 64, j * 64, game_map.get_obs());
        }
    }
    srand(QTime::currentTime().hour());  //随机数种子
    //画障碍物
    for(int k = 0; k < 10; k++)
    {
        int i = rand() % 40;
        int j = rand() % 25;
        while(obs_positon.contains(QRect(i, j, 64, 64)))
        {
            i = rand() % 40;
            j = rand() % 25;
        }
        obs[k + 12] = QRect(i, j, 64, 64);
        p->drawPixmap(i * 64, j * 64, game_map.get_obs());
    }
    if(!map_is_draw) {
            map_is_draw = true;
                for(int i = 0; i < 22; i++)
                {
                    //扩大到像素级别， 方便进行碰撞检测
                    //qDebug() << obs[i];
                    obs_positon.push_back(QRect(obs[i].x() * 64, obs[i].y() * 64, 64, 64));
                }
    }
    for(int i = 0; i < 22; i++)  //小时随机数 更新障碍物位置
    {
        obs_positon[i] = QRect(obs[i].x() * 64, obs[i].y() * 64, 64, 64);
    }
    //画人物  判断是否受伤
    if(Hero_play->isInjure)
    {
        QPixmap redMask(Hero_play->getpix().size());
        redMask.fill(QColor(255, 0, 0, 100)); // 使用红色半透明颜色
        p->drawPixmap(Hero_play->getRect() ,redMask);
        p->drawPixmap(Hero_play->getRect(), Hero_play->getpix());
        Hero_play->isInjure = false;
    }
    p->drawPixmap(Hero_play->getRect(), Hero_play->getpix());

    //画人物血条  高为20，宽96(像素)
    int barX = Hero_play->getRect().x() + 2;
    int barY = Hero_play->getRect().y() - 18;
    p->setPen(Qt::NoPen);
    p->setBrush(QColor(200, 200, 200));
    p->drawRect(barX, barY, 96, 20);
    p->setBrush(QColor(255, 0, 0));
    p->drawRect(barX, barY, Hero_play->getCurrHP()  * 96 / Hero_play->getHP(), 20);

    //画子弹 与爆炸效果
    for(int i = 0; i < Bullet_num; i++) {
        if(m_bullet[i]->is_shot)
            p->drawPixmap(m_bullet[i]->get_rect(), m_bullet[i]->get_pix());
        else if(m_bullet[i]->is_bomb)
        {
            QRect bomb_;
            if(hero_dir)
                bomb_ = QRect(m_bullet[i]->get_rect().x() - 15, m_bullet[i]->get_rect().y(), 64, 64);
            else
                bomb_ = QRect(m_bullet[i]->get_rect().x() + 10, m_bullet[i]->get_rect().y(), 64, 64);
            p->drawPixmap(bomb_, QPixmap(bomb_path));
            m_bullet[i]->is_bomb = false;
            m_bullet[i]->setRect(QRect(-1, -1, 1, 1));  //快移走！
        }
    }

    //画炸弹人怪物
    for(int i = 0; i < bomb_mNum; i++)
        if(bomb_m[i]->is_alive)
            p->drawPixmap(bomb_m[i]->get_rect(), bomb_m[i]->get_pix());
        else if(bomb_m[i]->check()) {
            p->drawPixmap(bomb_m[i]->get_rect(), QPixmap(bomb_path));
            bomb_m[i]->change();
            bomb_m[i]->setRect(QRect(-1, -1, 1, 1));
        }
    //画火人   及其血条 长为64， 高为18  判断是否受伤
    for(int i = 0; i < fire_mNum; i++)
    {
        if(fire_m[i]->is_alive)
        {
            p->drawPixmap(fire_m[i]->get_rect(), fire_m[i]->get_pix());
            int barX = fire_m[i]->get_rect().x();
            int barY = fire_m[i]->get_rect().y() - 18;
            p->setPen(Qt::NoPen);
            p->setBrush(QColor(200, 200, 200));   //先画灰色背景
            p->drawRect(barX, barY, 64, 18);
            p->setBrush(QColor(255, 0, 0));
            p->drawRect(barX, barY, fire_m[i]->getcurrHP()  * 64 / fire_m[i]->getHP(), 18);
        }
        if(fire_m[i]->isInjure)
        {
            QPixmap redMask(fire_m[i]->get_pix().size());
            redMask.fill(QColor(255, 0, 0, 100)); // 使用红色半透明颜色
            p->drawPixmap(fire_m[i]->get_rect(), redMask);
            fire_m[i]->isInjure = false;
        }
    }
    //绘制掉落物
    for(int i = 0; i < itemNum; i++)
    {
        if(f_item[i]->check())
            p->drawPixmap(f_item[i]->getRect(), f_item[i]->getPix());
    }

    for(int i = 0; i < Bullet_num; i++)
        if(m_ball[i]->is_shot)
            p->drawPixmap(m_ball[i]->get_rect(), m_ball[i]->get_pix());
    p->end();
}

bool MainScene::detection_obs(QRect new_rect)
{
    for(int i = 0; i < obs_positon.size(); i++)
    {
        if(obs_positon[i].intersects(new_rect))
            return true;
    }
    return false;
}

bool MainScene::hero_clla(QRect item)
{
    if(item.intersects(Hero_play->getRect()))
        return true;
    return false;
}

bool MainScene::boundary_detection(QRect new_rect)
{
    return new_rect.x() >= 0 && new_rect.y() >= 0 && new_rect.x() <= 40 * 64 && new_rect.y() <= 25 * 64;
}

bool MainScene::boundary_detection_for_fireman(QRect new_rect)
{
    return new_rect.x() >= 0 && new_rect.y() >= 0 && new_rect.x() <= 39 * 64 + 32 && new_rect.y() <= 24 * 64 + 32;
}

void MainScene::handleMovement(int key)
{
    if (movements.contains(key)) {
            // 处理对应方向的移动操作
            if (key == Qt::Key_W) {
                QRect next_position(Hero_play->getRect().x(), Hero_play->getRect().y() - Hero_play->getSpeed(), 96, 96);
                if(!detection_obs(next_position) && boundary_detection(next_position))
                    Hero_play->setRect(next_position);
            } else if (key == Qt::Key_A) {
                if(!hero_dir) //为右
                {
                    Hero_play->mirrored();
                    hero_dir = true;
                }
                QPixmap help = QPixmap(hero2_path);
                help = help.transformed(QTransform().scale(-1, 1));
                if(play_option)
                    Hero_play->pixLoad(help);
                QRect next_position(Hero_play->getRect().x() - Hero_play->getSpeed(), Hero_play->getRect().y(), 96, 96);
                if(!detection_obs(next_position) && boundary_detection(next_position))
                    Hero_play->setRect(next_position);
            } else if (key == Qt::Key_S) {
                QRect next_position(Hero_play->getRect().x(), Hero_play->getRect().y() + Hero_play->getSpeed(), 96, 96);
                if(!detection_obs(next_position) && boundary_detection(next_position))
                    Hero_play->setRect(next_position);
            } else if (key == Qt::Key_D) {
                if(hero_dir)  //为左
                {
                    Hero_play->mirrored();
                    hero_dir = false;
                }
                QPixmap help = QPixmap(hero2_path);
                if(play_option)
                    Hero_play->pixLoad(help);
                QRect next_position(Hero_play->getRect().x() + Hero_play->getSpeed(), Hero_play->getRect().y(), 96, 96);
                if(!detection_obs(next_position) && boundary_detection(next_position))
                    Hero_play->setRect(next_position);
            }
        }
}

int spaceType = 0;
void MainScene::keyPressEvent(QKeyEvent *event)
{
    if (movements.contains(event->key())) {
            handleMovement(event->key());
        }
    else if(event->key() == Qt::Key_Space)
    {   //存档按钮出现
        spaceType ^= 1;
        if(spaceType)
        {
            Time_froze();
            Save.show();
        }
        else
        {
            Time_start();
            Save.hide();
        }
    }
}

void MainScene::check_win_and_lose()
{
    if(count >= winTime && Hero_play->getCurrHP() > 0)  //把两个要命的计时器关掉
    {
        Time_froze();
        end_widget *final = new end_widget(1);
        this->close();
        //this->~MainScene();
        final->show();
    }
    else if(Hero_play->getCurrHP() <= 0)
    {
        if(Hero_play->check_again()) {
            Hero_play->newlife();
            Hero_play->consume();
            Time_froze();
            //player->stop();
            QMessageBox::information(nullptr, "提示", "你已复活，多加小心");
            Time_start();
        }
        else {
            Time_froze();
            //player->stop();
            end_widget *final = new end_widget(0);
            this->close();
            //this->~MainScene();
            final->show();
        }
    }
}

void MainScene::Time_froze()
{
    game_time.stop();
    update_time.stop();
    attack1_time->stop();
    attack2_time->stop();
    generate_time.stop();
    ball_shoot_time.stop();
}

void MainScene::Time_start()
{
    game_time.start();
    update_time.start();
    attack1_time->start();
    attack2_time->start();
    generate_time.start();
    ball_shoot_time.start();
}

void MainScene::SaveGame(const GameSaveData &data)
{
    QSettings setting("存档", "幸存者");
    //写入
    //qDebug() << data.again;  //输出1
    setting.setValue("again", QVariant(data.again));
    //qDebug() << setting.value("again").toBool();  //输出0 -> tobool
    setting.setValue("e_HP", data.e_HP);
    setting.setValue("e_Attack", data.e_Attack);
    setting.setValue("level", data.level);
    setting.setValue("exp", data.exp);
    setting.setValue("level_up_need", data.level_up_need);
    setting.setValue("coin", data.coin);
    setting.setValue("cnt", data.cnt);
    //qDebug() << setting.value("cnt").toInt();
    setting.setValue("option", data.option);
    setting.setValue("HPsum", data.HPsum);
    setting.setValue("HPcurrent", data.HPcurrent);
    setting.setValue("Speed", data.Speed);
    setting.setValue("Attack", data.Attack);
    setting.setValue("x", data.x);
    setting.setValue("y", data.y);
}

void MainScene::loadGame()
{
    QSettings setting("存档", "幸存者");
    //读取
    level = setting.value("level", 1).toInt();  //默认1级
    exp_current = setting.value("exp", 0).toInt();
    levelUP_need = setting.value("level_up_need", 50).toInt();
    coin = setting.value("coin", 250).toInt();  //金币默认250
    play_option = setting.value("option", 0).toInt(); //默认近战
    count = setting.value("cnt", 0).toInt();
    repaint();
    delete attack1_time;
    delete attack2_time;
    delete transfer_time;
    attack1_time = new QTimer(this);
    attack1_time->start(500);
    attack2_time = new QTimer(this); //角色的攻击时间间隔
    attack2_time->start(200);
    transfer_time = new QTimer(this);
    transfer_time->start(30);
    if(play_option) {
        connect(attack2_time, &QTimer::timeout, [=](){
            this->shoot();
        });
        Hero_play->pixLoad(QPixmap(hero2_path));
        repaint();
    }
    else
    {
        connect(attack1_time, &QTimer::timeout, [=](){
            is_attacking ^= 1;
        });
        connect(transfer_time, &QTimer::timeout, [=](){
            this->transfer_timeout();
        });
    }
    repaint();
    Hero_play->set_again(setting.value("again").toBool());
    Hero_play->set_HPsum(setting.value("HPsum", 300).toInt());
    Hero_play->set_HPcurrent(setting.value("HPcurrent", 300).toInt());
    Hero_play->set_Speed(setting.value("Speed", 15).toInt());
    Hero_play->set_Attack(setting.value("Attack", 30).toInt());
    Hero_play->setRect(QRect(setting.value("x", 128).toInt(), setting.value("y", 128).toInt(), 96, 96));
}

MainScene::~MainScene()
{
    delete attack1_time;
    delete attack2_time;
    delete transfer_time;
    for(int i = 0; i < Bullet_num; i++)
    {
        delete m_bullet[i];
        delete m_ball[i];
    }
    delete Hero_play;
    for(int i = 0; i < bomb_mNum; i++)
    {
        delete bomb_m[i];
    }
    for(int i = 0; i < fire_mNum; i++)
    {
        delete fire_m[i];
    }
    for(int i = 0; i < itemNum; i++) {
        delete f_item[i];
    }
}

end_widget::end_widget(int type)
{
    end_widget::setFixedSize(900, 800);
    if(type) //win!
    {
        setWindowTitle("好吧 我承认你有点东西");
        end_pix.load(win_path);
    }
    else
    {
        setWindowTitle("胜负乃兵家常事，大侠请重新来过");
        end_pix.load(lose_path);
    }
    choose.setParent(this);
    choose.setText("New Start");
    choose.resize(200, 100);
    choose.setStyleSheet("border-radius: 25px; background-color: black; color: white");
    choose.setFont(QFont("Times New Roman", 12, QFont::Bold));
    choose.move(350, 650);
    connect(&choose, &QPushButton::clicked, [=](){
       Start *again = new Start();
       this->close();
       this->~end_widget();
       again->show();
    });
}

void end_widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(this->rect(), end_pix);
}
