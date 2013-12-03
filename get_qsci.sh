#! /bin/sh

printf "Locating most recent QScintilla2 release..."
_download_path=$(
  curl -s http://www.riverbankcomputing.co.uk/software/qscintilla/download 2>/dev/null | awk -F'"' '
    /snapshot/ {next}
    /tar\.gz/ { print $2 }
  ' )
_download_link=`echo $_download_path | sed "s#http://#http://optimate.dl.#" | sed "s#projects/pyqt/files#project/pyqt#"`
_archive_filename=$( basename "$_download_path" )
_archive_dir=$( echo "$_archive_filename" | sed 's/.tar.gz//' )
_archive_size=$(
  curl -sI "$_download_link" 2>/dev/null | awk '
    BEGIN { RS="\r\n" }
    $1=="Content-Length:" { print $2 }
  ' )
echo 'Done!'

if [ -f "$_archive_filename" ]; then
  _local_size=$( ls -l "$_archive_filename" | awk '{ print $5 }' )
  
  if [ "$_local_size" != "$_archive_size" ]; then
    rm -f "$_archive_filename"
    echo "Downloading QScintilla2..."
    curl "$_download_link" -o "$_archive_filename"
  else
    echo "QScintilla2 already downloaded." > /dev/stderr
  fi
else
  echo "Downloading QScintilla2..."
  curl "$_download_link" -o "$_archive_filename"
fi

if [ -d "$_archive_dir" ]; then
  echo "QScintilla2 archive already unpacked." > /dev/stderr
else
  printf "Extracting QScintilla2..." > /dev/stderr
  tar -xzf "$_archive_filename"
  echo 'Done!' > /dev/stderr
fi

ln -sf $_archive_dir QScintilla2
