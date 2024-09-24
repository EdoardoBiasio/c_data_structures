#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include <X11/Xlib.h>
#include "../data_structures/ds.h"
#include "../numerical/numerical.h"

#if defined(__APPLE__) || defined(__MACH__) || defined(__linux__)

typedef struct frame_s {
    char name[256];
    int width, height;
    Display* display;
    Window window, main_window;
    Atom WM_DELETE_WINDOW;
    GC gc;
    void* data;
    void (*draw)(Display*, Window, GC, void*);
} frame_t;

int frame_create(const char* name, int width, int height, void (*draw)(Display*, Window, GC, void*), void* data, struct frame_s* frame);

#elif defined(_WIN32) || defined(_WIN64) || defined(CYGWIN)


#else

typedef struct frame_s {} frame_t;

#endif

int frame_close(struct frame_s* frame);
int frame_display(struct frame_s* frame);

enum {
    AXIS_LINEAR,
    AXIS_LOGARITHMIC
};
#define AXIS_HIDDEN 1 << 0
#define AXIS_LABEL 1 << 1
#define AXIS_LIMITS 1 << 2

typedef struct axis_s {
    int type, flags;
    f64 start, end, increment;
    char label[256];
    int color;
} axis_t;

typedef struct plot_2D_s {
    list_t functions;
    list_t points;
    list_t datasets;
    frame_t frame;
    axis_t axes[2];
} plot_2D_t;

int plot_2D_create(plot_2D_t* plot, int width, int height);
int _plot_2D_set_axis(plot_2D_t* plot, f64 min, f64 max, f64 dx, int type, int axis);
#define plot_2D_set_x_axis(plot, min, max, dx, type) _plot_2D_set_axis(plot, min, max, dx, type, 0)
#define plot_2D_set_y_axis(plot, min, max, dx, type) _plot_2D_set_axis(plot, min, max, dx, type, 1)
int _plot_2D_show_axis_limits(plot_2D_t* plot, int axis);
#define plot_2D_show_x_axis_limits(plot) _plot_2D_show_axis_limits(plot, 0)
#define plot_2D_show_y_axis_limits(plot) _plot_2D_show_axis_limits(plot, 1)
int _plot_2D_set_axis_label(plot_2D_t* plot, const char* name, int axis);
#define plot_2D_set_x_axis_label(plot, name) _plot_2D_set_axis_label(plot, name, 0)
#define plot_2D_set_y_axis_label(plot, name) _plot_2D_set_axis_label(plot, name, 1)
int plot_2D_add_function(plot_2D_t* plot, function1x1 f, int color);
int plot_2D_add_point(plot_2D_t* plot, f64 x, f64 y, int color);
int plot_2D_add_dataset(plot_2D_t* plot, f64* data, int n, int color);
int plot_2D_display(plot_2D_t* plot);
int plot_2D_add_function_legend(plot_2D_t* plot, ...);


#endif
