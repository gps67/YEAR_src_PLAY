
D1=$HOME/audio
d2=YEAR/AUDIO_2016
D2=$HOME/$d2
set -x
[ -d $HOME/YEAR/. ] || exit
[ -d $D2/. ] || exit
[ -d $D1/. ] || ask_do ln -s $d2 $D1
# FAILED # type cdda2wav || sudo apt-get install icedax
type lame || ask_do sudo apt-get install lame

echo https://sourceforge.net/projects/cdrtools/files/alpha/cdrtools-3.02a06.tar.gz/download
