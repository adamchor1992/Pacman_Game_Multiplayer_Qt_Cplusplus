#pragma once

#include "map.h"
#include "pacman.h"
#include "../common/powerball_manager.h"
#include "../common/foodball_manager.h"
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

public:
    explicit GameWindow(QWidget *parent = 0, QHostAddress address = QHostAddress("0"));
    ~GameWindow();

private:
    const int SCENE_WIDTH = 614;
    const int SCENE_HEIGHT = 740;
    const int MESSAGE_TIMEOUT = 3000;

    Ui::Game_window* ui;

    QGraphicsScene m_Scene;

    QTimer m_UpdaterTimer;
    QTimer m_SceneUpdateTimer;
    QTimer m_IpdateCoordinatesTimer;
    QTimer m_WaitForRestartKeyTimer;
    QTimer m_WaitForGameRestartTimer;

    PowerballManager m_PowerballManager;
    FoodballManager m_FoodBallManager;

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

    bool m_WaitingForRestartKey;
    bool m_RestartPending;

    void GenerateMap();
    void PopulateMap();
    void GenerateAndPlacePacman();
    void GenerateAndPlaceGhosts();
    void InitializeSounds();
    void PrepareGameToStart();
    void ResetVariablesAndContainers();
    void HideSceneItems();
    void RestartGame();

    void keyPressEvent(QKeyEvent *event);

    enum class GameState
    {
        BeforeFirstRun = 0,
        Running = 1,
        Paused = 2,
        Aborted = 3,
        PacmanWin = 4,
        GhostWin = 5
    } m_GameState;

private slots:
    void StartGame();
    void CheckForRestartGameSignal();
    void Updater();
    void UpdateCoordinatesFromServer();
    void UpdateScene();
};
