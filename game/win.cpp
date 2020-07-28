#include "win.h"
#include "ui_win.h"

Win::Win(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Win)
{
    ui->setupUi(this);
}

Win::~Win()
{
    delete ui;
}

void Win::display(int level, int moves, int pushes)
{
    if(level < 0)
        ui->level->hide();
    if(moves < 0)
        ui->moves->hide();
    if(pushes < 0)
        ui->pushes->hide();
    ui->level->setText("Level " + QString::number(level));
    ui->moves->setText("Mario Moves Number " + QString::number(moves));
    ui->pushes->setText("Box Push Number " + QString::number(pushes));
    show();
}

void Win::on_next_clicked()
{
    emit next_clicked();
    hide();
}

void Win::on_retry_clicked()
{
    emit retry_clicked();
    hide();
}

void Win::on_home_clicked()
{
    emit home_clicked();
    hide();
}
