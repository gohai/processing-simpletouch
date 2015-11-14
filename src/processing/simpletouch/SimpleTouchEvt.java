/* -*- mode: java; c-basic-offset: 2; indent-tabs-mode: nil -*- */

/*
  Copyright (c) The Processing Foundation 2015
  Simple Touch library developed by Gottfried Haider 2015

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

package processing.simpletouch;


/**
 *  @webref
 */
public class SimpleTouchEvt {

  // from device

  /**
   *  ABS_MT_POSITION_X mapped to 0.0 - 1.0
   */
  public float	x;
  /**
   *  ABS_MT_POSITION_Y mapped to 0.0 - 1.0
   */
  public float y;
  /**
   * ABS_MT_TRACKING_ID
   */ 
  public int id;
  
}
