#!/bin/bash

_download_path=$( curl http://www.riverbankcomputing.co.uk/software/qscintilla/download | grep 'tar\.gz' | grep -v snapshot | awk -F'"' '{ print $2 }' )
_download_link="http://www.riverbankcomputing.co.uk$_download_path"
_archive_filename=$( basename "$_download_path" )
_archive_dir=$( echo "$_archive_filename" | sed 's/.tar.gz//' )

curl "$_download_link" -o "$_archive_filename"
tar -xvzf "$_archive_filename"
cd "$_archive_dir/Qt4"
qmake-qt4
make staticlib
cp libqscintilla2.a ../../build/
cp -pvrf Qsci ../../include/

cd ../../
ln -sf "$_archive_dir" ./QScintilla
qmake qscite-release.pro
make
