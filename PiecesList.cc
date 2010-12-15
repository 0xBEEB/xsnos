///////////////////////////////////////////////////////////////////////
// Copyright © 2010 Thomas Schreiber <ubiquill@gmail.com>
//
// This file is part of xsnos. However, it is a modified version of 
// the file PieceList.cc located at 
// http://doc.qt.nokia.com/4.7-snapshot/draganddrop-puzzle-pieceslist-cpp.html
// The file is used and released here under the BSD license as
// outlined below.
// 
///////////////////////////////////////////////////////////////////////
 /****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include <QtGui>

#include "PiecesList.hh"

///////////////////////////////////////////////////////////////////////
// PiecesList(bool is_x, QWidget *parent)
//
// Parameters:  bool        is_x
//              QWidget     *parent
//
// Constructor
///////////////////////////////////////////////////////////////////////
PiecesList::PiecesList(bool is_x, QWidget *parent) :
    QListWidget(parent), 
    is_x(is_x)
{
    setDragEnabled(true);                   // Yes, I want to drag things
    setViewMode(QListView::IconMode);       // Make it look like icons
    setIconSize(QSize(60, 60));             // Size of icons
    setSpacing(5);                          // Space between icons
    setAcceptDrops(true);                   // Yes, drop things back here
    setDropIndicatorShown(true);            // Show cursor indicator
    setMaximumSize(175, 220);               // The perfect size
}

///////////////////////////////////////////////////////////////////////
// createPiece(QPixmap pixmap, bool is_x)
//
// Parameters:  QPixmap     pixmap
//              bool        is_x
//
// Creates piece to be added to the list.
///////////////////////////////////////////////////////////////////////
void
PiecesList::createPiece(QPixmap pixmap, bool is_x)
{
    QListWidgetItem *piece_item = new QListWidgetItem(this);

    piece_item->setIcon(QIcon(pixmap));
    piece_item->setData(Qt::UserRole, QVariant(pixmap));
    piece_item->setData(Qt::UserRole + 1, is_x);
    piece_item->setFlags(Qt::ItemIsEnabled |                // Not grayed out
                         Qt::ItemIsSelectable |             // Can be selected
                         Qt::ItemIsDragEnabled);            // Can be dragged
}

///////////////////////////////////////////////////////////////////////
// dragEnterEvent(QDragEnterEvent *event)
//
// Parameters:  *event
///////////////////////////////////////////////////////////////////////
void
PiecesList::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-piece"))
        event->accept();
    else
        event->ignore();
}

///////////////////////////////////////////////////////////////////////
// dragMoveEvent(QDragMoveEvent *event)
//
// Parameters:  *event
///////////////////////////////////////////////////////////////////////
void
PiecesList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("image/x-piece")) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else
        event->ignore();
}

///////////////////////////////////////////////////////////////////////
// dropEvent(QDropEvent *event)
//
// Parameters:  *event
//
// How to handle items being dropped into the list
///////////////////////////////////////////////////////////////////////
void
PiecesList::dropEvent(QDropEvent *event)
{
    // Make sure that that the piece is tictactoe piece
    // Currently pieces of either type can be dropped into either list. I'm
    // not sure if this is a bug or not.
    if (event->mimeData()->hasFormat("image/x-piece")) {
        // piece_data  - The mimetype of the item
        // data_stream - The IO stream for writing the item data 
        QByteArray  piece_data = event->mimeData()->data("image/x-piece");
        QDataStream data_stream(&piece_data, QIODevice::ReadOnly);
        QPixmap     pixmap;                         // Pixmap to be painted
        bool        is_x;                           // Is the item an X?

        data_stream >> pixmap >> is_x;

        createPiece(pixmap, is_x);                  // Add piece to the list

        event->setDropAction(Qt::MoveAction);
        event->accept();

    } else
        event->ignore();
}

///////////////////////////////////////////////////////////////////////
// startDrag(Qt::DropActions)
//
// Parameters:  Qt::DropActions
//
// Actions to take when an item is dragged off of the list
///////////////////////////////////////////////////////////////////////
void
PiecesList::startDrag(Qt::DropActions /*supported_actions*/)
{
    QByteArray      item_data;
    QDataStream     data_stream(&item_data, QIODevice::WriteOnly);
    QListWidgetItem *item = currentItem();
    QPixmap         pixmap = qVariantValue<QPixmap>(item->data(Qt::UserRole));
    QMimeData       *mime_data = new QMimeData;
    QDrag           *drag = new QDrag(this);
    bool            is_x = qVariantValue<bool>(item->data(Qt::UserRole + 1));

    data_stream << pixmap << is_x;

    mime_data->setData("image/x-piece", item_data);

    drag->setMimeData(mime_data);
    drag->setHotSpot(QPoint(pixmap.width()/2, pixmap.height()/2));
    drag->setPixmap(pixmap);

    if (drag->exec(Qt::MoveAction) == Qt::MoveAction)
        delete takeItem(row(item));
}
