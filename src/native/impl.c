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

#include <errno.h>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "iface.h"


JNIEXPORT jlong JNICALL Java_gohai_simpletouch_SimpleTouch_openDevice
  (JNIEnv *env, jclass cls, jstring _fn)
{
	const char *fn = (*env)->GetStringUTFChars(env, _fn, JNI_FALSE);
	int fd = open(fn, O_RDONLY|O_NONBLOCK);
	(*env)->ReleaseStringUTFChars(env, _fn, fn);

	struct libevdev *dev;
	int ret = libevdev_new_from_fd(fd, &dev);
	if (ret < 0) {
		fprintf(stderr, "%s\n", strerror(-ret));
		close(fd);
		return (uintptr_t)NULL;
	}

	if (!libevdev_has_event_type(dev, EV_ABS)) {
		fprintf(stderr, "Device does not report absolut axis value changes. Not a touch screen device?\n");
		close(fd);
		return (uintptr_t)NULL;
	}

	return (uintptr_t)dev;
}

JNIEXPORT jstring JNICALL Java_gohai_simpletouch_SimpleTouch_getName
  (JNIEnv *env, jclass cls, jlong ptr)
{
	struct libevdev *dev = (struct libevdev*)(uintptr_t)ptr;
	const char *name = libevdev_get_name(dev);
	if (name) {
		return (*env)->NewStringUTF(env, name);
	} else {
		return NULL;
	}
}

JNIEXPORT jobjectArray JNICALL Java_gohai_simpletouch_SimpleTouch_getTouches
  (JNIEnv *env, jclass cls, jlong ptr)
{
	struct libevdev *dev = (struct libevdev*)(uintptr_t)ptr;
	int ret;
	int xAxisLimits[2];
	int yAxisLimits[2];

	// get limits (could be pre-computed)
	xAxisLimits[0] = libevdev_get_abs_minimum(dev, ABS_X);
	xAxisLimits[1] = libevdev_get_abs_maximum(dev, ABS_X);
	yAxisLimits[0] = libevdev_get_abs_minimum(dev, ABS_Y);
	yAxisLimits[1] = libevdev_get_abs_maximum(dev, ABS_Y);

	// read in events
	do {
		struct input_event ev;
		ret = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
		//if (ret == 0) {
		//	fprintf(stderr, "got event\n");
		//}
	} while (ret == 1 || ret == 0);

	int numTouches = 0;
	for (int i=0; i < libevdev_get_num_slots(dev); i++) {
		if (libevdev_get_slot_value(dev, i, ABS_MT_TRACKING_ID) != -1) {
			numTouches++;
		}
	}

	// could be pre-computed
	jclass touchEventCls = (*env)->FindClass(env, "gohai/simpletouch/SimpleTouchEvt");
	jmethodID touchEventCtorId = (*env)->GetMethodID(env, touchEventCls, "<init>", "()V");
	jfieldID touchEventXId = (*env)->GetFieldID(env, touchEventCls, "x", "F");
	jfieldID touchEventYId = (*env)->GetFieldID(env, touchEventCls, "y", "F");
	jfieldID touchEventIdId = (*env)->GetFieldID(env, touchEventCls, "id", "I");

	jobjectArray touchEvents = (*env)->NewObjectArray(env, numTouches, touchEventCls, NULL);
	int inArray = 0;
	for (int i=0; i < libevdev_get_num_slots(dev) && inArray < numTouches; i++) {
		int id = libevdev_get_slot_value(dev, i, ABS_MT_TRACKING_ID);
		if (id == -1) {
			// nothing in current slot
			continue;
		}
		jobject touchEvent = (*env)->NewObject(env, touchEventCls, touchEventCtorId);
		// fill in values
		int _x = libevdev_get_slot_value(dev, i, ABS_MT_POSITION_X);
		float x = (_x-xAxisLimits[0]) / ((float)(xAxisLimits[1]-xAxisLimits[0]));
		(*env)->SetFloatField(env, touchEvent, touchEventXId, x);
		int _y = libevdev_get_slot_value(dev, i, ABS_MT_POSITION_Y);
		float y = (_y-yAxisLimits[0]) / ((float)(yAxisLimits[1]-yAxisLimits[0]));
		(*env)->SetFloatField(env, touchEvent, touchEventYId, y);
		(*env)->SetIntField(env, touchEvent, touchEventIdId, id);
		// this is all the Raspberry Pi display supports, but more
		// properties could be added (e.g. pressure)
		(*env)->SetObjectArrayElement(env, touchEvents, inArray++, touchEvent);
	}

	return touchEvents;
}

JNIEXPORT void JNICALL Java_gohai_simpletouch_SimpleTouch_closeDevice
  (JNIEnv *env, jclass cls, jlong ptr)
{
	struct libevdev *dev = (struct libevdev*)(uintptr_t)ptr;
	int fd = libevdev_get_fd(dev);
	libevdev_free(dev);
	close(fd);
}
