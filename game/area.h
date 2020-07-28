#ifndef AREA_H
#define AREA_H

#include "label.h"

class Area : public QWidget
{
    Q_OBJECT
public:

    explicit Area(QStringList level = QStringList(), QWidget *parent = nullptr);

    ~Area();

    void initBackGround();

    void initLevel();

    bool spaceInside(int i, int j);

    void clearBackground();

    QPoint destination(int x, int y, int key) const;

    QPoint destination(QPoint curr_pos, int key) const;

    bool moveBox(QPoint box_pos, int key);

    bool moveBox(int curr_x, int curr_y, int key);

    void clearLabels();

    void move(Label* lab, const QPoint& curr_pos, const QPoint& dest);

    void move(Label* lab, int x0, int y0, int x1, int y1);

    void keyPressEvent(QKeyEvent *event) override;

    void levelComplete();

    void saveSteps();

    void undo();

public slots:

    bool moveMario(int key);

    void setLevel(const QStringList& level);

signals:

    void levelCompleted();

    void boxMoved();

    void marioMoved();

private:

    QStringList m_level;

    QList <QList <Label*>> m_labels;

    QList <Label*> m_background;

    QPoint  m_mario_pos;

};

#endif // AREA_H
