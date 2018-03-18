# ampa-vt18
lighting control software prototypes

ampa* is the umbrella name I tend to use when developing lighting control software.
More often than not—ie. *so far*—it's a prototype targeting a specific lighting
setup, and live control.

The version in here talks on the network to a conduit/shim piece of software that
takes what is received on the network side and pushes it onto the serial port.
This is less effective than desired, but seeing as how the effort needed for
setting up a 1.25Mbps serial link is magnitudes easier in python than in C, it'll
have to do for now, until all the *interesting* problems about how build an intuitive
live control interface are done.

Requirements:
HW: No idea, it runs on a single-core 1.86GHz celeron with integrated Intel 945GM graphics
OS: Linux
Library dependencies are: SDL2 GL and GLU (and standard math)
