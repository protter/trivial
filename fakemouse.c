/* fakeMouse.c
compile it: cc fakemouse.c -lX11 -lXtst -o fakemouse */

#include <stdio.h>
#include <X11/extensions/XTest.h>  
#include <unistd.h>  
  
int main(int argc, char *argv[])  
{  
    Display *dpy = NULL;  
    XEvent event;  
    int xoffset= 0;
    int yoffset = 0;
    int yrange = 0;
    int xrange = 0;
    dpy = XOpenDisplay (NULL);  
    int i = 0;
    
    if((argc > 1) && (NULL != argv[1]) && (NULL != argv[2]))
    {
        xrange = atoi(argv[1]);
        yrange = atoi(argv[2]);
    }
    else 
    {
        xrange = 256;
        yrange = 128;
    }    
    
    if(!dpy)
    {
        printf("No DISPLAY!, Please export yours. For example: export DISPLAY=:0.0\n");
		return -1;
    }
    
    /* Get the current pointer position */  
    XQueryPointer(dpy, RootWindow(dpy, 0),  
                  &event.xbutton.root, &event.xbutton.window,  
                  &event.xbutton.x_root, &event.xbutton.y_root,  
                  &event.xbutton.x, &event.xbutton.y,  
                  &event.xbutton.state);  
  
    /* Fake the pointer movement to new relative position */  
    while(1){
        //dpy = XOpenDisplay (NULL);
        usleep(1000);
        XQueryPointer (dpy, RootWindow (dpy, 0),
                       &event.xbutton.root, &event.xbutton.window,
                       &event.xbutton.x_root, &event.xbutton.y_root,
                       &event.xbutton.x, &event.xbutton.y,
                       &event.xbutton.state);
                       
        for(yoffset = 0; yoffset < yrange; yoffset++)
        {
            for(xoffset = 0; xoffset < xrange; xoffset+=2)
            {
                //usleep(1000);
                XTestFakeMotionEvent(dpy, 0, (event.xbutton.x + (xrange - xoffset)),  
                                     event.xbutton.y + yoffset, CurrentTime);  
                XSync(dpy, 0);
                //usleep(1000);
                XTestFakeMotionEvent(dpy, 0, event.xbutton.x + xoffset,  
                                     event.xbutton.y + yoffset, CurrentTime);
                XSync(dpy, 0);
            }                  
        }
        
        XTestFakeMotionEvent(dpy, 0, event.xbutton.x,
                             event.xbutton.y, CurrentTime);
        XSync(dpy, 0);
        //XCloseDisplay (dpy);
    }  
    XCloseDisplay (dpy);  
    return 0;  
} 
