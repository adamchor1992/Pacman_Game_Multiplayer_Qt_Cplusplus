#include "sounds.h"

Sounds::Sounds()
{
    m_BeginningSound.setMedia(QUrl("qrc:/sounds/pacman_beginning.wav"));
    m_EatSound.setMedia(QUrl("qrc:/sounds/pacman_eat.wav"));
    m_EatGhostSound.setMedia(QUrl("qrc:/sounds/pacman_eatghost.wav"));
    m_PacmanDeathSound.setMedia(QUrl("qrc:/sounds/pacman_death.wav"));
}
