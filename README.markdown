# Hamurabi: Classic BASIC gaming.

An MIT licensed, C99 implementation of the classic BASIC game
hamurabi. Useful for boostrapping build systems, dreaming of a
life that can never be.

## What you get.

For history's sake, the HMRABBI by Peter Turnbull is
included. Of primary interest are going to be:

* main.c :: User interaction, state machine.
* city.{c,h} :: City structure, manipulation functions.

This being a quick hack, comments are minimal but it shouldn't
be _too_ hard to untangle. The primary goal in the translation
was to separate user I/O from the core computations. In some
sense this is not yet complete: the state machine is not yet
separated from user I/O. Still, it is now _far_ simpler to
swap out the command-driven interface with, say, ncurses or a
socket.

As with any translation, there was some leeway taken. If I
went too far, report it as a bug!

## What I'll think is nifty.

There's certainly possibility for improvement. Here's a list
of things I'd readily accept:

* Curses interface,
* documentation (source comments or otherwise) and
* Debian package infrastructure.

If it's not on the list, send me a patch: I'll probably like
it!