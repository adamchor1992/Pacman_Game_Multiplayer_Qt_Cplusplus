#include "textscreenmessage.h"

TextScreenMessage::TextScreenMessage()
{
    game_won=false;
    game_lost=false;

    text_states.insert("start", 1);
    text_states.insert("paused", 2);
    text_states.insert("end_pacman", 3);
    text_states.insert("end_ghost", 4);
    text_states.insert("disconnected", 5);
    text_states.insert("null", 6);

    current_text_state = 0;
    setTextState("start");

    w=600;
    h=200;
    x=614/2-w/2;
    y=714/2-h/2;

    score=0;
    playing=false;
}

QRectF TextScreenMessage::boundingRect() const
{
    return QRect(x, y, w, h);
}

void TextScreenMessage::setScore(int _score)
{
    this -> score = _score;
}

void TextScreenMessage::setGameWon(bool _game_won)
{
    this -> game_won= _game_won;
}

void TextScreenMessage::setGameLost(bool _game_lost)
{
    this -> game_lost= _game_lost;
}

void TextScreenMessage::setTextState(QString _textstate)
{
    if(text_states.contains(_textstate))
    {
        current_text_state = text_states.value(_textstate);
    }
    else
    {
        qDebug() << "Wrong text state";
    }
}

void TextScreenMessage::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::red);
    painter->setPen(pen);
    QFont font=painter->font();
    font.setPointSize (33);
    painter->setFont(font);

    switch(current_text_state)
    {
    case 1:
        painter->drawText(boundingRect(),Qt::AlignCenter, "GET READY AND PRESS SPACE");
        break;
    case 2:
        painter->drawText(boundingRect(),Qt::AlignCenter, "PAUSED");
        break;
    case 3:
        painter->drawText(boundingRect(),Qt::AlignCenter, "PACMAN WINS");
        break;
    case 4:
        painter->drawText(boundingRect(),Qt::AlignCenter, "GHOST WINS");
        break;
    case 5:
        painter->drawText(boundingRect(),Qt::AlignCenter, "DISCONNECTED");
        break;
    case 6:
        painter->drawText(boundingRect(),Qt::AlignCenter, "");
        break;
    default:
        painter->drawText(boundingRect(),Qt::AlignCenter, "WRONG TEXT STATE");
        break;
    }

//    if(!playing)
//    {
//        if(game_won)
//        {
//            painter->drawText(boundingRect().x(),boundingRect().y(), "  CONGRATULATIONS");
//            painter->drawText(boundingRect(),Qt::AlignCenter, "FINAL SCORE : "+QString::number(score));
//        }
//        else if(game_lost)
//        {
//            painter->drawText(boundingRect().x(),boundingRect().y(), "YOU LOST, GAME OVER");
//            painter->drawText(boundingRect(),Qt::AlignCenter, "FINAL SCORE : "+QString::number(score));
//        }
//        else
//        {
//            painter->drawText(boundingRect(),Qt::AlignCenter, "PRESS SPACE TO START");
//        }
//    }
}
