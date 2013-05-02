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


#ifndef __MAINWIN_H
#define __MAINWIN_H


#include <clxclient.h>
#include "ringbuff.h"
#include "global.h"


#define XPOS  100
#define YPOS  100
#define XMIN  240
#define YMIN  160
#define XDEF  300
#define YDEF  200
#define XMAX 1200
#define YMAX  800
#define RMAR    0
#define BMAR   18


class Mainwin : public X_window, public X_callback
{
public:

    Mainwin (X_window *parent, X_resman *xresm, unsigned int ncapt, Ringbuff *cbuff);
    ~Mainwin (void);
    bool running (void) const { return _state != EXIT; }
    int  handle_time (void);
    void handle_exit (void) { _state = EXIT; }
 
private:

    enum { WAIT, RUN, EXIT, NSPEED = 8 };
    enum { B_AGC, B_SMI, B_SPL };

    virtual void handle_event (XEvent *xe);
    virtual void handle_callb (int, X_window*, _XEvent*);

    void new_plotmap (void);
    void del_plotmap (void);
    void clmesg (XClientMessageEvent *);
    void resize (XConfigureEvent *);
    void update (void);
    void setcol (const char *);

    int            _xs;
    int            _ys;
    int            _state;
    Atom           _xatom;
    X_window      *_plotwin;
    Pixmap         _plotmap;
    GC             _plotgct;
    X_button      *_b_agc;
    X_button      *_b_smi;
    X_button      *_b_spl;
    Ringbuff      *_cbuff;
    bool           _clear;
    float          _dtime;
    unsigned int   _ncapt;
    unsigned int   _quant;
    unsigned int   _speed;
    unsigned int   _ybase [MAX_CHAN];
    unsigned long  _color [MAX_CHAN];         
    float          _ymult;
    float          _ygain [MAX_CHAN];

    static unsigned int _nppq [NSPEED];
};


#endif
