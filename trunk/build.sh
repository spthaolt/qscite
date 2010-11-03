#!/bin/bash

_QMAKE_CMD=""
which qmake-qt4 > /dev/null 2>&1
if [ $? -ne 0 ]; then
  which qmake > /dev/null 2>&1
  
  if [ $? -ne 0 ]; then
    echo "ERROR: qmake was not found in your current path.  Verify that you have the Qt4 dev tools installed and correctly configured." > /dev/stderr
    exit 1
  else
    _QMAKE_CMD="qmake"
  fi
else
  _QMAKE_CMD="qmake-qt4"
fi

which curl > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "ERROR: curl was not found in your current path.  Verify that you have curl installed and correctly configured." > /dev/stderr
  exit 1
fi

_download_path=$( curl http://www.riverbankcomputing.co.uk/software/qscintilla/download 2>/dev/null | grep 'tar\.gz' | grep -v snapshot | awk -F'"' '{ print $2 }' )
_download_link="http://www.riverbankcomputing.co.uk$_download_path"
_archive_filename=$( basename "$_download_path" )
_archive_dir=$( echo "$_archive_filename" | sed 's/.tar.gz//' )
_archive_size=$( curl -I "$_download_link" 2>/dev/null | grep '^Content-Length' | awk '{ print $2 }' | tr -d '\n' | tr -d '\r' )

if [ -f "$_archive_filename" ]; then
  _local_size=$( ls -la "$_archive_filename" | awk '{ print $5 }' | tr -d '\r' | tr -d '\n' )
  
  if [ "$_local_size" != "$_archive_size" ]; then
    rm -f "$_archive_filename"
    curl "$_download_link" -o "$_archive_filename"
  else
    echo "QScintilla2 already downloaded." > /dev/stderr
  fi
else
  echo
  curl "$_download_link" -o "$_archive_filename"
fi

if [ -d "$_archive_dir" ]; then
  echo "QScintilla2 archive already unpacked." > /dev/stderr
else
  tar -xvzf "$_archive_filename"
fi

cd "$_archive_dir/Qt4"

if [ -f ../../build/libqscintilla2.a ]; then
  echo "QScintilla2 static library already built." > /dev/stderr
else
  "$_QMAKE_CMD" 
  make staticlib
  cp libqscintilla2.a ../../build/
fi

if [ -d ../../include/Qsci ]; then
  echo "QScintilla2 headers already copied." > /dev/stderr
else
  cp -pvrf Qsci ../../include/
fi

cd ../../
ln -sf "$_archive_dir" ./QScintilla
"$_QMAKE_CMD" qscite-release.pro
make