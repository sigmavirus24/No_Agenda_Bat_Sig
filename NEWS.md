# NEWS

## 7 January 2011

### No Agenda Bat Signal updated to version 0.3.

Features added:

* [[nabatsignalrc]] configuration file
    * Ability to set different mp3 player from `mpg123`
    * Ability to use different jingle
    * Ability to turn on or off SSL
    * Ability to turn on or off Gtk pop-up box
    * Ability to set default browser (only used with Gtk pop-up box)
    * Ability to have your bat signal echo the signal to the IRC bot (only if you're running the bot).
* Equivalent command-line options (./nabatsignal -h)
* Fully functioning and non-leaky regular HTTP connection.

### No Agenda IRC Bot released at version 0.1.

Features:

* [[nabotrc]] configuration file.
* Two commands:
    * `.test` will say "Hello Slaves" in the appropriate channel or private message only to authorized users.
    * `.quit` will have the bot shutdown.
* Automattically backgrounds itself, can and will run indefinitely without a memory leak.
