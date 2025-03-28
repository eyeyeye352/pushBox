#include "gamesys.h"

Gamesys::Gamesys(QWidget *parent)
    : QWidget(parent)
{

    //设置窗口
    setFixedSize(800,600);
    setWindowIcon(QIcon(":/img/img/box.png"));
    setWindowTitle("推箱箱");

    //载入地图
    map = new GameMap(this);
    map->loadMap(true);
    map->setMapPos(QPoint(this->width()/3,this->height()/4));
    map->setElementSize(30);

    connect(map,&GameMap::loadMapErr,this,&Gamesys::onLoadMapFail);
    connect(map,&GameMap::noLastPoint,this,&Gamesys::onGameComplete);

    //玩家
    p1 = new Player(this);
    p1->connectMap(map);

    //其它设置
    painter = new QPainter(this);

    //说明窗口显示
    showHelpWidget();

}

Gamesys::~Gamesys(){}


//游戏帮助说明窗口
void Gamesys::showHelpWidget(){
    QDialog* help = new QDialog(this);
    QLabel* label = new QLabel("推箱子（ver 1.1.0）\n"
                               "2025/3/28\n\n"
                               "游戏说明：\n\n"
                              "(W)(A)(S)(D) 移动 (也可以使用方向键！)\n"
                              "(R) 重置地图\n"
                              "(C) 更换地图\n"
                              "(H) 显示游戏说明\n\n"
                              "自制、修改游戏地图文件，可参考maps文件夹中的example.txt。\n"
                               "官方地图皆在examples文件夹中！\n\n"
                               "感谢您的下载。",help);
    label->setFont(QFont("Arial",12));
    label->move(QPoint(10,10));

    help->setFixedSize(600,500);
    help->setWindowTitle("游戏说明");
    help->show();
};



void Gamesys::paintEvent(QPaintEvent *event){

    painter->begin(this);

    //背景(优化：若已绘制则不重新绘制)
    painter->drawImage(this->rect(),QImage(":/img/img/background.png"));

    //画地图
    map->paintMap(painter);

    //画玩家
    p1->paintPlayer(painter);

    painter->end();
}


/*系统接收键盘事件，移动玩家
*功能键：
* (C) 读取新地图
* (R) 重置现有地图
* (H) 显示帮助窗口
*/

void Gamesys::keyPressEvent(QKeyEvent *event){

    switch(event->key()){

        case::Qt::Key_W:
        case::Qt::Key_Up:
            p1->move(Direction::UP);
        break;

        case::Qt::Key_A:
        case::Qt::Key_Left:
            p1->move(Direction::LEFT);
        break;

        case::Qt::Key_S:
        case::Qt::Key_Down:
            p1->move(Direction::DOWN);
        break;

        case::Qt::Key_D:
        case::Qt::Key_Right:
            p1->move(Direction::RIGHT);
        break;

        case::Qt::Key_R:
            resetGame(false);
        break;

        case::Qt::Key_C:
            resetGame(true);
        break;

        case::Qt::Key_H:
            showHelpWidget();
        break;
    }

    this->update();
}


//重置，changefile决定是否更换地图。
void Gamesys::resetGame(bool changefile){
    map->loadMap(changefile);
    p1->connectMap(map);
}

//胜利弹窗槽函数。
void Gamesys::onGameComplete(){
    QMessageBox::information(this,"完成!","恭喜你推满了所有的箱子!\n"
                                            "按（H）可查看进阶说明。");
}

//加载地图失败槽函数。
void Gamesys::onLoadMapFail(){
    QMessageBox::critical(this,"载入失败","地图文件格式错误，或者你没有选择文件！");
}
