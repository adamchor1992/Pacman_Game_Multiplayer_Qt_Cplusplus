#pragma once

#include <QMediaPlayer>

class Sounds
{
public:
     Sounds();
     QMediaPlayer m_BeginningSound;
     QMediaPlayer m_EatSound;
     QMediaPlayer m_EatGhostSound;
     QMediaPlayer m_PacmanDeathSound;
};
