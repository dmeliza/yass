/*
    Copyright (C) 2006 Fons Adriaensen <fons.adriaensen@skynet.be>
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#include <stdlib.h>
#include <stdio.h>
#include <clthreads.h>
#include "styles.h"
#include "mainwin.h"
#include "jclient.h"


#define NOPTS 3


XrmOptionDescRec options [NOPTS] =
{
    {"-h", ".help",         XrmoptionNoArg,  "true"  },
    {"-n", ".nchan",        XrmoptionSepArg,  0      },
    {"-c", ".trace.colors", XrmoptionSepArg,  0      }
};


static void help (void)
{
    fprintf (stderr, "\nYass-%s\n\n", VERSION);
    fprintf (stderr, "  (C) 2006 Fons Adriaensen  <fons.adriaensen@skynet.be>\n\n");
    fprintf (stderr, "Options:\n");
    fprintf (stderr, "  -h   Display this text\n");
    fprintf (stderr, "  -n   Number of channels\n");
    fprintf (stderr, "  -c   Trace color pattern\n");
    exit (1);
}


int main (int ac, char *av [])
{
    X_resman       xresman;
    X_display     *display;
    X_handler     *handler;
    X_rootwin     *rootwin;
    Jclient       *jclient;
    Mainwin       *mainwin;
    ITC_ctrl       cmain;
    Ringbuff       cbuff;
    unsigned int   nchan;
    const char    *p;

    xresman.init (&ac, av, "yass", options, NOPTS);
    if (xresman.getb (".help", 0)) help ();
            
    display = new X_display (xresman.get (".display", 0));
    if (display->dpy () == 0)
    {
	fprintf (stderr, "Can't open display.\n");
        delete display;
	exit (1);
    }

    p = xresman.get (".nchan", 0);
    if (! p || sscanf (p, "%d", &nchan) != 1) nchan = 2;
    if (nchan > MAX_CHAN) nchan = MAX_CHAN;
    jclient = new Jclient (&cmain, &xresman, nchan, &cbuff);

    init_styles (display, &xresman);
    rootwin = new X_rootwin (display);
    mainwin = new Mainwin (rootwin, &xresman, nchan, &cbuff);

    handler = new X_handler (display, &cmain, EV_X11);
    handler->next_event ();
    XFlush (display->dpy ());

    cmain.set_time (0);
    cmain.inc_time (10000);
    jclient->start ();

    while (mainwin->running ())
    {
	switch (cmain.get_event_timed ())
	{
        case Esync::EV_TIME:
            cmain.inc_time (mainwin->handle_time ());
            break;
        case EV_EXIT:
            mainwin->handle_exit ();
            break;
	case EV_X11:
	    rootwin->handle_event ();
	    handler->next_event ();
            break;
	}
    }

    jclient->stop ();
    delete jclient;
    delete handler;
    delete mainwin;
    delete rootwin;
    delete display;
   
    return 0;
}



