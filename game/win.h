#ifndef WIN_H
#define WIN_H

#include <QDialog>
#include <QtWidgets>

namespace Ui {
class Win;
}

class Win : public QDialog
{
    Q_OBJECT

public:
    explicit Win(QWidget *parent = nullptr);

    ~Win();


public slots:
    void display(int level = -1, int moves = -1, int pushes = -1);

    void on_next_clicked();

    void on_retry_clicked();

    void on_home_clicked();

signals:

    void next_clicked();

    void retry_clicked();

    void home_clicked();

private:
    Ui::Win *ui;
};

#endif // WIN_H
