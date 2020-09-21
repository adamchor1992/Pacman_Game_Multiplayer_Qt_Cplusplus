#pragma once

#include "map.h"
#include "pacman.h"
#include "powerball.h"
#include "foodball.h"
#include "ghost.h"
#include "text_screen_message.h"
#include "sounds.h"
#include "client_connection.h"
#include "assert.h"
#include "connection_dialog_window.h"

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

class GameWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::Game_window *ui;

    QGraphicsScene m_Scene;

    QTimer m_UpdaterTimer;
    QTimer m_SceneUpdateTimer;
    QTimer m_IpdateCoordinatesTimer;
    QTimer m_WaitForRestartKeyTimer;
    QTimer m_WaitForGameRestartTimer;

    PowerBall m_Powerball;
    FoodBall m_FoodBall;

    ClientConnection* m_pClientConnection;

    TextScreenMessage m_TextScreenMessage;

    Map m_Pacmap;
    Pacman m_Pacman;
    Ghost m_Ghostplayer;
    Sounds m_Sounds;

    QGraphicsPixmapItem* m_pMapItem;

    QVector<QPoint> m_PowerballPositions;
    QVector<QPoint> m_FoodballPositions;

    QVector<QGraphicsEllipseItem*> m_FoodballGraphicalItemsTable;
    QVector<QGraphicsEllipseItem*> m_PowerballGraphicalItemsTable;

    QMap<QString, QGraphicsEllipseItem*> m_PowerballGraphicalItemsTableDict;
    QMap<QString, QGraphicsEllipseItem*> m_FoodballGraphicalItemsTableDict;

    int m_FoodballItemsCount;
    int m_PowerballItemsCount;
    int m_GameState; //0-not started yet, 1-started and running, 2-paused, 3-to be restarted, 4-pacman wins, 5-ghost wins
    bool m_WaitingForRestartKey;
    bool m_RestartPending;

    const int GAME_PORT = 5000;

    void GenerateMap();
    void PopulateMap();
    void GenerateAndPlacePacman();
    void GenerateAndPlaceGhosts();
    void InitializeSounds();
    void PrepareGameToStart();
    void ResetVariablesandContainers();
    void HideSceneItems();
    void RestartGame();

public:
    explicit GameWindow(QWidget *parent = 0, QHostAddress address = QHostAddress("0"));
    ~GameWindow();

private slots:
    void StartGame();
    void CheckForRestartGameSignal();
    void Updater();
    void UpdateCoordinatesFromServer();
    void UpdateScene();

protected:
    void keyPressEvent(QKeyEvent *event);
};
