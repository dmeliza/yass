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
#include <math.h>
#include "styles.h"
#include "mainwin.h"


unsigned int Mainwin::_nppq [NSPEED] = { 1, 2, 3, 4, 6, 8, 12, 16 };


Mainwin::Mainwin (X_window *parent, X_resman *xresm, unsigned int ncapt, Ringbuff *cbuff) :
    X_window (parent, XPOS, YPOS, XDEF, YDEF, Colors.main_bg), _state (50),
    _plotmap (0),
    _plotgct (0),
    _cbuff (cbuff),
    _ncapt (ncapt)
{
    X_hints  H;
    char     s [1024];

    _xatom = XInternAtom (dpy (), "WM_DELETE_WINDOW", True);
    XSetWMProtocols (dpy (), win (), &_xatom, 1);
    _xatom = XInternAtom (dpy (), "WM_PROTOCOLS", True);

    H.position (XPOS, YPOS);
    H.size (XMIN, YMIN);
    H.minsize (XMIN, YMIN);
    H.maxsize (disp()->xsize(), disp()->ysize());
    H.sizeinc (20, 20);
    H.rname (xresm->rname ());
    H.rclas ("Jass");
    x_apply (&H); 
    sprintf (s, "%s-%s  [%s]", PROGNAME, VERSION, xresm->rname ());
    x_set_title (s);

    _xs = XDEF - RMAR;
    _ys = YDEF - BMAR;
    _plotwin = new X_window (this, 0, 0, _xs, _ys, 0);
    _plotwin->x_map ();     
    new_plotmap ();

    Bst0.size.x = 40;
    Bst0.size.y = 17;
    _b_agc = new X_tbutton (this, this, &Bst0, _xs - 100, _ys + 1, "AGC", 0, B_AGC);
    _b_agc->x_set_win_gravity (SouthEastGravity);
    _b_agc->x_map ();
    Bst0.size.x = 17;
    Bst0.size.y = 17;
    _b_smi = new X_ibutton (this, this, &Bst0, _xs - 50, _ys + 1, disp ()->image1515 (X_display::IMG_MI), B_SMI);
    _b_smi->x_set_win_gravity (SouthEastGravity);
    _b_smi->x_map ();
    _b_spl = new X_ibutton (this, this, &Bst0, _xs - 32, _ys + 1, disp ()->image1515 (X_display::IMG_PL), B_SPL);
    _b_spl->x_set_win_gravity (SouthEastGravity);
    _b_spl->x_map ();

    setcol (xresm->get (".trace.colors", 0));

    x_add_events (StructureNotifyMask);
    x_map ();
    _state = WAIT;
    _dtime = 10000;
}


Mainwin::~Mainwin (void)
{
    x_unmap ();
    del_plotmap ();
}


void Mainwin::handle_event (XEvent *E)
{
    switch (E->type)
    {
    case ConfigureNotify:
        resize ((XConfigureEvent *) E);
        break;

    case ClientMessage:
        clmesg ((XClientMessageEvent *) E);
        break;
    }
}


void Mainwin::resize (XConfigureEvent *E)
{
    if ((_xs != E->width - RMAR) || (_ys != E->height - BMAR))
    {
	_xs = E->width  - RMAR;
	_ys = E->height - BMAR;
	_plotwin->x_resize (_xs, _ys); 
	del_plotmap ();
	new_plotmap ();
	_clear = true;
    }
}


void Mainwin::clmesg (XClientMessageEvent *E)
{
    if (E->message_type == _xatom) _state = EXIT;
}


void Mainwin::new_plotmap (void)
{
    _plotmap = XCreatePixmap (dpy (), _plotwin->win (), _xs, _ys, disp ()->depth ());
    _plotgct = XCreateGC (dpy (), _plotmap, 0, NULL);
    XSetWindowBackgroundPixmap (dpy (), _plotwin->win (), _plotmap);
}


void Mainwin::del_plotmap (void)
{
    if (!  _plotmap) return;
    XFreePixmap (dpy (), _plotmap);
    XFreeGC (dpy (), _plotgct);
}


void Mainwin::handle_callb (int k, X_window *W, _XEvent *E )
{
    int       c, e, i;
    X_button  *B;
    
    c = X_callback::cb_class (k);
    e = X_callback::cb_event (k);
    switch (c)
    {
    case X_callback::BUTTON:
 	B = (X_button *) W;
        k = B->cbid ();
        if (e == X_button::RELSE)
	{
	    switch (k)
	    {
            case B_AGC:
                if (B->stat ()) B->set_stat (0);
                else
		{
                    B->set_stat (1);
                    for (i = 0; i < MAX_CHAN; i++) _ygain [i] = 1e2f;
		} 
		break;

            case B_SMI:
                if (_speed > 0) _speed--;
		break;

            case B_SPL:
                if (_speed < NSPEED - 1) _speed++;
		break;
	    }
	}
    }
}


int Mainwin::handle_time (void)
{
    unsigned int  b, n, r;
    float         t;

    if (_state == WAIT)
    {
	if (_cbuff->diff () >= (int)(_cbuff->size () / 2))
	{
            r = _cbuff->fsam ();
            _state = RUN;
            _quant = 480;
            if (r >  50000) _quant *= 2;
            if (r > 100000) _quant *= 2;
            _dtime = 1e6f * _quant / r;
            _clear = true; 
            _speed = 2;
	}
    }
    t = 0;   
    if (_state == RUN)
    {
        n = _cbuff->rd_avail ();
        b = _cbuff->size ();
        r = _cbuff->fsam ();
        if (n >= b)
	{
            _clear = true; 
	    _cbuff->rd_commit (b);
            return (int)(1e6f * b / r);
	}
        t = (n - b / 2.0f) / _quant;
        if      (t >  0.1f) t =  0.1f;
        else if (t < -0.1f) t = -0.1f;  
        _dtime *= 1 - 0.5e-4f * t;
        if (n >= _quant) update ();         
    }
//    printf ("%10d %10.1lf\n", n, _dtime);
    return (int)(_dtime * (1 - 1e-2f * t) + 0.5f);
}


void Mainwin::update (void)
{
    unsigned int  i, j, k, c, ppq, spp;
    int           n, n1, n2;
    float         *p, *p1, *p2;     
    float         y1, y2, m, g;
    float         yp1 [16], yp2 [16];
    XSegment      S [16];
    X_draw        D (dpy (), _plotmap, _plotgct, 0);

    ppq = _nppq [_speed];
    spp = _quant / ppq;
      
    c = _cbuff->chan (); 
    D.setcolor (Colors.trace_bg);
    if (_clear)
    {
        _ymult = (float)(4 - _ys) / (2 * c);
	for (i = 0; i < c; i++)
	{
	    _ybase [i] = (int)(2 + (2 * i + 1) * -_ymult + 0.5f);
            _ygain [i] = 1.0f;
	}
	D.fillrect (0, 0, _xs, _ys);
        _clear = false;
    }
    else
    {
	D.movepix (ppq, 0, _xs, _ys);
	D.fillrect (_xs - ppq, 0, _xs, _ys);
        _plotwin->x_clear ();    
        XFlush (dpy ());
    }
    _cbuff->rd_cnt (_quant, &n1, &n2); 
    for (i = 0; i < c; i++)
    {
	_cbuff->rd_ptr (i, &p1, &p2);
        p = p1;
        n = n1;
        m = 0;
        for (j = 0; j < ppq; j++)
	{
            y1 =  1e6;
            y2 = -1e6;
	    for (k = 0; k < spp; k++)
	    {
		if (*p < y1) y1 = *p;
		if (*p > y2) y2 = *p;
                if (--n) p++;
                else { p = p2; n = n2; }
	    }
            yp1 [j] = y1;
            yp2 [j] = y2;
            if (m < fabsf (y1)) m = fabsf (y1); 
            if (m < fabsf (y2)) m = fabsf (y2); 
	}
        if (_b_agc->stat ())
	{
            g = _ygain [i];
            if (m > 1e-15f) m = expf (0.667f * logf (m));
            else            m = 1e-10;
            if (m * g > 1.0f) g = 1.0f / m;
            else
            {
	        g *= 1.0116f;
	        if (g > 1e2f) g = 1e2f;
            }
            _ygain [i] = g;
	}
        else g = 1;
        g *= _ymult; 
        D.setcolor ((m < 1e-3) ? Colors.trace_c0 : _color [i]);
        for (j = 0; j < ppq; j++)
	{
            S [j].x1 = S [j].x2 = _xs - ppq + j;
            S [j].y1 = _ybase [i] + (short)(g * yp1 [j] + 0.5f);
            S [j].y2 = _ybase [i] + (short)(g * yp2 [j] + 0.5f);
	}
        D.drawsegments (ppq, S);
    }
    _cbuff->rd_commit (n1 + n2);
    _plotwin->x_clear ();    
    XFlush (dpy ());
}


void Mainwin::setcol (const char *pattern)
{
    int         i;
    const char  *p;

    if (pattern && *pattern)
    {
        p = pattern;
        for (i = 0; i < MAX_CHAN; i++)
	{
	    if (*p == 0) p = pattern;
            switch (*p++)
	    {
	    case '1': _color [i] = Colors.trace_c1; break;
	    case '2': _color [i] = Colors.trace_c2; break;
	    case '3': _color [i] = Colors.trace_c3; break;
	    case '4': _color [i] = Colors.trace_c4; break;
            default:  _color [i] = Colors.trace_c0;
	    }
	}
    }
    else for (i = 0; i < MAX_CHAN; i++) _color [i] = Colors.trace_c1;
}
