#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH
#include "gameboard.hh"
#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QLabel>

class GameBoard;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

std::vector<std::vector<std::pair<QGraphicsRectItem*, NumberTile*>>> createNumberVector();
std::vector<std::vector<QLabel*>> createLabelVector();
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Updates the squares with new values.
    void updateGUI();

    // Ends the game if player has won or lost.
    void endingGame();

    // Disables the directional buttons if game hasn't started.
    void disableGame();

    // Enables the directional buttons if the game has started.
    void enableGame();

    // Function for checking that the goal value is power of 2.
    bool checkGoalValue(int goal_value);

    // Set the font for the label below the game grid.
    void setFont();

private slots:

    void onTimeOut();

    void on_up_Button_clicked();

    void on_left_Button_clicked();

    void on_down_Button_clicked();

    void on_right_Button_clicked();

    void on_start_Button_clicked();

    void on_reset_Button_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene_;
    std::vector<std::vector<std::pair<QGraphicsRectItem*, NumberTile*>>> number_squares;
    GameBoard* board_;
    std::vector<std::vector<QLabel*>> labels_;
    bool has_Won_ = false;
    bool has_Lost_ = false;
    int goal_ = 2048;
    int seed_ = 0;
    int timer_value_ = 0;
    QTimer* timer;
};
#endif // MAINWINDOW_HH
