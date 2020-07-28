#ifndef LABEL_H
#define LABEL_H

#include <QtWidgets>

const int _WIDTH_(46);
const int _HEIGHT_(46);

class Label : public QLabel
{
    Q_OBJECT
public:

    Label(QWidget* parent = nullptr);

    Label(QString text, QWidget* parent = nullptr);

    Label(QString text, int x, int y, QWidget* parent = nullptr);

    QString text() const {return m_text; }

    void setText(const QString &text);

    void setPixmap();

    void mousePressEvent(QMouseEvent *ev) override;

    void move(const QPoint& pos);

    void move(int ax, int ay);

    static void marioDirections(Label* lab, int key);


private:

    QString m_text;

signals:

    void pressed(Label*);

    void pressed();
};

#endif // LABEL_H
