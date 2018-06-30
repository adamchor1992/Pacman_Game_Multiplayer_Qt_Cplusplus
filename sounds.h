#ifndef SOUNDS_H
#define SOUNDS_H

#include <QMediaPlayer>

class Sounds
{
public:
     Sounds();

     QMediaPlayer beginning_sound;
     QMediaPlayer eat_sound1;
     QMediaPlayer eat_sound2;
     QMediaPlayer eat_ghost_sound;
     QMediaPlayer pacman_death_sound;
     QMediaPlayer pacman_siren;
};

#endif // SOUNDS_H
