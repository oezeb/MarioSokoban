#include "area.h"

Area::Area(QStringList level, QWidget *parent) :
    QWidget(parent)
  , m_level(level)
  , m_mario_pos(QPoint(0,0))
{
    if(QFile("files/steps").exists())
        QFile::remove("files/steps");
    initBackGround();
    initLevel();
}

Area::~Area()
{
    if(QFile("files/steps").exists())
        QFile::remove("files/steps");
}

void Area::setLevel(const QStringList& level)
{
    m_level = level;
    if(QFile("files/steps").exists())
        QFile::remove("files/steps");
    initBackGround();
    initLevel();
}

void Area::initBackGround()
{
    clearBackground();
    for(int i = 0; i < m_level.size(); i++)
        for(int j = 0; j < m_level[i].size(); j++)
            if(m_level[i][j] != ' '
                    || (m_level[i][j] == ' ' && spaceInside(i,j))
                    )
                m_background.push_back( new Label("%",j,i,this));
}

bool Area::spaceInside(int ax, int ay)
{
    int count(0);
    int x(ax);
    int y(ay);
    while(x >= 0 && m_level[x][y] == ' ')
        x--;
    if(x>=0)
        count++;
    x = ax;
    while (y >= 0 && m_level[x][y] == ' ')
        y--;
    if(y >= 0)
        count++;
    y = ay;
    while(x < m_level.size() && m_level[x][y] == ' ')
        x++;
    if(x < m_level.size())
        count++;
    x = ax;
    while(y < m_level[x].size() && m_level[x][y] == ' ')
        y++;
    if(y < m_level[x].size())
        count++;
    if(count == 4)
        return  true;
    return false;
}

void Area::clearBackground()
{
    for(int i = 0; i < m_background.size(); i++)
        delete m_background[i];
    m_background.clear();
}

void Area::initLevel()
{
    clearLabels();
    int h = m_level.size();
    int w(0);
    for(int i = 0; i < h; i++)
    {
        int k = m_level[i].size();
        if(w < k)
            w = k;
        QList <Label*> tmp;
        m_labels.push_back(tmp);
        for(int j = 0; j < k; j++)
        {
            if(m_level[i][j] == ' ')
                m_labels[i].push_back(nullptr);
            else
            {
                m_labels[i].push_back(new Label(QString(m_level[i][j]),j,i,this));
                if(m_level[i][j] == '@' || m_level[i][j] == '+')
                    m_mario_pos = QPoint(i,j);
            }
        }
    }
    this->setFixedSize(w*_WIDTH_,h*_HEIGHT_);
    saveSteps();
}

QPoint Area::destination(QPoint curr_pos, int key) const
{
    return destination(curr_pos.x(),curr_pos.y(),key);
}

QPoint Area::destination(int x, int y, int key) const
{
    switch (key) {
    default:
        break;
    case Qt::Key_Up:
        return QPoint(x-1,y);
    case Qt::Key_Down:
        return QPoint(x+1,y);
    case Qt::Key_Right:
        return QPoint(x,y+1);
    case Qt::Key_Left:
        return QPoint(x,y-1);
    }
    return QPoint(-1,-1);
}

void Area::clearLabels()
{
    for(int i = 0; i < m_labels.size(); i++)
        for (int j = 0; j < m_labels[i].size(); j++)
            if(m_labels[i][j] != nullptr)
                delete m_labels[i][j];
    m_labels.clear();
}

void Area::move(Label *lab, const QPoint &curr_pos, const QPoint &dest)
{
    move(lab,curr_pos.x(),curr_pos.y(),dest.x(),dest.y());
}

void Area::move(Label *lab, int x0, int y0, int x1, int y1)
{
    if(lab == nullptr)
    {
        m_level[x1][y1] = ' ';
        m_labels[x1][y1] = lab;
        return;
    }

    if(lab->text().size() < 0)
        return;
    if(m_labels[x1][y1] != nullptr)
        delete m_labels[x1][y1];
    m_level[x1][y1] = lab->text().at(0);
    lab->move(y1,x1);
    m_labels[x1][y1] = lab;
    if(lab->text() == "+" || lab->text() == "@")
    {
        m_mario_pos = QPoint(x1,y1);
        emit marioMoved();
    }
    if(lab->text() == "$" || lab->text() == "*")
        emit boxMoved();

    if(x0 >= 0 && y0 >= 0)
    {
        m_level[x0][y0] = ' ';
        m_labels[x0][y0] = nullptr;
    }
}

bool Area::moveBox(QPoint box_pos, int key)
{
    return moveBox(box_pos.x(),box_pos.y(),key);
}

bool Area::moveBox(int x, int y, int key)
{
    QPoint dest(destination(x,y,key));
    if(dest.x() < 0 || dest.y() < 0)
        return false;
    uchar ch = m_level[dest.x()][dest.y()].cell();
    switch (ch) {
    case ' ':
        saveSteps();
        if(m_level[x][y] == '$') {
            move(m_labels[x][y],QPoint(x,y),dest);
            move(nullptr,-1,-1,x,y);
        }
        else if(m_level[x][y] == '*') {
            m_level[x][y] = '$';
            m_labels[x][y]->setText("$");
            move(m_labels[x][y],QPoint(x,y),dest);
            move(new Label(".",this),-1,-1,x,y);
        }
        return true;
    case '.':
        saveSteps();
        if(m_level[x][y] == '*') {
            move(m_labels[x][y],QPoint(x,y),dest);
            move(new Label(".",this),-1,-1,x,y);
        } else if (m_level[x][y] == '$') {
            m_level[x][y] = '*';
            m_labels[x][y]->setText("*");
            move(m_labels[x][y],QPoint(x,y),dest);
            move(nullptr,-1,-1,x,y);
        }
        return true;;
    }
    return false;
}

bool Area::moveMario(int key)
{

    if(key == Qt::Key_Backspace || key == Qt::Key_Shift)
    {
        undo();
        return false;
    }
    QPoint dest(destination(m_mario_pos,key));
    if(dest.x() < 0 || dest.y() < 0)
        return false;
    int x = m_mario_pos.x();
    int y = m_mario_pos.y();

    bool moved = false;
    uchar ch = m_level[dest.x()][dest.y()].cell();
    switch (ch) {
    case ' ':
        if(m_level[x][y] == '@') {
            move(m_labels[x][y],QPoint(x,y),dest);
            move(nullptr,-1,-1,x,y);
        }
        else if(m_level[x][y] == '+') {
            m_level[x][y] = '@';
            m_labels[x][y]->setText("@");
            move(m_labels[x][y],QPoint(x,y),dest);
            move(new Label(".",this),-1,-1,x,y);
        }
        Label::marioDirections(m_labels[m_mario_pos.x()][m_mario_pos.y()],key);
        return true;
    case '.':
        if(m_level[x][y] == '+') {
            move(m_labels[x][y],QPoint(x,y),dest);
            move(new Label(".",this),-1,-1,x,y);
        } else if (m_level[x][y] == '@') {
            m_level[x][y] = '+';
            m_labels[x][y]->setText("+");
            move(m_labels[x][y],QPoint(x,y),dest);
            move(nullptr,-1,-1,x,y);
        }
        Label::marioDirections(m_labels[m_mario_pos.x()][m_mario_pos.y()],key);
        return true;
    case '$':
    case '*':
        Label::marioDirections(m_labels[m_mario_pos.x()][m_mario_pos.y()],key);
        if(!moveBox(dest,key))
            return false;
        moved = moveMario(key);
        levelComplete();
        return moved;
    }
    Label::marioDirections(m_labels[m_mario_pos.x()][m_mario_pos.y()],key);
    return false;
}

void Area::levelComplete()
{
    int count_1(0);
    int count_2(0);
    for(int i = 0; i < m_level.size(); i++)
    {
        count_1 += m_level[i].count('.');
        count_2 += m_level[i].count('+');
    }
    if(count_1 == 0 && count_2 == 0)
        emit levelCompleted();
}

void Area::saveSteps()
{
    QFile f("files/steps");
    f.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&f);
    out << "step\n";
    for(int i = 0; i < m_level.size(); i++)
        out << m_level[i] + "\n";
    f.close();
}

void Area::undo()
{
    QFile f("files/steps");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&f);
    /*
     *  getting previous step
     */
    QString all(in.readAll());
    f.close();
    // get prev step string
    int idx = all.lastIndexOf("step\n");
    QString str = all.mid(idx);
    if(idx > 0)
        all.remove(str);
    str.remove("step\n");
    m_level = str.split('\n',Qt::SkipEmptyParts);
    // save new file steps
    f.open(QIODevice::WriteOnly | QFile::Text);
    QTextStream out(&f);
    out << all;
    // update window
    for(int i = 0; i < m_level.size(); i++)
    {
        for(int j = 0; j < m_level[i].size(); j++)
        {
            QChar ch = m_level[i][j];
            Label* lab = m_labels[i][j];
            if(lab == nullptr)
            {
                if(ch != ' ')
                {
                    m_labels[i][j] = new Label(ch,j,i,this);
                    if(ch == '@' || ch == '+')
                        m_mario_pos = QPoint(i,j);
                }
                continue;
            }
            else if(ch == lab->text())
                continue;
            if(m_level[i][j] == ' ')
            {
                delete m_labels[i][j];
                m_labels[i][j] = nullptr;
            }
            else
            {
                delete m_labels[i][j];
                m_labels[i][j] = new Label(ch,j,i,this);
                if(ch == '@' || ch == '+')
                    m_mario_pos = QPoint(i,j);
            }
        }
    }
}

void Area::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Shift)
        undo();
    else
        moveMario(event->key());
}
