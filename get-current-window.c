#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXSTR 1000

long unsigned int get_float_property(unsigned char *property)
{
    return property[0] + (property[1]<<8) + (property[2]<<16) + (property[3]<<24);
}

void check_status(int status, unsigned long window)
{
    if (status == BadWindow) {
        printf("window id # 0x%lx does not exists!", window);
        exit(1);
    }

    if (status != Success) {
        printf("XGetWindowProperty failed!");
        exit(2);
    }
}

int main(int argc, char** argv)
{
    char *display_name = NULL;  // could be the value of $DISPLAY
    Display *display;
    Atom actual_type, filter_atom;
    int actual_format;
    int status;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop;

    display = XOpenDisplay(display_name);
    if (display == NULL) {
        fprintf (stderr, "%s:  unable to open display '%s'\n", argv[0], XDisplayName (display_name));
    }
    int screen = XDefaultScreen(display);
    unsigned long root_window = RootWindow(display, screen);

    filter_atom = XInternAtom(display, "_NET_ACTIVE_WINDOW", True);
    status = XGetWindowProperty(display, root_window, filter_atom, 0, MAXSTR, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
    check_status(status, root_window);
    unsigned long application_window = get_float_property(prop);

    filter_atom = XInternAtom(display, "_NET_WM_PID", True);
    status = XGetWindowProperty(display, application_window, filter_atom, 0, MAXSTR, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
    check_status(status, application_window);
    unsigned long application_pid = get_float_property(prop);
    printf("pid: %lu\n", application_pid);

    filter_atom = XInternAtom(display, "WM_CLASS", True);
    status = XGetWindowProperty(display, application_window, filter_atom, 0, MAXSTR, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
    check_status(status, application_window);
    printf("WM_CLASS: %s\n", prop);

    filter_atom = XInternAtom(display, "_NET_WM_NAME", True);
    status = XGetWindowProperty(display, application_window, filter_atom, 0, MAXSTR, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
    check_status(status, application_window);
    printf("_NET_WM_NAME :%s\n", prop);

    XCloseDisplay (display );
    return 0;
}
