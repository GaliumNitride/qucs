/*
 * ipulse.cpp - pulse current source class implementation
 *
 * Copyright (C) 2004 Stefan Jahn <stefan@lkcc.org>
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this package; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.  
 *
 * $Id: ipulse.cpp,v 1.1 2004/09/20 10:09:55 ela Exp $
 *
 */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "complex.h"
#include "object.h"
#include "node.h"
#include "circuit.h"
#include "component_id.h"
#include "ipulse.h"

ipulse::ipulse () : circuit (2) {
  setS (1, 1, 1.0);
  setS (1, 2, 0.0);
  setS (2, 1, 0.0);
  setS (2, 2, 1.0);
  type = CIR_IPULSE;
}

void ipulse::initDC (void) {
  nr_double_t i = getPropertyDouble ("I1");
  setI (1, +i); setI (2, -i);
}

void ipulse::initAC (void) {
  clearI ();
}

void ipulse::initTR (void) {
  initDC ();
}

void ipulse::calcTR (nr_double_t t) {
  nr_double_t i1 = getPropertyDouble ("I1");
  nr_double_t i2 = getPropertyDouble ("I2");
  nr_double_t t1 = getPropertyDouble ("T1");
  nr_double_t t2 = getPropertyDouble ("T2");
  nr_double_t tr = getPropertyDouble ("Tr");
  nr_double_t tf = getPropertyDouble ("Tf");
  nr_double_t it = 0;

  if (t < t1) { // before pulse
    it = i1;
  }
  else if (t >= t1 && t < t1 + tr) { // rising edge
    it = i1 + (i2 - i1) / tr * (t - t1);
  }
  else if (t >= t1 + tr && t < t2 - tf) { // during full pulse
    it = i2;
  }
  else if (t >= t2 - tf && t < t2) { // falling edge
    it = i2 + (i1 - i2) / tf * (t - (t2 - tf));
  }
  else { // after pulse
    it = i1;
  }
  setI (1, +it); setI (2, -it);
}