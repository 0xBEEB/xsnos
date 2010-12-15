///////////////////////////////////////////////////////////////////////
// Copyright © 2010 Thomas Schreiber <ubiquill@gmail.com>
//
// This file is part of xsnos.
//
// Xsnos is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option) any later version.
//
// Xsnos is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with xsnos.  If not, see <http://www.gnu.org/licenses/>.
///////////////////////////////////////////////////////////////////////

#include <QtGui>
#include <iostream>
#include <stdlib.h>

#include "MainWindow.hh"
#include "PiecesList.hh"
#include "GameSpace.hh"

///////////////////////////////////////////////////////////////////////
// MainWindow(QWidget *parent)
//
// Parameters:  QWidget     *parent
//
// Constructor
///////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // Initialize the gamespaces
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            space[i][j] = new GameSpace(i, j);
        }
    }

    move = new QVector<Move>;

    // Create all the widgets and start a new game
    setupWidgets();

    // One size fits all
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));

    // Should titles be translatable?
    setWindowTitle(tr("Xs-n-Os"));
}

///////////////////////////////////////////////////////////////////////
// printMoves()
//
// Prints the moves taken during the current game. Uses the format:
// {Player}->({x-coordinate},{y-coordinate})
// A ':' delimits each move and a new line is used to separate games.
///////////////////////////////////////////////////////////////////////
void
MainWindow::printMoves()
{
    char piece;

    for(int i = 0; i < move->size(); i++) {
        Move cur = move->at(i);
        if (cur.state == GameSpace::X) 
            piece = 'X';
        else
            piece = 'O';
        
        std::cout << piece << "->(" << cur.row << "," << cur.col << ")";

        if (&(move->at(i)) != &(move->last()))
            std::cout << ":";
    }

    std::cout << "\n";
}

///////////////////////////////////////////////////////////////////////
// newGame()
//
// Clears everything and resets all states back to how they are at
// first execution.
///////////////////////////////////////////////////////////////////////
void
MainWindow::newGame()
{
    move->clear();

    x_pieces_list->clear();
    o_pieces_list->clear();

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            space[i][j]->clear();
        }
    }

    QString x_image_file_name = ":/images/x.png";
    QString o_image_file_name = ":/images/o.png";

    QPixmap x_image;
    x_image.load(x_image_file_name);
    
    QPixmap o_image;
    o_image.load(o_image_file_name);

    for (int i = 0; i < 6; ++i) {
        x_pieces_list->createPiece(x_image, true);
        o_pieces_list->createPiece(o_image, false);
    }

    changeTurnToX();

}

///////////////////////////////////////////////////////////////////////
// movePlayed(bool is_x, int row, int col)
//
// Parameters:  bool        is_x        Was the piece played an X?
//              int         row         What row was the piece played
//              int         col         what col was the piece played
//
// This checks to see if the game has ended. It then acts appropriately
// depending on determined state of the game. The bool is a bit of a 
// kludge, and I'm sorry for it. Unfortunately, Qt would not queue
// signals with my GameSpace::SpaceState enum as a parameter, and it
// resulted in a race condition bug. The bool was my fix for this.
///////////////////////////////////////////////////////////////////////
void
MainWindow::movePlayed(bool is_x, int row, int col)
{
    // Get the info from the last move, and log it
    Move lastmove;
    lastmove.row   = row;
    lastmove.col   = col;

    if (is_x) {
        changeTurnToO();
        lastmove.state = GameSpace::X;
    }
    if (!is_x) {
        changeTurnToX();
        lastmove.state = GameSpace::O;
    }

    move->push_back(lastmove);

    if (lastmove.state == GameSpace::X) {
        if (checkXWin()) {
            // This creates a message box that alerts the user that X won
            QMessageBox *x_box = new QMessageBox(this);
            QString x_wins_pixmap_file_name = ":/images/x_wins.png";

            QPixmap x_wins_pixmap;
            x_wins_pixmap.load(x_wins_pixmap_file_name);
            x_box->setIconPixmap(x_wins_pixmap);
            x_box->setText(tr("X Wins!"));
            x_box->setStandardButtons(QMessageBox::Ok);
            x_box->exec();
            
            delete x_box;

            printMoves();

            newGame();
            return;
        }

    } else {
        if (checkOWin()) {
            // This message box alerts the user that O won
            QMessageBox *o_box = new QMessageBox(this);
            QString o_wins_pixmap_file_name = ":/images/o_wins.png";

            QPixmap o_wins_pixmap;
            o_wins_pixmap.load(o_wins_pixmap_file_name);
            o_box->setIconPixmap(o_wins_pixmap);
            o_box->setText(tr("O Wins!"));
            o_box->setStandardButtons(QMessageBox::Ok);
            o_box->exec();
            
            delete o_box;

            printMoves();

            newGame();
            return;
        }
    }

    // If we did find a draw do the following
    if (checkDraw()) {
        //This messagebox alerts the user there is a draw
        QMessageBox *draw_box             = new QMessageBox(this);
        QString     draw_pixmap_file_name = ":/images/draw.png";
        QPixmap     draw_pixmap;

        draw_pixmap.load(draw_pixmap_file_name);
        draw_box->setIconPixmap(draw_pixmap);
        draw_box->setText(tr("Draw!"));
        draw_box->setStandardButtons(QMessageBox::Ok);
        draw_box->exec();
            
        delete draw_box;

        printMoves();
        newGame();
    }

}

///////////////////////////////////////////////////////////////////////
// checkXwin()
//
// Checks if X has won.
///////////////////////////////////////////////////////////////////////
bool
MainWindow::checkXWin()
{
    // Check to see if X wins
    for (int i = 0; i < 3; i++) {
        if (   (space[i][0]->getState() == GameSpace::X 
            &&  space[i][1]->getState() == GameSpace::X  // Rows
            &&  space[i][2]->getState() == GameSpace::X)
            || (space[0][i]->getState() == GameSpace::X 
            &&  space[1][i]->getState() == GameSpace::X  // Cols
            &&  space[2][i]->getState() == GameSpace::X)
            || (space[0][0]->getState() == GameSpace::X
            &&  space[1][1]->getState() == GameSpace::X  // Diagonal 1
            &&  space[2][2]->getState() == GameSpace::X)
            || (space[0][2]->getState() == GameSpace::X
            &&  space[1][1]->getState() == GameSpace::X  // Diagonal 2
            &&  space[2][0]->getState() == GameSpace::X)) 
            return true;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////
// checkOWin()
//
// Checks if O has won.
///////////////////////////////////////////////////////////////////////
bool
MainWindow::checkOWin()
{
    // Check to see if O wins
    for (int i = 0; i < 3; i++) {
        if (   (space[i][0]->getState() == GameSpace::O 
            &&  space[i][1]->getState() == GameSpace::O  // Rows
            &&  space[i][2]->getState() == GameSpace::O)
            || (space[0][i]->getState() == GameSpace::O
            &&  space[1][i]->getState() == GameSpace::O  // Cols
            &&  space[2][i]->getState() == GameSpace::O)
            || (space[0][0]->getState() == GameSpace::O
            &&  space[1][1]->getState() == GameSpace::O  // Diagonal 1
            &&  space[2][2]->getState() == GameSpace::O)
            || (space[0][2]->getState() == GameSpace::O
            &&  space[1][1]->getState() == GameSpace::O  // Diagonal 2
            &&  space[2][0]->getState() == GameSpace::O)) 
        return true;
        
    }
    return false;
}

///////////////////////////////////////////////////////////////////////
// checkDraw()
//
// Checks if a draw has occured. Alerts the user and starts a new game 
// if neccessary. Inspired and based on code written by Bart Massey.
///////////////////////////////////////////////////////////////////////
bool
MainWindow::checkDraw()
{
    bool draw    = true;
    bool found_x = false;
    bool found_o = false;

    // Check the rows to see if they contain an X and an O
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (space[i][j]->getState() == GameSpace::X)
                found_x = true;
            else if (space[i][j]->getState() == GameSpace::O)
                found_o = true;
        }
        if (!found_x || !found_o) {
            draw = false;
        } else {
            found_x = false;
            found_o = false;
        }
    }

    // Check the columns to see if they contain an X and an O
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (space[j][i]->getState() == GameSpace::X)
                found_x = true;
            else if (space[j][i]->getState() == GameSpace::O)
                found_o = true;
        }
        if (!found_x || !found_o) {
            draw = false;
        } else {
            found_x = false;
            found_o = false;
        }
    }

    // Check the first diaganol
    for (int i = 0; i < 3; i++) {
        if (space[i][i]->getState() == GameSpace::X)
            found_x = true;
        else if (space[i][i]->getState() == GameSpace::O)
            found_o = true;
    }
    if (!found_x || !found_o) {
        draw = false;
    } else {
        found_x = false;
        found_o = false;
    }

    // Check the second diaganol
    int j = 2;
    for (int i = 0; i < 3; i++) {
        if (space[i][j]->getState() == GameSpace::X)
            found_x = true;
        else if (space[i][j]->getState() == GameSpace::O)
            found_o = true;

        j--;
    }
    if (!found_x || !found_o) 
        draw = false;

    // If all rows, cols, and diaganols contain an X
    // and an O then it is a draw.
    return draw;
}

///////////////////////////////////////////////////////////////////////
// changeTurnToX()
//
// Displays a graphic to the user alerting them that it is now Xs turn
///////////////////////////////////////////////////////////////////////
void
MainWindow::changeTurnToX()
{
    turn->setText(tr("X's Turn"));
    QString x_turn_image_file_name = ":/images/x_turn.png";

    QPixmap x_turn_image;
    x_turn_image.load(x_turn_image_file_name);

    turn->setPixmap(x_turn_image);
    return;
}

///////////////////////////////////////////////////////////////////////
// changeTurntoO()
//
// Displays a graphic to the user alerting them that it is now Os turn
///////////////////////////////////////////////////////////////////////
void
MainWindow::changeTurnToO()
{
    bool start_game = true;

    turn->setText(tr("O's Turn"));
    QString o_turn_image_file_name = ":/images/o_turn.png";

    QPixmap o_turn_image;
    o_turn_image.load(o_turn_image_file_name);

    turn->setPixmap(o_turn_image);

    // Check to see if it is the start of a game
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!(space[i][j]->getState() == GameSpace::EMPTY))
                start_game = false;
        }
    }

    // if this is a new game change the player to X
    if (start_game)
        changeTurnToX();

    return;
}

///////////////////////////////////////////////////////////////////////
// undo()
//
// Undoes the last move taken during the current active game
///////////////////////////////////////////////////////////////////////
void
MainWindow::undo()
{
    if (!move->isEmpty()) {
        GameSpace::SpaceState state;
        int                   row;
        int                   col;
        Move                  last;

        // What was the last move?
        last  = move->last();
        row   = last.row;
        col   = last.col;
        state = last.state;
    
        // Clear the space where the last move occurred
        space[row][col]->clear();
        
        // If we are undoing an X move add an X to the x_pieces_list
        if (state == GameSpace::X) {
            QString x_image_file_name = ":/images/x.png";
    
            QPixmap x_image;
            x_image.load(x_image_file_name);
    
            x_pieces_list->createPiece(x_image, true);
        }
    
        // If we are undoing an O move an O to the o_pieces_list
        if (state == GameSpace::O) {
            QString o_image_file_name = ":/images/o.png";

            QPixmap o_image;
            o_image.load(o_image_file_name);

            o_pieces_list->createPiece(o_image, false);
        }

        // After an undo move I set the current player depending on 
        // who has played the most pieces. This is not a bug! If a
        // user plays a move out of turn and then undoes that move
        // I want to display the turn of who should be going next.
        if ((x_pieces_list->count()) >= (o_pieces_list->count()))
            changeTurnToX();
        else
            changeTurnToO();

        move->pop_back();
    }
}

///////////////////////////////////////////////////////////////////////
// setupWidgets()
//
// Creates all the widgets and puts them in the right places.
///////////////////////////////////////////////////////////////////////
void
MainWindow::setupWidgets()
{
    frame         = new QFrame;
    vert_layout   = new QVBoxLayout(frame);
    horz_layout   = new QHBoxLayout();
    board_layout  = new QGridLayout;
    notify_layout = new QHBoxLayout();
    bottom_layout = new QHBoxLayout();
    game_box      = new QGroupBox;
    notify_box    = new QGroupBox;
    bottom_box    = new QGroupBox;
    board_grid    = new QGroupBox;
    x_pieces_list = new PiecesList(true);
    o_pieces_list = new PiecesList(false);

    // New Game Button
    new_game_button = new QPushButton();
    new_game_button->setMaximumSize(80, 80);
    QPixmap new_game_image;
    QString new_game_image_file_name = ":/images/new_game.png";
    new_game_image.load(new_game_image_file_name);
    new_game_button->setIcon(new_game_image);
    new_game_button->setIconSize(QSize(50, 50));
    connect(new_game_button, SIGNAL(clicked()), this, SLOT(newGame()));

    // Title Image
    QString title_image_file_name = ":/images/title.png";
    QPixmap title_image;
    title_image.load(title_image_file_name);
    title_pic = new QLabel();
    title_pic->setPixmap(title_image);
    turn = new QLabel(tr("X's Turn"));

    // Notify Layout
    notify_layout->addWidget(title_pic);
    notify_layout->addWidget(turn);
    notify_layout->addWidget(new_game_button);
    notify_box->setLayout(notify_layout);

    // Quit Button
    quit_button   = new QPushButton();
    quit_button->setMaximumSize(60, 60);
    QPixmap quit_image;
    QString quit_image_file_name = ":/images/quit.png";
    quit_image.load(quit_image_file_name);
    quit_button->setIcon(quit_image);
    quit_button->setIconSize(QSize(50, 50));
    connect(quit_button, SIGNAL(clicked()), qApp, SLOT(quit()));

    // Undo Button
    undo_button   = new QPushButton();
    undo_button->setMaximumSize(60, 60);
    QPixmap undo_image;
    QString undo_image_file_name = ":/images/undo.png";
    undo_image.load(undo_image_file_name);
    undo_button->setIcon(undo_image);
    undo_button->setIconSize(QSize(50, 50));
    connect(undo_button, SIGNAL(clicked()), this, SLOT(undo()));

    // Bottom Layout
    bottom_layout->addWidget(undo_button);
    bottom_layout->addWidget(quit_button);
    bottom_box->setLayout(bottom_layout);

    // Board Grid
    board_grid->setStyleSheet("background-image: url(images/bg.png)");
    board_grid->setMaximumSize(280, 280);

    // Board Layout
    board_layout->addWidget(space[0][0], 0, 0);
    board_layout->addWidget(space[0][1], 0, 1);
    board_layout->addWidget(space[0][2], 0, 2);
    board_layout->addWidget(space[1][0], 1, 0);
    board_layout->addWidget(space[1][1], 1, 1);
    board_layout->addWidget(space[1][2], 1, 2);
    board_layout->addWidget(space[2][0], 2, 0);
    board_layout->addWidget(space[2][1], 2, 1);
    board_layout->addWidget(space[2][2], 2, 2);
    board_grid->setLayout(board_layout);

    // Connect each game space with the piecePlayed signal
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            connect(space[i][j], 
                    SIGNAL(piecePlayed(bool, int, int)),
                    this, 
                    SLOT(movePlayed(bool, int, int)), 
                    Qt::QueuedConnection);
        }
    }

    // Horizontal Layout
    horz_layout->addWidget(x_pieces_list);
    horz_layout->addWidget(board_grid);
    horz_layout->addWidget(o_pieces_list);
    horz_layout->setSizeConstraint(QLayout::SetFixedSize);
    game_box->setLayout(horz_layout);

    // Vertical Layout
    vert_layout->addWidget(notify_box);
    vert_layout->addWidget(game_box);
    vert_layout->addWidget(bottom_box);
    vert_layout->setSpacing(0);

    setCentralWidget(frame);
}
