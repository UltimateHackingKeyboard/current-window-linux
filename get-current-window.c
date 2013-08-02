#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <stdio.h>

#define MAXSTR 1000

long unsigned int get_float_property(unsigned char *property)
{
    return property[0] + (property[1]<<8) + (property[2]<<16) + (property[3]<<24);
}

int main(int argc, char** argv)
{
    char *display_name = NULL;  // could be the value of $DISPLAY
    Display *display;
    Atom *atoms, *atom, actual_type, filter_atom;
    int count, i;
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
    unsigned long application_window = get_float_property(prop);

    atoms = XListProperties(display, application_window, &count);

    for (i=0; i<count; i++) {
        atom = atoms + i;
        status = XGetWindowProperty(display, application_window, *atom, 0, MAXSTR, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
        if (status == BadWindow) {
            printf("window id # 0x%lx does not exists!", application_window);
        }

        if (status != Success) {
            printf("XGetWindowProperty failed!");
        }

        printf("* property:%39s | propertyName:%s\n  atom:%s | actual_format:%i | nitems:%lu | bytes_after:%lu\n",
                XGetAtomName(display, *atom), prop, XGetAtomName(display, actual_type), actual_format, nitems, bytes_after);
    }
//    XA_WM_ICON_NAME
//    _NET_WM_PID(CARDINAL) = 6379
//    WM_CLASS(STRING) = "Tomboy", "Tomboy"
//	XFree(name);

    XCloseDisplay (display );

    return 0;

}
// _NET_ACTIVE_WINDOW
