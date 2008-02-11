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
#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<string> tokenize(string strIn, char delim) {
  vector<string> tokenized;
  int count = 0;

  for (int n = 0; n < strIn.length(); ++n) {
    if (strIn[n] == delim) {
      if (!count) {
        tokenized.push_back("");
      } else {
        tokenized.push_back(strIn.substr(n - count, count));
      }
      count = -1;
    }

    if ((n + 1) == strIn.length()) {
      if (!count || count == -1) {
        tokenized.push_back("");
      } else {
        tokenized.push_back(strIn.substr(n - count, count + 1));
      }
    }
    ++count;
  }

  return tokenized;
}

