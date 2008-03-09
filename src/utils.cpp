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

#include "utils.h"
#include <string>
#include <iostream>
#include <QString>

using std::string;

void printHex (QString & thestr) {
  string str = thestr.toStdString();
  string readableStr = "";
  for (unsigned int j = 0; j < str.length(); ++j) {
    if (thestr.at(j).isPrint()) {
      readableStr += str[j];
    } else {
      readableStr += '?';
    }
    char c = str[j];
    for (int i = 2 * sizeof(char) - 1; i >= 0; --i) {
        std::cerr << "0123456789ABCDEF"[((c >> i*4) & 0xF)];
    }
    std::cerr << " ";
  }
  
  std::cerr << "-- " << readableStr;
}


