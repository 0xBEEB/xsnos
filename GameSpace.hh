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

#ifndef WF_GAMESPACE_HH
#define WF_GAMESPACE_HH

///////////////////////////////////////////////////////////////////////
// GameSpace.hh
// 
// This file contains the declarations for the GameSpace class. It's
// objects represent spaces on a tictactoe board. The spaces can exist
// in one of three states: EMPTY, X, or O.
///////////////////////////////////////////////////////////////////////

#include <QtGui>        // Qt4.6
#include <QPixmap>      // For displaying the current state to the user
#include <QPoint>       // Specifies the area drag/drop events take place
#include <QWidget>      // The parent class

class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;

///////////////////////////////////////////////////////////////////////
// GameSpace
//
// A class whose objects represent a space on a tictactoe board. Can
// exist in one of three states: EMPTY, X, or O.
///////////////////////////////////////////////////////////////////////
class GameSpace : public QWidget
{
    Q_OBJECT            // A macro used by Qt

public:
    enum SpaceState {EMPTY, X, O};              // The state the space is in
    
    GameSpace(int row, int col, QWidget *parent = 0);   // Constructor
    GameSpace::SpaceState getState();           // Getter for the current state
    int                   getRow();             // Getter for the row
    int                   getCol();             // Getter for the column
    void                  clear();              // Clear the space

// Signals are used by Qt to communicate with Slots in other objects
signals:
    void piecePlayed(bool, int, int);           // A piece was played

protected:
    void dragEnterEvent(QDragEnterEvent *event); // Begin dragging
    void dragLeaveEvent(QDragLeaveEvent *event); // Drag out of a space
    void dragMoveEvent(QDragMoveEvent *event);  // Move over a space
    void dropEvent(QDropEvent *event);          // Placement of piece
    void paintEvent(QPaintEvent *event);        // How square is drawn

private:
    QPixmap        piece_pixmap;                // Image used in space
    QRect          piece_rect;                  // Rect that is updated
    SpaceState     space_state;                 // State of the space
    int            row;                         // Row in grid
    int            col;                         // Column in grid
};
#endif
