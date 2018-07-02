#ifndef SOUNDS_H
#define SOUNDS_H

#include <QMediaPlayer>

class Sounds
{
public:
     /*! Initialize sounds as QMediaPlayer objects binded to resource files containing proper sounds */
     Sounds();
     /*! Sound played at beginning of game */
     QMediaPlayer beginning_sound;
     /*! Sound played when foodball is eaten */
     QMediaPlayer eat_sound;
     /*! Sound played when pacman eats ghost */
     QMediaPlayer eat_ghost_sound;
     /*! Sound played when pacman is eaten by ghost */
     QMediaPlayer pacman_death_sound;
};

#endif // SOUNDS_H
