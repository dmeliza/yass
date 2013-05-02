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


#ifndef __RINGBUFF_H
#define __RINGBUFF_H


class Ringbuff
{
public:

    Ringbuff (void);
    ~Ringbuff (void);
    
    void init (unsigned int chan, unsigned int size, unsigned int fsam);
    void reset (void);

    unsigned int chan (void) const { return _chan; }
    unsigned int size (void) const { return _size; }
    unsigned int fsam (void) const { return _fsam; }

    int diff (void) const { return _nwr - _nrd; }
     
    int  wr_avail (void) const { int n = _size - _nwr + _nrd; return (n < 0) ? 0 : n; }
    int  rd_avail (void) const { int n =         _nwr - _nrd; return (n < 0) ? 0 : n; }
         
    void wr_commit (unsigned int k) { _nwr += k; _iwr += k; while (_iwr >= _size) _iwr -= _size; }
    void rd_commit (unsigned int k) { _nrd += k; _ird += k; while (_ird >= _size) _ird -= _size; }        

    void wr_cnt (unsigned int n, int *n1, int *n2) const
    {
        unsigned int k = _size - _iwr;
        if (k >= n) { *n1 = n; *n2 = 0;	}      
        else   	    { *n1 = k; *n2 = n - k; }                     
    } 
    void rd_cnt (unsigned int n, int *n1, int *n2) const
    {
        unsigned int k = _size - _ird;
        if (k >= n) { *n1 = n; *n2 = 0;	}      
        else   	    { *n1 = k; *n2 = n - k; }                     
    } 

    void wr_ptr (unsigned int c, float **p1, float **p2) const
    {
        float *p = _data + c * _size; 
        *p1 = p + _iwr;
        *p2 = p;
    }  
    void rd_ptr (unsigned int c, float **p1, float **p2) const
    {
        float *p = _data + c * _size; 
        *p1 = p + _ird;
        *p2 = p;
    }  

private:

    unsigned int     _chan;
    unsigned int     _size;
    unsigned int     _fsam;
    unsigned int     _iwr;
    unsigned int     _ird;
    float           *_data;
    int              _nwr;
    int              _nrd;
};


#endif
