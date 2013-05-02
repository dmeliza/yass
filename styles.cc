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


#include "styles.h"


struct colors      Colors;
struct fonts       Fonts;
struct xft_colors  XftColors;
struct xft_fonts   XftFonts;
XftColor          *cscale [17];
X_button_style     Bst0;
X_textln_style     Tst0;


void init_styles (X_display *disp, X_resman *xrm)
{
    Colors.black     = disp->blackpixel ();
    Colors.white     = disp->whitepixel ();
    Colors.main_bg   = disp->alloc_color (xrm->get (".color.main.bg",   "gray30"),    Colors.white);
    Colors.main_ds   = disp->alloc_color (xrm->get (".color.main.ds",   "gray20"),    Colors.black);
    Colors.main_ls   = disp->alloc_color (xrm->get (".color.main.ls",   "white"),     Colors.white);
    Colors.trace_bg  = disp->alloc_color (xrm->get (".color.trace.bg",  "black"),     Colors.black);
    Colors.trace_gr  = disp->alloc_color (xrm->get (".color.trace.gr",  "gray20"),    Colors.white);
    Colors.trace_c0  = disp->alloc_color (xrm->get (".color.trace.c0",  "gray70"),    Colors.white);
    Colors.trace_c1  = disp->alloc_color (xrm->get (".color.trace.c1",  "green"),     Colors.white);
    Colors.trace_c2  = disp->alloc_color (xrm->get (".color.trace.c2",  "yellow"),    Colors.white);
    Colors.trace_c3  = disp->alloc_color (xrm->get (".color.trace.c3",  "red"),       Colors.white);
    Colors.trace_c4  = disp->alloc_color (xrm->get (".color.trace.c4",  "blue"),      Colors.white);
    Colors.butt_bg0  = disp->alloc_color (xrm->get (".color.butt.bg0",  "#e0d8d0"),   Colors.white);
    Colors.butt_bg1  = disp->alloc_color (xrm->get (".color.butt.bg1",  "yellow"),    Colors.white);

    XftColors.white    = disp->alloc_xftcolor ("white", 0);
    XftColors.black    = disp->alloc_xftcolor ("black", 0);
    XftColors.main_fg  = disp->alloc_xftcolor (xrm->get (".color.main.fg",   "black"), XftColors.black);
    XftColors.text_fg  = disp->alloc_xftcolor (xrm->get (".color.text.fg",   "black"), XftColors.black);
    XftColors.butt_fg0 = disp->alloc_xftcolor (xrm->get (".color.butt.fg0",  "black"), XftColors.black);
    XftColors.butt_fg1 = disp->alloc_xftcolor (xrm->get (".color.butt.fg1",  "black"), XftColors.black);
 
    XftFonts.button = disp->alloc_xftfont (xrm->get (".font.button", "luxi:pixelsize=12"));
    XftFonts.labels = disp->alloc_xftfont (xrm->get (".font.labels", "luxi:pixelsize=11"));
    XftFonts.scales = disp->alloc_xftfont (xrm->get (".font.scales", "luxi:pixelsize=9"));

    Bst0.font = XftFonts.button;
    Bst0.color.bg [0] = Colors.butt_bg0;
    Bst0.color.fg [0] = XftColors.butt_fg0;
    Bst0.color.bg [1] = Colors.butt_bg1;
    Bst0.color.fg [1] = XftColors.butt_fg1;
    Bst0.color.shadow.bgnd = Colors.main_bg;
    Bst0.color.shadow.lite = Colors.main_ls;
    Bst0.color.shadow.dark = Colors.main_ds;
    Bst0.type = X_button_style::RAISED;

    Tst0.font = XftFonts.labels;
    Tst0.color.normal.bgnd = Colors.white;
    Tst0.color.normal.text = XftColors.text_fg;
    Tst0.color.shadow.lite = Colors.main_bg;
    Tst0.color.shadow.dark = Colors.main_bg;
    Tst0.color.shadow.bgnd = Colors.main_bg;
}

