This is a simple C program that users can run and will background for 30 minutes. It will be configurable by the user and will play a "jingle" of the user's choice to notify them that Adam has tweeted the bat-signal.

## DEPENDENCIES:
* **GTK+2.0** http://www.gtk.org
* **mpg123** http://www.mpg123.de or someother mp3 player (i.e. mplayer)

## INSTALLATION:
1. In the directory containing src/ and jingles/ run the command "make".
2. The bat signal will be made in that directory and can be run from there.
3. Run ./nabatsignal -h for more information.
4. Copy nabatsignalrc (and if you are going to use it, nabotrc) to ~/.nabatsignalrc (and ~/.nabotrc). Edit these files to configure then and you will be able to run ./nabatsignal and ./nabotrc without problems.
5. Also see the wiki page about the nabatsignalrc file included in this directory. (https://github.com/sigmavirus24/No_Agenda_Bat_Sig/wiki/nabatsignalrc)
