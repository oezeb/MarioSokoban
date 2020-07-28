#include "label.h"

Label::Label(QWidget* parent) : QLabel(parent), m_text("")
{
}

Label::Label(QString text,  QWidget* parent) : QLabel(parent), m_text(text)
{
    setFixedSize(_WIDTH_,_HEIGHT_);
    setPixmap();
}

Label::Label(QString text, int x, int y,  QWidget* parent) : QLabel(parent), m_text(text)
{
    setFixedSize(_WIDTH_,_HEIGHT_);
    move(x,y);
    setPixmap();
}



void Label::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit pressed();
        emit pressed(this);
    }
}

void Label::move(const QPoint& pos)
{
    move(pos.x(),pos.y());
}

void Label::move(int ax, int ay)
{
    QLabel::move(ax*_WIDTH_,ay*_HEIGHT_);
}


void Label::setText(const QString &text)
{
    m_text = text;
    setPixmap();
}
void Label::setPixmap()
{
    if(m_text == "@" || m_text == "+") {
        QLabel::setPixmap(QPixmap("files/MarioDown.gif"));
    } else if (m_text == "$") {
       QLabel::setPixmap(QPixmap("files/Box.jpg"));
    } else if (m_text == "#") {
        QLabel::setPixmap(QPixmap("files/Wall-4.png"));
    } else if (m_text == ".") {
        QLabel::setPixmap(QPixmap("files/Target.png"));
    } else if (m_text == "*" || m_text == "TB") {
        QLabel::setPixmap(QPixmap("files/BoxOk.jpg"));
    } else if (m_text == "%") {
        QLabel::setPixmap(QPixmap("files/grass.jpg"));
    }
    show();
}

void Label::marioDirections(Label* lab, int key)
{
    switch (key) {
    default:
        return;
    case Qt::Key_Up:
        lab->QLabel::setPixmap(QPixmap("files/MarioUp.gif"));
        break;
    case Qt::Key_Down:
        lab->QLabel::setPixmap(QPixmap("files/MarioDown.gif"));
        break;
    case Qt::Key_Right:
        lab->QLabel::setPixmap(QPixmap("files/MarioRight.gif"));
        break;
    case Qt::Key_Left:
        lab->QLabel::setPixmap(QPixmap("files/MarioLeft.gif"));
        break;
    }
}
