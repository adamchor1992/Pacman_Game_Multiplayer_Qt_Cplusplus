#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "map.h"
#include "pacman.h"
#include "powerball.h"
#include "foodball.h"
#include "ghost.h"
#include "textscreenmessage.h"
#include "sounds.h"
#include "clientconnection.h"
#include "assert.h"

#include "askforipinterface.h"

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QPainter>
#include <QGraphicsScene>
#include <QList>
#include "QRegExp"

namespace Ui
{
class Game_window;
}

class Game_window : public QMainWindow
{
    Q_OBJECT

private:
    Ui::Game_window *ui;

    QGraphicsScene scene;

    QTimer updatertimer;
    QTimer sceneupdate_timer;
    QTimer updatecoordinates_timer;
    QTimer wait_for_restart_key_timer;
    QTimer wait_for_game_restart_timer;

    PowerBall power_ball;
    FoodBall food_ball;

    ClientConnection *clientconnection;

    TextScreenMessage textscreenmessage;

    Map pac_map;
    Pacman pac_man;
    Ghost ghostplayer;
    Sounds sounds;

    QGraphicsPixmapItem *map_item;

    QVector<QPoint> powerball_positions;
    QVector<QPoint> foodball_positions;

    QVector<QGraphicsEllipseItem*> foodball_graphical_items_table;
    QVector<QGraphicsEllipseItem*> powerball_graphical_items_table;

    QMap<QString, QGraphicsEllipseItem*> powerball_graphical_items_table_dict;
    QMap<QString, QGraphicsEllipseItem*> foodball_graphical_items_table_dict;

    int foodball_items_count;
    int powerball_items_count;
    int game_state; //0-not started yet, 1-started and running, 2-paused, 3-to be restarted, 4-pacman wins, 5-ghost wins
    bool waitingforrestartkey;
    bool restartpending;

public:
    explicit Game_window(QWidget *parent = 0, QHostAddress _address = QHostAddress("0"), uint _port = 0);

    ~Game_window();

    void GenerateMap();
    void PopulateMap();
    void GenerateAndPlacePacman();
    void GenerateAndPlaceGhosts();
    void InitializeSounds();

    void PrepareGameToStart();
    void ResetVariablesandContainers();
    void HideSceneItems();
    void RestartGame();

public slots:
    void StartGame();
    void CheckForRestartGameSignal();
    void updater();
    void UpdateCoordinatesFromServer();
    void UpdateScene();

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // GAME_WINDOW_H
