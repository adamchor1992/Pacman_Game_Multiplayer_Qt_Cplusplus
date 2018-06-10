#ifndef TEXTSCREENMESSAGE_H
#define TEXTSCREENMESSAGE_H

#include <QMap>
#include <QDebug>
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>

class TextScreenMessage : public QGraphicsItem
{
private:
    QMap <QString,int> text_states;
    int current_text_state;

public:
    TextScreenMessage();

    void setScore(int _score);
    void setGameWon(bool _game_won);
    void setGameLost(bool _game_lost);
    void setTextState(QString _textstate);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool game_won, game_lost, playing;
    int score;
    int x,y,w,h;
};

#endif // TEXTSCREENMESSAGE_H
