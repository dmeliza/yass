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


PREFIX = /usr
SUFFIX := $(shell uname -m | sed -e 's/^unknown/$//' -e 's/^i.86/$//' -e 's/^x86_64/$/64/')
LIBDIR = lib$(SUFFIX)
VERSION = 0.0.2
DISTDIR = yass-$(VERSION)
CPPFLAGS += -O3 -Wall -DVERSION=\"$(VERSION)\" -DPREFIX=\"$(PREFIX)\" `freetype-config --cflags` `pkg-config jack --cflags`


LDFLAGS += -L$(PREFIX)/$(LIBDIR) -L/usr/X11R6/$(LIBDIR)
LDLIBS += -lclthreads -lclxclient -lX11 `freetype-config --libs` `pkg-config jack --libs`

YASS_O = ringbuff.o jclient.o mainwin.o styles.o yass.o
YASS_H = ringbuff.h jclient.h mainwin.h styles.h


yass:	$(YASS_O)
	g++ $(LDFLAGS) -o yass $(YASS_O) $(LDLIBS)

$(YASS_O):	$(YASS_H)


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
