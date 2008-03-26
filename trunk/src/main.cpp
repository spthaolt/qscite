/***************************************************************************
 *   Copyright (C) 2007-2008 by Jared Sutton <jpsutton@gmail.com>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QApplication>
#include <QStringList>
#include <QVector>
#include "mainwindow.h"
#include "launcher.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(qscite);

    QApplication app(argc, argv);
    app.setApplicationName("QSciTE");
    app.setOrganizationName("QSciteTeam");
    QStringList _argv;
    
    for (int i = 1; i < argc; ++i) {
      _argv.push_back(QString(argv[i]));
    }
    
    Launcher launcher(_argv, &app);
    return app.exec();
}


