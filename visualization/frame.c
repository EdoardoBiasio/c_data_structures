#include "visualization.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(__APPLE__) || defined(__MACH__) || defined(__linux__)

int frame_create(const char* name, int width, int height, void (*draw)(Display*, Window, GC, void*), void* data, frame_t* frame) {
    memcpy(frame->name, name, 255);
    frame->name[255] = '\0';
    frame->width = width;
    frame->height = height;
    frame->draw = draw;
    frame->data = data;
    return 0;
}
pid_t pid;

int frame_close(frame_t *frame) {
    
    return 0;
}

void frame_loop(frame_t *frame) {
    XEvent event;
    int window_open = 1;
    XKeyPressedEvent *key_pressed;
    XClientMessageEvent *client_event;
    XSetWindowAttributes attribs;
    XConfigureEvent *notify_event;
    memset(&attribs, 0, sizeof(XSetWindowAttributes));
    attribs.background_pixel = 0xffafe9af;
    attribs.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask | ExposureMask;
    frame->display = XOpenDisplay(0);
    frame->window = XDefaultRootWindow(frame->display);
    frame->main_window = XCreateWindow(frame->display, frame->window, 0, 0, frame->width, frame->height, 0, CopyFromParent, CopyFromParent, CopyFromParent, CWBackPixel | CWEventMask, &attribs);
    frame->gc = XCreateGC(frame->display, frame->main_window, 0, NULL);
    XMapWindow(frame->display, frame->main_window);
    XStoreName(frame->display, frame->main_window, frame->name);
    frame->WM_DELETE_WINDOW = XInternAtom(frame->display, "WM_DELETE_WINDOW", False);
    usleep(40000);
    frame->draw(frame->display, frame->main_window, frame->gc, frame->data);
    if (!XSetWMProtocols(frame->display, frame->main_window, &frame->WM_DELETE_WINDOW, 1))
	printf("Error\n");
    
    while (window_open) {
	memset(&event, 0, sizeof(XEvent));
	XNextEvent(frame->display, &event);

	switch (event.type) {
	case KeyPress:
	case KeyRelease:
	    {
		key_pressed = (XKeyPressedEvent*) &event;
		if (key_pressed->keycode == XKeysymToKeycode(frame->display, XK_Escape))
		    window_open = 0;
	    }
	    break;
	case ClientMessage:
	    {
		client_event = (XClientMessageEvent*) &event;
		if ((Atom)client_event->data.l[0] == frame->WM_DELETE_WINDOW) {
		    window_open = 0;
		}
	    }
	    break;
	case ConfigureNotify:
	    {
		notify_event = (XConfigureEvent*) &event;
		if (notify_event->width != frame->width || notify_event->height != frame->height) {
		    frame->width = notify_event->width;
		    frame->height = notify_event->height;
		    XClearWindow(frame->display, frame->main_window);
		    frame->draw(frame->display, frame->main_window, frame->gc, frame->data);
		}

	    }
	    break;
	}
    }

    XDestroyWindow(frame->display, frame->main_window);
    XCloseDisplay(frame->display);
    exit(0);
}

int frame_display(frame_t *frame) {
    pid = fork();
    if (pid == 0)
	frame_loop(frame);
    return 0;
}

#elif defined(_WIN32) || defined(_WIN64) || defined(CYGWIN)

int frame_create(const char* name, int width, int height, void (*draw)(Display*, Window, GC, void*), void* data, frame_t* frame) {

    return 0;
}
pid_t pid;

int frame_close(frame_t *frame) {
    
    return 0;
}

void frame_loop(frame_t *frame) {

    exit(0);
}

int frame_display(frame_t *frame) {

    return 0;
}
#else

int frame_create(const char* name, int width, int height, void (*draw)(Display*, Window, GC, void*), void* data, frame_t* frame) {

    return 0;
}
pid_t pid;

int frame_close(frame_t *frame) {
    
    return 0;
}

void frame_loop(frame_t *frame) {

    exit(0);
}

int frame_display(frame_t *frame) {

    return 0;
}
#endif

