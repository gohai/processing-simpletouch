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

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import processing.core.*;


/**
 *  Class for getting touch events on Linux
 */
public class SimpleTouch {

  protected PApplet parent;
  protected String dev;
  protected long ptr;


  /**
   *  Opens a touch input device
   *  @param parent typically use "this"
   *  @param dev interface name
   *  @see list
   */
  public SimpleTouch(PApplet parent, String dev) {
    if (!"Linux".equals(System.getProperty("os.name"))) {
      System.err.println("The Simple Touch library is only supported on Linux");
      return;
    }
    System.loadLibrary("processing-simpletouch");

    this.parent = parent;
    this.dev = dev;
    ptr = openDevice("/dev/input/" + dev);
    if (ptr == 0) {
      throw new RuntimeException("Failed to initialize libevdev");
    }
  }


  /**
   *  Opens the first input device
   *  @param parent typically use "this"
   */
  public SimpleTouch(PApplet parent) {
    this(parent, "event0");
  }


  /**
   *  Closes the input device
   */
  public void close() {
    if (ptr == 0) {
      return;
    }
    closeDevice(ptr);
    ptr = 0;
  }


  public void dispose() {
    close();
  }


  /**
   *  Lists all available input devices
   *  @return String array
   */
  public static String[] list() {
    ArrayList<String> devs = new ArrayList<String>();
    File dir = new File("/dev/input");
    File[] files = dir.listFiles();
    if (files != null) {
      for (File file : files) {
        if (file.getName().startsWith("event")) {
          devs.add(file.getName());
        }
      }
    }
    // listFiles() does not guarantee ordering
    String[] tmp = devs.toArray(new String[devs.size()]);
    Arrays.sort(tmp);
    return tmp;
  }


  /**
   *  Returns the device name of the input device
   *  @return String
   */
  public String name() {
    if (ptr == 0) {
      return "";
    }
    String name = getName(ptr);
    if (name == null) {
      return "";
    } else {
      return name;
    }
  }


  /**
   *  Returns an array of current touches registered
   *  @return array of SimpleTouchEvt
   */
  public SimpleTouchEvt[] touches() {
    if (ptr == 0) {
      return new SimpleTouchEvt[0];
    }
    return getTouches(ptr);
  }


  protected static native long openDevice(String fn);
  protected static native String getName(long ptr);
  protected static native SimpleTouchEvt[] getTouches(long ptr);
  protected static native void closeDevice(long ptr);
}
