#include<stdio.h>
#include<X11/Xlib.h>
#include<stdlib.h>
#include<X11/extensions/Xrender.h>

int main(){
    //Display server
    Display *display = XOpenDisplay(NULL);
    if(display == NULL){
        fprintf(stderr, "Can't open display\n");
        return 1;
    }

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    XVisualInfo vinfo;
    if(!XMatchVisualInfo(
            display, screen, 32, TrueColor, &vinfo
        )){
        fprintf(
            stderr, 
            "No 32-bit visual found — is a compositor running?\n"
        );
        return 1;
    }

    XSetWindowAttributes attrs;
    attrs.colormap = XCreateColormap(
                    display, root,
                    vinfo.visual, AllocNone 
    );
    attrs.background_pixel = 0;
    attrs.border_pixel = 0;
    attrs.override_redirect = True;

    Window win = XCreateWindow(
                display, root,
                100, 100,
                800, 600,
                0,
                vinfo.depth,
                InputOutput,
                vinfo.visual,
                CWColormap | CWBackPixel | CWBorderPixel | CWOverrideRedirect,
                &attrs
            );
    
    GC gc = XCreateGC(display, win, 0, NULL); 
    XSelectInput(display, win, ExposureMask | ButtonPressMask | KeyPressMask);
    //XRaiseWindow(display, win);
    XMapWindow(display, win);
    XSetForeground(display, gc, 0xFF3333);

    XRectangle btn1 = {10, 10, 80, 30};
    XRectangle btn2 = {110, 10, 80, 30};

    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        
        if(event.type == Expose) {
            XClearWindow(display, win);
            XFillRectangle(display, win, gc, btn1.x, btn1.y, btn1.width, btn1.height);
            XFillRectangle(display, win, gc, btn2.x, btn2.y, btn2.width, btn2.height);
        }

        if(event.type == KeyPress){
            break;
        }
    }

    XFreeGC(display, gc);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
    return 0;
}
