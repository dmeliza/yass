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
#include "clxclient.h"
#include <jack/jack.h>
#include "ringbuff.h"
#include "global.h"


class Jclient
{
public:

    Jclient (ITC_ctrl *cmain, X_resman *xresm, unsigned int chan, Ringbuff *buff);
    ~Jclient (void);

    void start (void) { _cbuff->reset (); _running = true; }
    void stop (void) { _running = false; }

private:

    void  init_jack (void);
    void  close_jack (void);
    void  jack_shutdown (void);
    int   jack_callback (jack_nframes_t nframes);

    X_resman       *_xresm;
    ITC_ctrl       *_cmain;
    Ringbuff       *_cbuff;
    bool            _running;
    jack_client_t  *_jack_client;
    jack_port_t    *_jack_ports [MAX_CHAN];
    unsigned int    _fsamp;
    unsigned int    _fsize;
    unsigned int    _ncapt;

    static void jack_static_shutdown (void *arg);
    static int  jack_static_callback (jack_nframes_t nframes, void *arg);
};


