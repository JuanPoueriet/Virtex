#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xft/Xft.h>
#include <X11/Xatom.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TABS 10
#define TAB_HEIGHT 45
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 700

typedef struct {
    char title[64];
    char content[2048];
} Tab;

Tab tabs[MAX_TABS];
int num_tabs = 0;
int active_tab = 0;

Display *display;
int screen;
Window window;
GC gc;
XftFont *font_tab;
XftFont *font_content;
XftFont *font_header;
XftColor xft_color_text;
XftColor xft_color_subtext;
XftColor xft_color_white;
XftDraw *xft_draw;
Atom wm_delete_window;

unsigned long get_color(const char *hex) {
    XColor col;
    Colormap cmap = DefaultColormap(display, screen);
    XParseColor(display, cmap, hex, &col);
    XAllocColor(display, cmap, &col);
    return col.pixel;
}

void init_tabs() {
    num_tabs = 4;
    strncpy(tabs[0].title, "Dashboard", 63);
    strncpy(tabs[0].content, "Enterprise Resource Overview\n\nWelcome, Administrator. All systems are performing within optimal parameters.\n\nSystem Uptime: 99.99%\nActive Nodes: 1,240\nNetwork Load: 15%\n\nNo critical alerts at this time.\n\nType here to edit:", 2047);

    strncpy(tabs[1].title, "Analytics", 63);
    strncpy(tabs[1].content, "Real-time Data Stream\n\n[Processing...]\n\nKey Performance Indicators:\n- Conversion Rate: +4.2%\n- User Retention: 88%\n- Server Response Time: 45ms\n\nRecent Trends show a significant increase in mobile traffic.\n\nType here to edit:", 2047);

    strncpy(tabs[2].title, "Inventory", 63);
    strncpy(tabs[2].content, "Global Asset Management\n\nWarehouse A: 4,500 units\nWarehouse B: 2,100 units\nWarehouse C: 8,900 units\n\nReorder points reached for: Heavy-duty Gaskets, Thermal Paste.\n\nType here to edit:", 2047);

    strncpy(tabs[3].title, "Settings", 63);
    strncpy(tabs[3].content, "Security & Compliance\n\nLast Audit: 2023-10-15\nCompliance Level: Level 1 PCI-DSS\nEncryption: AES-256 GCM\n\nAuto-update is ENABLED.\n\nType here to edit:", 2047);
}

void draw_tabs() {
    int tab_width = WINDOW_WIDTH / num_tabs;

    XSetForeground(display, gc, get_color("#F3F2F1"));
    XFillRectangle(display, window, gc, 0, 0, WINDOW_WIDTH, TAB_HEIGHT);

    for (int i = 0; i < num_tabs; i++) {
        int x = i * tab_width;

        if (i == active_tab) {
            XSetForeground(display, gc, get_color("#FFFFFF"));
            XFillRectangle(display, window, gc, x, 0, tab_width, TAB_HEIGHT);

            XSetForeground(display, gc, get_color("#0078D4"));
            XFillRectangle(display, window, gc, x, TAB_HEIGHT - 3, tab_width, 3);
        } else {
            XSetForeground(display, gc, get_color("#EDEBE9"));
            XDrawLine(display, window, gc, x + tab_width - 1, 10, x + tab_width - 1, TAB_HEIGHT - 10);
        }

        XGlyphInfo extents;
        XftTextExtents8(display, font_tab, (FcChar8 *)tabs[i].title, strlen(tabs[i].title), &extents);
        int text_x = x + (tab_width - extents.width) / 2;
        int text_y = (TAB_HEIGHT + extents.height) / 2 - 2;

        XftDrawString8(xft_draw, (i == active_tab) ? &xft_color_text : &xft_color_subtext, font_tab, text_x, text_y, (FcChar8 *)tabs[i].title, strlen(tabs[i].title));
    }
}

void draw_content() {
    XSetForeground(display, gc, get_color("#FFFFFF"));
    XFillRectangle(display, window, gc, 0, TAB_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT - TAB_HEIGHT);

    XSetForeground(display, gc, get_color("#EDEBE9"));
    XDrawLine(display, window, gc, 0, TAB_HEIGHT, WINDOW_WIDTH, TAB_HEIGHT);

    char content_copy[2048];
    strncpy(content_copy, tabs[active_tab].content, 2047);
    content_copy[2047] = '\0';

    int y = TAB_HEIGHT + 60;
    char *saveptr;
    char *line = strtok_r(content_copy, "\n", &saveptr);
    int first_line = 1;
    while (line != NULL) {
        if (first_line) {
            XftDrawString8(xft_draw, &xft_color_text, font_header, 60, y, (FcChar8 *)line, strlen(line));
            y += 40;
            first_line = 0;
        } else {
            XftDrawString8(xft_draw, &xft_color_text, font_content, 60, y, (FcChar8 *)line, strlen(line));
            y += 30;
        }
        line = strtok_r(NULL, "\n", &saveptr);
    }

    XSetForeground(display, gc, get_color("#0078D4"));
    XFillRectangle(display, window, gc, 0, WINDOW_HEIGHT - 30, WINDOW_WIDTH, 30);
    XftDrawString8(xft_draw, &xft_color_white, font_tab, 20, WINDOW_HEIGHT - 10, (FcChar8 *)"Status: Connected to Enterprise Backend | User: Admin | Press 'q' or 'X' to Quit", 78);
}

void redraw() {
    XClearWindow(display, window);
    draw_tabs();
    draw_content();
}

int main() {
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    screen = DefaultScreen(display);

    XSetWindowAttributes attrs;
    attrs.background_pixel = WhitePixel(display, screen);
    window = XCreateWindow(display, RootWindow(display, screen), 10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, 1,
                           CopyFromParent, InputOutput, CopyFromParent, CWBackPixel, &attrs);

    XStoreName(display, window, "Enterprise Tab Manager");

    // Handle WM_DELETE_WINDOW
    wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    XSelectInput(display, window, ExposureMask | ButtonPressMask | KeyPressMask);
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, NULL);

    font_tab = XftFontOpenName(display, screen, "Sans-11:bold");
    font_content = XftFontOpenName(display, screen, "Sans-12");
    font_header = XftFontOpenName(display, screen, "Sans-18:bold");

    if (!font_tab || !font_content || !font_header) {
        fprintf(stderr, "Cannot open fonts\n");
        exit(1);
    }

    Colormap cmap = DefaultColormap(display, screen);
    Visual *visual = DefaultVisual(display, screen);
    xft_draw = XftDrawCreate(display, window, visual, cmap);

    XRenderColor render_color;
    render_color.red = 0x2020; render_color.green = 0x1F1F; render_color.blue = 0x1E1E; render_color.alpha = 0xffff;
    XftColorAllocValue(display, visual, cmap, &render_color, &xft_color_text);

    render_color.red = 0x6060; render_color.green = 0x5E5E; render_color.blue = 0x5D5D; render_color.alpha = 0xffff;
    XftColorAllocValue(display, visual, cmap, &render_color, &xft_color_subtext);

    render_color.red = 0xffff; render_color.green = 0xffff; render_color.blue = 0xffff; render_color.alpha = 0xffff;
    XftColorAllocValue(display, visual, cmap, &render_color, &xft_color_white);

    init_tabs();

    XEvent event;
    int running = 1;
    while (running) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            redraw();
        }
        if (event.type == ClientMessage) {
            if ((Atom)event.xclient.data.l[0] == wm_delete_window) {
                running = 0;
            }
        }
        if (event.type == ButtonPress) {
            int x = event.xbutton.x;
            int y = event.xbutton.y;
            if (y <= TAB_HEIGHT) {
                int tab_width = WINDOW_WIDTH / num_tabs;
                int clicked_tab = x / tab_width;
                if (clicked_tab < num_tabs) {
                    active_tab = clicked_tab;
                    redraw();
                }
            }
        }
        if (event.type == KeyPress) {
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            if (keysym == XK_q) {
                running = 0;
            } else if (keysym == XK_BackSpace) {
                int len = strlen(tabs[active_tab].content);
                if (len > 0) {
                    tabs[active_tab].content[len - 1] = '\0';
                    redraw();
                }
            } else {
                char buffer[32];
                int n = XLookupString(&event.xkey, buffer, sizeof(buffer), NULL, NULL);
                if (n > 0) {
                    buffer[n] = '\0';
                    if (strlen(tabs[active_tab].content) + n < sizeof(tabs[active_tab].content)) {
                        strcat(tabs[active_tab].content, buffer);
                        redraw();
                    }
                }
            }
        }
    }

    XftDrawDestroy(xft_draw);
    XftFontClose(display, font_tab);
    XftFontClose(display, font_content);
    XftFontClose(display, font_header);
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
