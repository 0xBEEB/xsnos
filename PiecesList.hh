///////////////////////////////////////////////////////////////////////
// Copyright © 2010 Thomas Schreiber <ubiquill@gmail.com>
//
// This file is part of xsnos. However, it is a modified version of 
// the file PieceList.hh located at 
// http://doc.qt.nokia.com/4.7-snapshot/draganddrop-puzzle-pieceslist-h.html
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
 
#ifndef WF_PIECESLIST_HH
#define WF_PIECESLIST_HH

#include <QListWidget>

///////////////////////////////////////////////////////////////////////
// PiecesList
//
// This is the headerfile for the PiecesList class.
///////////////////////////////////////////////////////////////////////
class PiecesList : public QListWidget
{
    Q_OBJECT                                            // Macro used by Qt4

public:
    PiecesList(bool is_x, QWidget *parent = 0);         // Constructor
    void createPiece(QPixmap pixmap, bool is_x);        // Make a new piece

protected:
    void dragEnterEvent(QDragEnterEvent *event);        // Drag&Drop methods
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions supported_actions);

private:
    bool is_x;                                          // Is this a list of Xs
};
#endif
