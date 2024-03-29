#  Copyright (C) 2006 Fons Adriaensen <fons.adriaensen@skynet.be>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.

#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


PREFIX = /usr/local
VERSION = 0.0.2
DISTDIR = yass-$(VERSION)
CPPFLAGS += -O3 -Wall -Wno-narrowing -fPIC -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS \
	-DVERSION=\"$(VERSION)\" -DPREFIX=\"$(PREFIX)\" \
	-I/usr/X11R6/include `pkg-config --cflags freetype2` `pkg-config jack --cflags`

LDFLAGS += -L/usr/X11R6/$(LIBDIR)
LDLIBS +=  -lX11 -lXft -lpthread `pkg-config --libs freetype2` `pkg-config jack --libs`

CLTHREADS_O = p_thread.o a_thread.o itc_mesg.o itc_ip1q.o itc_ctrl.o textmsg.o
CLTHREADS_H = clthreads.h

CLXCLIENT_O = xdisplay.o xresman.o xhandler.o xwindow.o xdraw.o \
	button.o textip.o enumip.o menuwin.o scale.o slider.o scroll.o mclist.o meter.o
CLXCLIENT_H = clxclient.h

YASS_O = ringbuff.o jclient.o mainwin.o styles.o yass.o
YASS_H = ringbuff.h jclient.h mainwin.h styles.h

%.o : %.cc
	$(CXX) $(CPPFLAGS) -o $@ -c $<

yass:	$(CLTHREADS_O) $(CLXCLIENT_O) $(YASS_O)
	g++ $(LDFLAGS) -o yass $(CLTHREADS_O) $(CLXCLIENT_O) $(YASS_O) $(LDLIBS)

install:	yass
	/usr/bin/install -m 755 yass $(PREFIX)/bin

clean:
	/bin/rm -f *~ *.o *.a *.so yass

tarball:
	cd ..; \
	/bin/rm -f -r $(DISTDIR)*; \
	svn export yass $(DISTDIR); \
	tar cvf $(DISTDIR).tar $(DISTDIR); \
	bzip2 $(DISTDIR).tar; \
	/bin/rm -f -r $(DISTDIR);
