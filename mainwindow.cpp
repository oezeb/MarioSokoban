#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_curr_level(0)
    , m_lock_start(0)
    , m_help(help())
{
    ui->setupUi(this);
    setWindowIcon(QIcon("files/icon.png"));
    loadLevels();
    if(m_lock_start <= 2)
        ui->continu->hide();

    m_area = new Area();
    m_win = new Win(this);
    m_loadLevel = new LoadLevelPage(ui->nextPage,ui->previousPage,4,6,m_lock_start);
    m_control = new ControlPanel();
    ui->SelectLevelPageLayout->addWidget(m_loadLevel,4,0,4,6);
    ui->AreaLayout->addWidget(m_control,5,0,4,4);
    ui->controlPanelText->setChecked(false);
    m_control->hide();

    connect(m_area,SIGNAL(levelCompleted()),this,SLOT(gameWin()));
    connect(m_area,SIGNAL(marioMoved()),this,SLOT(countMarioMoves()));
    connect(m_area,SIGNAL(boxMoved()),this,SLOT(countBoxMoves()));

    connect(m_win,SIGNAL(next_clicked()),this,SLOT(playNext()));
    connect(m_win,SIGNAL(retry_clicked()),this,SLOT(retry()));
    connect(m_win,SIGNAL(home_clicked()),this,SLOT(on_startBack_clicked()));

    connect(m_loadLevel,SIGNAL(pressed(int)),this,SLOT(play(int)));

    connect(m_control,SIGNAL(clicked(int)),m_area,SLOT(moveMario(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_win;
    delete m_area;
    QFile f("files/lock");
    f.open(QIODevice::WriteOnly | QFile::Text);
    QTextStream out(&f);
    out << m_lock_start;
    f.close();
}


void MainWindow::loadLevels()
{

    QFile f("files/level");
    if(!f.open(QIODevice::ReadOnly | QFile::Text))
        return;
    QTextStream in(&f);
    QString all(in.readAll());
    f.close();
    all.remove(QRegExp("'[.]+'"));
    QStringList list = all.split(QRegExp("Level [0-9]+"),Qt::SkipEmptyParts);
    for(int i = 0; i < list.size(); i++)
        m_levels.push_back(list[i].split("\n",Qt::SkipEmptyParts));

    // load lock start
    f.setFileName("files/lock");
    if(!f.open(QIODevice::ReadOnly | QFile::Text))
    {
        m_lock_start = 2;
        return;
    }
    in.skipWhiteSpace();
    in >> m_lock_start;
    f.close();
}

void MainWindow::play(int level)
{
    if(level <= 0 || level > m_levels.size())
        return;
    m_curr_level = level;
    m_area->setLevel(m_levels[level-1]);
    m_area->grabKeyboard();
    ui->AreaLayout->addWidget(m_area,0,5,10,4);
    ui->level->setNum(level);
    ui->moves->setNum(0);
    ui->pushes->setNum(0);
    ui->centralWindow->setCurrentWidget(ui->Area);
}

void MainWindow::playNext()
{
    play(m_curr_level+1);
}

void MainWindow::retry()
{
    play(m_curr_level);
}

void MainWindow::countMarioMoves()
{
    ui->moves->setNum(ui->moves->text().toInt()+1);
}

void MainWindow::countBoxMoves()
{
    ui->pushes->setNum(ui->pushes->text().toInt()+1);
}

void MainWindow::gameWin()
{
    int level = ui->level->text().toInt();
    if(level+1 >= m_lock_start)
    {
        m_lock_start = level+2;
        m_loadLevel->setLocked(m_lock_start);
        if(m_lock_start > 2 && ui->continu->isHidden())
            ui->continu->show();
    }

    m_win->display(level
                   , ui->moves->text().toInt()
                   , ui->pushes->text().toInt());
}

bool MainWindow::help()
{
    QFile f("files/help");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return true;
    QTextStream in(&f);
    in.skipWhiteSpace();
    int x;
    in >> x;
    return x;
}

void MainWindow::setHelp(bool state)
{
    QFile f("files/help");
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&f);
    int x = state;
    out << x;
    m_help = x;
}

void MainWindow::on_exit_clicked()
{
    qApp->quit();
}

void MainWindow::on_start_clicked()
{
    ui->centralWindow->setCurrentWidget(ui->StartPage);
}

void MainWindow::on_continu_clicked()
{
    play(m_lock_start-1);
}

void MainWindow::on_newGame_clicked()
{
    play(1);
    if(m_help)
    {
        QMessageBox::information(this
                                 , "move Keys"
                                 , "Use direction keys (UP, DOWN, LEFT, RIGHT) to make Mario move\n"
                                   "Use SHIFT or BACKSPACE to undo an action"
                                );
        setHelp(false);
    }
}

void MainWindow::on_selectLevel_clicked()
{
    ui->centralWindow->setCurrentWidget(ui->SelectLevelPage);
}

void MainWindow::on_startBack_clicked()
{
    ui->centralWindow->setCurrentWidget(ui->HomePage);
}

void MainWindow::on_areaBack_clicked()
{
    ui->centralWindow->setCurrentWidget(ui->StartPage);
}


void MainWindow::on_controlPanelText_toggled(bool checked)
{
    if(checked)
        m_control->show();
    else
        m_control->hide();
}

void MainWindow::on_selectLvlBack_clicked()
{
    ui->centralWindow->setCurrentWidget(ui->StartPage);
}
