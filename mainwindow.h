#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game/area.h"
#include "game/win.h"
#include "game/controlpanel.h"
#include "loadlevelpage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:

    void loadLevels();

    void setHelp(bool state);

    bool help();

public slots:

    void play(int level);

    void playNext();

    void retry();

    void countMarioMoves();

    void countBoxMoves();

    void gameWin();

    void on_start_clicked();

    void on_exit_clicked();

    void on_continu_clicked();

    void on_newGame_clicked();

    void on_selectLevel_clicked();

    void on_startBack_clicked();

    void on_areaBack_clicked();

    void on_controlPanelText_toggled(bool checked);

    void on_selectLvlBack_clicked();


private:
    Ui::MainWindow *ui;

    QList <QStringList> m_levels;

    int m_curr_level;

    int m_lock_start;

    bool m_help;

    Area* m_area;

    Win* m_win;

    ControlPanel* m_control;

    LoadLevelPage* m_loadLevel;

};
#endif // MAINWINDOW_H
