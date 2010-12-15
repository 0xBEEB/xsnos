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

#ifndef WF_MAINWINDOW_HH
#define WF_MAINWINDOW_HH

#include <QPixmap>
#include <QMainWindow>
#include <QGroupBox>
#include <QRectF>
#include <QPushButton>
#include <QLabel>
#include <QVector>

#include "GameSpace.hh"

// These classes need to be declared here so that I can put them to use later. 
class PiecesList;
class GameBoard;
class QListWidgetItem;

///////////////////////////////////////////////////////////////////////
// Move
//
// This struct represents a move that was played in this game. It is
// utilized for undo and logging procedures.
///////////////////////////////////////////////////////////////////////
struct Move 
{
    GameSpace::SpaceState   state; 
    int                     row;
    int                     col;
};

///////////////////////////////////////////////////////////////////////
// MainWindow
//
// This class creates a main window for xsnos. It will probably always
// be ran as a singleton, but if you want multiple instances go for it.
// It works great with multiple instances with reasonable results.
///////////////////////////////////////////////////////////////////////
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    void printMoves();

public slots:
    void        newGame();

private slots:
                // Check for endgame condition
    void        movePlayed(bool state, int row, int col);
    void        changeTurnToX();
    void        changeTurnToO();
    void        undo();

private:
    void        setupWidgets();
    bool        checkOWin();
    bool        checkXWin();
    bool        checkDraw();

    // The layouts and boxes that make it look pretty
    QFrame      *frame;
    QVBoxLayout *vert_layout;
    QHBoxLayout *notify_layout;
    QHBoxLayout *horz_layout;
    QHBoxLayout *bottom_layout;
    QGridLayout *board_layout;
    QGroupBox   *notify_box;
    QGroupBox   *board_grid;
    QGroupBox   *game_box;
    QGroupBox   *bottom_box;

    // The game objects
    PiecesList  *o_pieces_list;
    PiecesList  *x_pieces_list;
    GameSpace   *space[3][3];

    // Record keeping
    QVector<Move> *move;

    // Buttons and Labels
    QPushButton *quit_button;
    QPushButton *new_game_button;
    QPushButton *undo_button;
    QLabel      *turn;
    QLabel      *title_pic;
};

#endif
