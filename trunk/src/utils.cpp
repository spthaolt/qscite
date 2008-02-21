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
#include <vector>
#include <string>
#include <iostream>
#include <QString>
#include <QFileInfo>

using namespace std;

vector<string> tokenize(string strIn, char delim) {
	vector<string> tokenized;
	int tokStart = 0, tokEnd = 0;
	
	tokStart = strIn.find_first_not_of(delim);
	while (tokStart != -1) {
		tokEnd = strIn.find(delim, tokStart);
		tokenized.push_back(strIn.substr(tokStart, tokEnd - tokStart));
		tokStart = strIn.find_first_not_of(delim, tokEnd);
	}
	
	return tokenized;
}

QString strippedName(const QString &fullFileName) {
  if (fullFileName.isEmpty()) {
    return "Untitled Document";
  } else {
    return QFileInfo(fullFileName).fileName();
  }
}

void printHex (QString & thestr) {
  string str = thestr.toStdString();
  
  for (unsigned int j = 0; j < str.length(); ++j) {
    char c = str[j];
    for (int i = 2 * sizeof(char) - 1; i >= 0; --i) {
        cout << "0123456789ABCDEF"[((c >> i*4) & 0xF)];
    }
    cout << " ";
  }
}


