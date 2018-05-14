#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H


#include "map.h"
#include "pacman.h"
#include "powerball.h"
#include "foodball.h"
#include "ghost.h"
//#include "textstartend.h"
#include "clientconnection.h"
#include "assert.h"

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QList>
#include "QRegExp"
//#include <QMediaPlayer>

namespace Ui
{
class Game_window;
}

class Game_window : public QDialog
{
    Q_OBJECT

private:
    Ui::Game_window *ui;
    QGraphicsScene *scene;
    QTimer *updatertimer;
    QTimer *updatecoordinates_timer;

    PowerBall *power_ball;
    FoodBall *food_ball;

    ClientConnection *clientconnection;

    Map *pac_map;
    Pacman *pac_man;
    Ghost *ghostplayer;

    QGraphicsPixmapItem *map_item;

    QVector<QPoint> *powerball_positions;
    QVector<QPoint> *foodball_positions;

    QVector<QGraphicsEllipseItem*> foodball_graphical_items_table;
    QVector<QGraphicsEllipseItem*> powerball_graphical_items_table;

    QMap<QString, QGraphicsEllipseItem*> *powerball_graphical_items_table_dict;
    QMap<QString, QGraphicsEllipseItem*> *foodball_graphical_items_table_dict;

    int foodball_items_count;
    int powerball_items_count;

public:
    explicit Game_window(QWidget *parent = 0);
    ~Game_window();

    void GenerateMap();
    void PopulateMap();
    void GenerateAndPlacePacman();
    void GenerateAndPlaceGhosts();

    void StartGame();

public slots:
    void updater();
    void UpdateCoordinatesFromServer();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // GAME_WINDOW_H
