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

///////////////////////////////////////////////////////////////////////
// main.cc
//
// This is the driver for xsnos. Not much to see here.
///////////////////////////////////////////////////////////////////////

#include <QApplication>

#include "MainWindow.hh"

int
main(int argc, char **argv)
{
    QApplication app(argc,argv);

    MainWindow window;
    window.newGame();
    window.show();

    return app.exec();
}
