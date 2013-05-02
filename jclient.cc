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


#include <string.h>
#include "jclient.h"


Jclient::Jclient (ITC_ctrl *cmain, X_resman *xresm, unsigned int ncapt, Ringbuff *cbuff) :
    _xresm (xresm),
    _cmain (cmain),
    _cbuff (cbuff),
    _running (false),
    _jack_client (0),
    _ncapt (ncapt)
{
    init_jack ();   
}


Jclient::~Jclient (void)
{
    if (_jack_client) close_jack ();
}


void Jclient::init_jack (void)
{
    unsigned int  i, k, q;
    char          s [256];
    const char    *p;
    
    if ((_jack_client = jack_client_new (_xresm->rname ())) == 0)
    {
        fprintf (stderr, "Can't connect to JACK.\n");
        exit (1);
    }

    jack_set_process_callback (_jack_client, jack_static_callback, (void *) this);
    jack_on_shutdown (_jack_client, jack_static_shutdown, (void *) this);

    for (i = 0; i < _ncapt; i++)
    {
        sprintf(s, "in_%d", i + 1);
        _jack_ports [i] = jack_port_register (_jack_client, s, JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    }

    if (jack_activate (_jack_client))
    {
        fprintf(stderr, "Can't activate JACK.\n");
        exit (1);
    }

    for (i = 0; i < _ncapt; i++)
    {
        sprintf(s, ".input_%d", i + 1);
        p = _xresm->get (s, 0); 
        sprintf(s, "%s:in_%d", _xresm->rname (), i + 1);
        if (p && *p && jack_connect (_jack_client, p, s))
	{
            fprintf(stderr, "Can't connect %s to %s.\n", p, s);
	} 
    }

    _fsamp = jack_get_sample_rate (_jack_client);
    _fsize = jack_get_buffer_size (_jack_client);

    k = 512;
    if (_fsamp >  50000) k *= 2;
    if (_fsamp > 100000) k *= 2;
    q = (_fsize > k) ? _fsize : k;
    _cbuff->init (_ncapt, 4 * q, _fsamp);
}


void Jclient::close_jack ()
{
    unsigned int i;

    jack_deactivate (_jack_client);
    for (i = 0; i < _ncapt; i++) jack_port_unregister (_jack_client, _jack_ports [i]);
    jack_client_close (_jack_client);
}


void Jclient::jack_static_shutdown (void *arg)
{
    return ((Jclient *) arg)->jack_shutdown ();
}


void Jclient::jack_shutdown (void)
{
    _cmain->put_event (EV_EXIT);
}


int Jclient::jack_static_callback (jack_nframes_t nframes, void *arg)
{
    return ((Jclient *) arg)->jack_callback (nframes);
}


int Jclient::jack_callback (jack_nframes_t nframes)
{
    unsigned int  i;
    int           n1, n2;
    float         *d1, *d2, *s;

    if (_running)
    {
        _cbuff->wr_cnt (nframes, &n1, &n2);
        for (i = 0; i < _ncapt; i++)
	{
	    _cbuff->wr_ptr (i, &d1, &d2); 
   	    s = (float *)(jack_port_get_buffer (_jack_ports [i], nframes));
   	    if (n1) memcpy (d1, s,      n1 * sizeof (float));
   	    if (n2) memcpy (d2, s + n1, n2 * sizeof (float));
	}
        _cbuff->wr_commit (n1 + n2);
    }
    return 0;
}


