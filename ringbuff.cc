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


#include "ringbuff.h"


Ringbuff::Ringbuff (void) :
    _chan (0),
    _size (0),
    _data (0)
{
    reset ();
}


Ringbuff::~Ringbuff (void)
{
    delete[] _data;
}


void Ringbuff::init (unsigned int chan, unsigned int size, unsigned int fsam)
{
    _chan = chan;
    _size = size;
    _fsam = fsam;
    _data = new float [_chan * _size * sizeof (float)];
    reset ();
}


void Ringbuff::reset (void)
{
    _iwr = _ird = 0;
    _nwr = _nrd = 0;
}


