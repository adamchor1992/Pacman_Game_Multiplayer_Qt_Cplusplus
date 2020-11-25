#pragma once

#include "pacman.h"
#include "../common/map.h"
#include "../common/powerball_manager.h"
#include "../common/foodball_manager.h"
#include "ghost.h"
#include "screen_text_display.h"
#include "sounds.h"
#include "server_connection.h"
#include "connection_dialog_window.h"
#include "status_bar_manager.h"

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <QGraphicsScene>
#include <cassert>

namespace Ui
{
class Game_window;
}

class GameWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget* parent = 0, QHostAddress address = QHostAddress("0"));
    ~GameWindow();

private:
    const int SCENE_WIDTH = 614;
    const int SCENE_HEIGHT = 740;
    const int UPDATER_TIMEOUT = 6;
    const int SCENE_UPDATE_TIMEOUT = 10;
    const int PROCESS_NEW_DATA_TIMEOUT = 5;

    Ui::Game_window* ui;

    QGraphicsScene m_Scene;

    QTimer m_UpdaterTimer;
    QTimer m_SceneUpdateTimer;
    QTimer m_WaitForRestartKeyTimer;
    QTimer m_WaitForGameRestartTimer;
    QTimer m_ProcessNewDataTimer;

    FoodballManager m_FoodBallManager;
    PowerballManager m_PowerballManager;

    ServerConnection m_ServerConnection;

    ScreenTextDisplay m_ScreenTextDisplay;

    Map m_Map;
    Pacman m_Pacman;
    Ghost m_Ghostplayer;
    Sounds m_Sounds;

    QGraphicsPixmapItem* m_pMapItem;

    const QVector<QPoint> m_FoodballPositions;
    const QVector<QPoint> m_PowerballPositions;

    QMap<QString, QGraphicsEllipseItem*> m_FoodballGraphicalItemsTableMap;
    QMap<QString, QGraphicsEllipseItem*> m_PowerballGraphicalItemsTableMap;

    bool m_WaitingForRestartKey;
    bool m_RestartPending;

    GameState m_GameState;

    void SetGameState(GameState gameState);
    void GenerateMap();
    void PopulateMap();
    void AddSceneItems();
    void InitializeSounds();
    void PrepareGameToStart();
    void HideSceneItems();
    void RestartGame();
    void ShowSceneItems();

    void keyPressEvent(QKeyEvent *event);

    void ProcessGameDataPacket(QJsonObject& jsonObject);
    void ProcessCommandPacket(QJsonObject& jsonObject);
    void ProcessMessagePacket(QJsonObject& jsonObject);

    void RemoveBall(QString& coordinatesOfObjectToBeRemoved);

private slots:
    void StartGame();
    void CheckForRestartGameSignal();
    void Updater();
    void ProcessNewDataFromServer();
    void UpdateScene();
};
