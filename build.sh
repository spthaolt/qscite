#!/bin/bash

_QMAKE_CMD=""
which qmake-qt4
if [ $? -ne 0 ]; then
  which qmake
  
  if [ $? -ne 0 ]; then
    echo "ERROR: qmake was not found in your current path.  Verify that you have the Qt4 dev tools installed and correctly configured." > /dev/stderr
    exit 1
  else
    _QMAKE_CMD="qmake"
  fi
else
  _QMAKE_CMD="qmake-qt4"
fi

which curl
if [ $? -ne 0 ]; then
  echo "ERROR: curl was not found in your current path.  Verify that you have curl installed and correctly configured." > /dev/stderr
  exit 1
fi

_download_path=$( curl http://www.riverbankcomputing.co.uk/software/qscintilla/download | grep 'tar\.gz' | grep -v snapshot | awk -F'"' '{ print $2 }' )
_download_link="http://www.riverbankcomputing.co.uk$_download_path"
_archive_filename=$( basename "$_download_path" )
_archive_dir=$( echo "$_archive_filename" | sed 's/.tar.gz//' )
_archive_size=$( curl -I "$_download_link" | grep '^Content-Length' | awk '{ print $2 }' )

if [ -f "$_archive_filename" ]; then
  _local_size=$( ls "$_archive_filename" | awk '{ print $5 }' )
  if [ "$_local_size" -ne "$_archive_size" ]; then
    rm -f "$_archive_filename"
    curl "$_download_link" -o "$_archive_filename"
  fi
else
  curl "$_download_link" -o "$_archive_filename"
fi

tar -xvzf "$_archive_filename"
cd "$_archive_dir/Qt4"

"$_QMAKE_CMD" 
make staticlib
cp libqscintilla2.a ../../build/
cp -pvrf Qsci ../../include/

cd ../../
ln -sf "$_archive_dir" ./QScintilla
"$_QMAKE_CMD" qscite-release.pro
make