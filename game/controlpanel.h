#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

/**
 * @author KingEze
 * @date 2020/06/17
 * @class Controller
 * push button for controlling Mario
 */

#include <game/label.h>

/// ste fixed size for all button
const QSize ButtonSize(40,40);
const QSize WinSize(3*ButtonSize.width()+10,3*ButtonSize.height()+10);

class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    /**
     * @class Methods
     */

    /// @class Constructor
    ControlPanel(QWidget* parent = nullptr);

    /// @class Destructor
    ~ControlPanel();

    void createButtons();

    void connectSignals() const;

public slots:

    void up() { emit clicked(Qt::Key_Up); }

    void right() { emit clicked(Qt::Key_Right); }

    void left()  { emit clicked(Qt::Key_Left); }

    void down() { emit clicked(Qt::Key_Down); }

    void undo() {
        emit clicked(Qt::Key_Backspace);
        emit clicked(Qt::Key_Shift);
    }

signals:

    void clicked(int);

private:
    Label* m_up;
    Label* m_right;
    Label* m_left;
    Label* m_down;
    Label* m_undo;

};

#endif // CONTROLPANEL_H
