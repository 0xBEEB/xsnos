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

#include "GameSpace.hh"

///////////////////////////////////////////////////////////////////////
// GameSpace(int row, int col, QWidget *parent)
//
// Constructor
//
// Parameters:  int     row     - Row the space is located in
//              int     col     - Column the space is located in
//              QWidget *parent - Parent object
///////////////////////////////////////////////////////////////////////
GameSpace::GameSpace(int row, int col, QWidget *parent) :
    QWidget(parent), 
    space_state(GameSpace::EMPTY), 
    row(row), 
    col(col)
{
    setAcceptDrops(true);           // Allow items to be dropped in space
    setMinimumSize(60, 60);         // We want the spaces to be uniform
    setMaximumSize(60, 60);         // Nice squares
}

///////////////////////////////////////////////////////////////////////
// SpaceState()
//
// Returns: The state the space is in.
///////////////////////////////////////////////////////////////////////
GameSpace::SpaceState
GameSpace::getState()
{
    return space_state;
}

///////////////////////////////////////////////////////////////////////
// getRow()
//
// Returns: The row the space occupies
///////////////////////////////////////////////////////////////////////
int
GameSpace::getRow()
{
    return row;
}

///////////////////////////////////////////////////////////////////////
// getCol()
//
// Returns: The column the space occupies
///////////////////////////////////////////////////////////////////////
int
GameSpace::getCol()
{
    return col;
}

///////////////////////////////////////////////////////////////////////
// clear()
//
// Clears the space and sets the state to EMPTY
///////////////////////////////////////////////////////////////////////
void
GameSpace::clear()
{
    QString empty_file_name = ":/images/empty.png";
    QPixmap empty_pixmap;
    empty_pixmap.load(empty_file_name);

    piece_pixmap = empty_pixmap;    

    space_state = GameSpace::EMPTY;

    update();
}

///////////////////////////////////////////////////////////////////////
// dragEnterEvent(QDragEnterEvent *event)
//
// Parameters:  *event      - The drag event
//
// Only accept tictactoe pieces
///////////////////////////////////////////////////////////////////////
void
GameSpace::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-piece"))
        event->accept();
    else
        event->ignore();
}

///////////////////////////////////////////////////////////////////////
// dragLeaveEvent(QDragLeaveEvent *event)
//
// Parameters:  *event      - The drag event
///////////////////////////////////////////////////////////////////////
void
GameSpace::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

///////////////////////////////////////////////////////////////////////
// dragMoveEvent(QDragMoveEvent *event)
//
// Parameters:  *event      - The drag event
//
// If the item being dragged is a tictactoe piece allow it to be
// dropped. Otherwise, ignore it.
///////////////////////////////////////////////////////////////////////
void
GameSpace::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-piece")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

///////////////////////////////////////////////////////////////////////
// dropEvent(QDropEvent *event)
//
// Parameters: *event       - The drag event
//
// If the item is a tictactoe piece and the space is currently in the
// EMPTY state, then allow the piece to be placed here. When the piece
// is placed change the spaces state, emit the appropriate signals to
// the MainWindow, and repaint the square to represent the new image.
///////////////////////////////////////////////////////////////////////
void
GameSpace::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-piece")
            && space_state == GameSpace::EMPTY) {

        // piece_data  - The mimetype of the tictactoe pieces
        // data_stream - An IO stream that is used to write to the space
        // square      - A geometric space that we repaint after a state change
        QByteArray  piece_data = event->mimeData()->data("image/x-piece");
        QDataStream data_stream(&piece_data, QIODevice::ReadOnly);
        QRect       square = QRect(event->pos().x()/60 * 60,
                                   event->pos().y()/60 * 60,
                                   60 , 60);
        QPixmap     pixmap;             // The pixmap to that is being dropped
        bool        is_x;               // Is the object an X?

        data_stream >> pixmap >> is_x;  // Write the data to the variables

        piece_pixmap = pixmap;          // Update the squares image
        piece_rect   = square;          // Draw the square

        event->setDropAction(Qt::MoveAction);
        event->accept();

        update(square);

        // Emit the appropriate signals and set the state
        if (is_x) {
            space_state = GameSpace::X;
            emit piecePlayed(true, getRow(), getCol());
        } else {
            space_state = GameSpace::O;
            emit piecePlayed(false, getRow(), getCol());
        }

    } else {
        event->ignore();
    }
}

///////////////////////////////////////////////////////////////////////
// paintEvent(QPaintEvent *event)
//
// Parameters:  *event      - The paint event
//
// This is called whenever a space is redrawn
///////////////////////////////////////////////////////////////////////
void
GameSpace::paintEvent(QPaintEvent * event)
{
    QPainter painter;                               // The painting machine
    painter.begin(this);                            // Start painting
    painter.fillRect(event->rect(), Qt::white);     // Paint the square white
    
    painter.drawPixmap(piece_rect, piece_pixmap);   // Paint the piece image
    painter.end();
}
