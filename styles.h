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


#ifndef __STYLES_H
#define __STYLES_H


#include "clxclient.h"


struct colors
{
    unsigned long   white;
    unsigned long   black;
    unsigned long   main_bg;
    unsigned long   main_ds;
    unsigned long   main_ls;
    unsigned long   trace_bg;
    unsigned long   trace_gr;
    unsigned long   trace_c0;
    unsigned long   trace_c1;
    unsigned long   trace_c2;
    unsigned long   trace_c3;
    unsigned long   trace_c4;
    unsigned long   butt_bg0;
    unsigned long   butt_bg1;
};


struct fonts 
{
};


struct xft_colors
{
    XftColor  *white;
    XftColor  *black;
    XftColor  *main_fg;
    XftColor  *text_fg;
    XftColor  *butt_fg0;
    XftColor  *butt_fg1;
};


struct xft_fonts 
{
    XftFont   *button;
    XftFont   *labels;
    XftFont   *scales;
};


extern struct colors       Colors;
extern struct fonts        Fonts;
extern struct xft_colors   XftColors;
extern struct xft_fonts    XftFonts;
extern X_button_style      Bst0;
extern X_textln_style      Tst0;


extern void init_styles (X_display *disp, X_resman *xrm);


#endif
