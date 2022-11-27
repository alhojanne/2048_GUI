#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QLabel>
//#include <QtWidgets>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    number_squares = createNumberVector();
    labels_ = createLabelVector();
    scene_ = new QGraphicsScene(this);
    board_ = new GameBoard;

    // Timer to track how long the game has lasted so far:
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onTimeOut);

    // Disable game at the start
    disableGame();

    setFont();

    // Positions and width/height for QGraphicsScene and for calculating
    // size and position of each number square and label.
    int left_margin = 10;
    int top_margin = 100;
    int width = 500;
    int height = 400;

    ui->graphicsView->setGeometry(left_margin, top_margin,
                                  width + 2, height + 2);

    ui->graphicsView->setScene(scene_);
    scene_->setSceneRect(0, 0, width, height);

    // Initialize the board at the start of the program.
    board_->init_empty();
    board_->fill(seed_);

    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            // Coordinates for the get_item-method.
            Coords coord = std::make_pair(x,y);

            QGraphicsRectItem* numberSquare = new QGraphicsRectItem();

            numberSquare->setRect(QRect(width / SIZE * y,height / SIZE * x,
                                        width / SIZE, height / SIZE));
            scene_->addItem(numberSquare);

            // Store each square and pointer to the corresponding NumberTile-
            // object. Storing the pointer is probably pointless in the
            // current version of the program but I don't dare to remove it.
            number_squares[y].push_back({numberSquare,
                                         board_->get_item(coord)});

            // Labels and their size/position/value inside the numberSquares:
            QString value = QString::fromStdString(board_->get_item(coord)->
                                                   get_value());
            QLabel* label = new QLabel;
            label->setText(value);
            label->setGeometry(0,0, 400/SIZE, 350/SIZE);
            label->setAlignment(Qt::AlignCenter);
            QFont font = label->font();
            font.setPointSize(100/SIZE);
            label->setFont(font);

            // Setting the labels inside the numberSquares:
            QGraphicsProxyWidget* myProxy = new QGraphicsProxyWidget(numberSquare);
            myProxy->setWidget(label);
            myProxy->setPos(numberSquare->boundingRect().center()-label->
                            rect().center());

            // Storing labels to a 2D vector:
            labels_.at(y).at(x) = label;
        }
    }
}

MainWindow::~MainWindow()
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            delete number_squares.at(y).at(x).first;
            delete number_squares.at(y).at(x).second;
            delete labels_.at(y).at(x);
            labels_.at(y).at(x) = nullptr;
            number_squares.at(y).at(x).first = nullptr;
            number_squares.at(y).at(x).second = nullptr;
        }
    }
    delete ui;
    delete scene_; // Not sure if this is necessary..
}

void MainWindow::updateGUI()
{
    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            Coords coord = std::make_pair(x,y);
            QString value = QString::fromStdString(board_->get_item(coord)->get_value());
            labels_.at(y).at(x)->setText(value);
        }
    }
}

void MainWindow::endingGame()
{
    disableGame();

    if (has_Won_)
    {
        ui->end_Label->setText("You WON!");
        ui->end_Label->setStyleSheet("background-color : green;");
    }
    else
    {
        ui->end_Label->setText("You LOST!");
        ui->end_Label->setStyleSheet("background-color : red;");
    }
}

void MainWindow::disableGame()
{
    ui->up_Button->setDisabled(true);
    ui->right_Button->setDisabled(true);
    ui->down_Button->setDisabled(true);
    ui->left_Button->setDisabled(true);
}

void MainWindow::enableGame()
{
    ui->up_Button->setDisabled(false);
    ui->right_Button->setDisabled(false);
    ui->down_Button->setDisabled(false);
    ui->left_Button->setDisabled(false);
}

bool MainWindow::checkGoalValue(int goal_value)
{
    int goal = goal_value;
    if (goal == 0)
    {
        return false;
    }
    while (true)
    {
        if (goal == 1)
        {
            return true;
        }
        else if (goal % 2 != 0)
        {
            return false;
        }
        else
        {
            goal = goal/2;
        }
    }
}

void MainWindow::setFont()
{
    QFont font = ui->end_Label->font();
    font.setPointSize(50);
    ui->end_Label->setAlignment(Qt::AlignCenter);
    ui->end_Label->setFont(font);
}

void MainWindow::onTimeOut()
{
    ++timer_value_;

    ui->lcdNumber->display(timer_value_);
}

void MainWindow::on_up_Button_clicked()
{
    if (board_->move(UP, goal_))
    {
        updateGUI();
        has_Won_ = true;
        endingGame();
        return;
    }
    else if (board_->is_full())
    {
        has_Lost_ = true;
        endingGame();
        return;
    }
    board_->new_value(false);
    updateGUI();
}

void MainWindow::on_left_Button_clicked()
{
    if (board_->move(LEFT, goal_))
    {
        updateGUI();
        has_Won_ = true;
        endingGame();
        return;
    }
    else if (board_->is_full())
    {
        has_Lost_ = true;
        endingGame();
        return;
    }
    board_->new_value(false);
    updateGUI();
}

void MainWindow::on_down_Button_clicked()
{
    if (board_->move(DOWN, goal_))
    {
        updateGUI();
        has_Won_ = true;
        endingGame();
        return;
    }
    else if (board_->is_full())
    {
        has_Lost_ = true;
        endingGame();
        return;
    }
    board_->new_value(false);
    updateGUI();
}

void MainWindow::on_right_Button_clicked()
{
    if (board_->move(RIGHT, goal_))
    {
        updateGUI();
        has_Won_ = true;
        endingGame();
        return;
    }
    else if (board_->is_full())
    {
        has_Lost_ = true;
        endingGame();
        return;
    }
    board_->new_value(false);
    updateGUI();
}

void MainWindow::on_start_Button_clicked()
{
    timer_value_ = 0;
    ui->lcdNumber->display(timer_value_);
    timer->start(1000);


    QString goal_as_string = ui->goal_Value->text();
    if (goal_as_string == "")
    {
        goal_ = 2048;
    }
    else
    {
        goal_ = goal_as_string.toInt();
    }

    if (checkGoalValue(goal_))
    {
        enableGame();
        QString seed_as_string = ui->seed_Value->text();
        seed_ = seed_as_string.toInt();
        ui->start_Button->setDisabled(true);
        ui->seed_Value->setDisabled(true);
        ui->goal_Value->setDisabled(true);
        ui->end_Label->setText("");
        board_->fill(seed_);
        updateGUI();
    }
}

void MainWindow::on_reset_Button_clicked()
{
    timer->stop();
    disableGame();
    ui->start_Button->setDisabled(false);
    ui->seed_Value->setDisabled(false);
    ui->goal_Value->setDisabled(false);
    ui->end_Label->setText("Game Reseted");
    ui->end_Label->setStyleSheet("background-color : rgb(185,185,185);");
    board_->fill(seed_);
    updateGUI();
}

std::vector<std::vector<std::pair<QGraphicsRectItem*,
NumberTile*>>>createNumberVector()
{
    std::vector<std::vector<std::pair<QGraphicsRectItem*,
            NumberTile*>>> number_squares(SIZE,
            std::vector<std::pair<QGraphicsRectItem*,
            NumberTile*>>(SIZE,{nullptr, nullptr}));
    return number_squares;
}

std::vector<std::vector<QLabel*>> createLabelVector()
{
    std::vector<std::vector<QLabel*>> label_vector(SIZE,
    std::vector<QLabel*>(SIZE,nullptr));
    return label_vector;
}


