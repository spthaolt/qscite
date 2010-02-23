#!/bin/bash

cd qtermwidget
qmake
make
cd ..
qmake qscite-debug.pro
make
