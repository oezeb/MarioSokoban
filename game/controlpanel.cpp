#include "controlpanel.h"

/**
 * @class Methods
 */

/// Constuctor
ControlPanel::ControlPanel(QWidget* parent) : QWidget(parent)
{
    createButtons();
    connectSignals();
}

ControlPanel::~ControlPanel()
{
    delete m_up;
    delete m_right;
    delete m_left;
    delete m_down;
    delete m_undo;
}

void ControlPanel::createButtons()
{
    m_up = new Label();
    m_right = new Label();
    m_left = new Label();
    m_down = new Label();
    m_undo = new Label();

    m_up->setFixedSize(ButtonSize);
    m_right->setFixedSize(ButtonSize);
    m_left->setFixedSize(ButtonSize);
    m_down->setFixedSize(ButtonSize);
    m_undo->setFixedSize(ButtonSize);

    m_up->QLabel::setPixmap(QPixmap("files/up.png"));
    m_right->QLabel::setPixmap(QPixmap("files/right.png"));
    m_left->QLabel::setPixmap(QPixmap("files/left.png"));
    m_down->QLabel::setPixmap(QPixmap("files/down.png"));
    m_undo->QLabel::setPixmap(QPixmap("files/undo.png"));

    setFixedSize(WinSize);

    QGridLayout* gLayout = new QGridLayout(this);
    gLayout->addWidget(m_up,0,1);
    gLayout->addWidget(m_right,1,2);
    gLayout->addWidget(m_left,1,0);
    gLayout->addWidget(m_down,2,1);
    gLayout->addWidget(m_undo,1,1);
}

void ControlPanel::connectSignals() const
{
    QObject::connect(m_up,SIGNAL(pressed()),this,SLOT(up()));
    QObject::connect(m_right,SIGNAL(pressed()),this,SLOT(right()));
    QObject::connect(m_left,SIGNAL(pressed()),this,SLOT(left()));
    QObject::connect(m_down,SIGNAL(pressed()),this,SLOT(down()));
    QObject::connect(m_undo,SIGNAL(pressed()),this,SLOT(undo()));

}
