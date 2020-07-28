#ifndef LOADLEVELPAGE_H
#define LOADLEVELPAGE_H

#include <game/label.h>

class LoadLevelPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoadLevelPage(QPushButton* next
                           , QPushButton* previous
                           , int rows
                           , int columns
                           , int lock_start = -1
                           , int  lock_end = -1
                           , QWidget *parent = nullptr
                          );

    void init();

    bool setNumb(int start);

    void setLocked(int start, int end = -1);

private:

    void boxLook(QLabel* box);

public slots:

    void next();

    void previous();

private:

    QList < QList <Label*>> m_labels;

    QPushButton* m_next;

    QPushButton* m_previous;

    int m_rows;

    int m_columns;

    int m_locked_start;

    int m_locked_end;

    int m_page;

    int m_page_count; // number of elements in one page

    int m_max_page;

    int m_totalLevels;


public slots:

    void press(Label* lab);

signals:

    void pressed(int level);
};

#endif // LOADLEVELPAGE_H
