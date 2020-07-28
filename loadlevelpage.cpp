#include "loadlevelpage.h"

LoadLevelPage::LoadLevelPage(QPushButton* next
                             , QPushButton* previous
                             , int rows
                             , int columns
                             , int lock_start
                             , int  lock_end
                             , QWidget *parent)
    : QWidget(parent)
    , m_next(next)
    , m_previous(previous)
    , m_rows(rows)
    , m_columns(columns)
    , m_locked_start(lock_start)
    , m_locked_end(lock_end)
    , m_page(1)
    , m_page_count(rows*columns)
    , m_max_page(0)
    , m_totalLevels(0)
{
    init();
    QObject::connect(m_previous,SIGNAL(clicked()),this,SLOT(previous()));
    QObject::connect(m_next,SIGNAL(clicked()),this,SLOT(next()));
}

void LoadLevelPage::init()
{
    QFile f("files/level");
    if(!f.open(QIODevice::ReadOnly | QFile::Text))
        return;
    QTextStream in(&f);
    QString all = in.readAll();
    f.close();

    m_totalLevels  = all.count("Level");
    int count(m_page_count);
    m_max_page = m_totalLevels%count == 0 ? m_totalLevels/count : m_totalLevels/count+1;
    count = 0;
    QGridLayout* gLayout = new QGridLayout(this);
    for(int i = 0; i < m_rows; i++)
    {
        QList <Label*> tmp;
        m_labels.push_back(tmp);
        for(int j = 0; j < m_columns; j++)
        {
            Label* lab = new Label();
            count++;
            if(m_locked_start < 0
                    || (m_locked_start >= 0 && count < m_locked_start)
                    || (m_locked_end >=0 && count > m_locked_end)
                    )
                lab->QLabel::setNum(count);
            else
            {
                lab->QLabel::setNum(0);
                lab->QLabel::setPixmap(QPixmap("files/Lock-icon.png"));
            }

            m_labels[i].push_back(lab);
            gLayout->addWidget(lab,i+1,j);

            boxLook(lab);
            QObject::connect(lab,SIGNAL(pressed(Label*)),this,SLOT(press(Label*)));
        }
    }
    setLayout(gLayout);
    m_previous->hide();
}

void LoadLevelPage::press(Label* lab)
{
    emit pressed(lab->QLabel::text().toInt());
}

void LoadLevelPage::next()
{
    int start(m_page*m_page_count+1);
    if(start <= 0 || start > m_totalLevels)
        return;
    if(setNumb(start))
    {
        if(m_page == 1)
            m_previous->show();
        if(m_page == m_max_page-1)
            m_next->hide();
        m_page++;
    }
}

void LoadLevelPage::previous()
{
    int start((m_page-2)*m_page_count+1);
    if(start <= 0 || start > m_totalLevels)
        return;
    if(setNumb(start))
    {
        if(m_page == m_max_page)
            m_next->show();
        if(m_page == 2)
            m_previous->hide();
        m_page--;
    }
}

bool LoadLevelPage::setNumb(int start)
{

    if(start > m_totalLevels)
        return false;
    int page(start/m_page_count +1);
    for(int i = 0; i < m_rows && i < m_labels.size(); i++)
    {
        for(int j = 0; j < m_columns && j < m_labels[i].size(); j++)
        {
            if(start > m_totalLevels+1)
                m_labels[i][j]->setEnabled(false);
            if(page == m_max_page-1
                    && !m_labels[i][j]->isEnabled())
                m_labels[i][j]->setEnabled(true);

            QLabel* lab = m_labels[i][j];
            if(m_locked_start < 0
                    || (m_locked_start >= 0 && start < m_locked_start)
                    || (m_locked_end >=0 && start > m_locked_end)
                    )
                lab->QLabel::setNum(start);
            else
            {
                lab->QLabel::setNum(0);
                lab->QLabel::setPixmap(QPixmap("files/Lock-icon.png"));
            }
            start++;
        }
    }
    return  true;
}

void LoadLevelPage::boxLook(QLabel *box)
{
    box->setFrameShape(QFrame::Box);
    box->setFrameShadow(QFrame::Raised);
    box->setLineWidth(3);
    box->setMidLineWidth(3);
    box->setAlignment(Qt::AlignCenter);
}

void LoadLevelPage::setLocked(int start, int end)
{
    m_locked_start = start;
    m_locked_end = end;
    setNumb(m_labels[0][0]->QLabel::text().toInt());
}
