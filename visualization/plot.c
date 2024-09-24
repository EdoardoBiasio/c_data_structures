#include "visualization.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct _graph_s {
    function1x1 f;
    int color;
    char name[256];
    int show_legend;
} _graph_t;

typedef struct _point_s {
    f64 x, y;
    int color;
} _point_t;

typedef struct _dataset_s {
    f64* data;
    int n, color;
} _dataset_t;

f64 clampf(f64 x, f64 a, f64 b) {
    if (x < a)
	return a;
    if (x > b)
	return b;
    return x;
}
void plot_2D_draw(Display* d, Window w, GC gc, void* data) {
    XPoint xaxis[2], yaxis[2];
    int width, height;
    int npoints, i;
    XPoint *space;
    list_node_t *t;
    _graph_t* g;
    _point_t* p;
    _dataset_t* ds;
    plot_2D_t* plot = data;
    float xlen;
    char buffer[10];
    width = plot->frame.width;
    height = plot->frame.height;
    xlen = fabs(plot->axes[0].end - plot->axes[0].start);
    npoints =(int)(xlen / plot->axes[0].increment);
    space = calloc(sizeof(XPoint), npoints);

    
    XSetForeground(d, gc, plot->axes[0].color);
    XDrawLine(d, w, gc, 10, height - 20, width - 10, height - 20);
    if (plot->axes[0].flags & AXIS_LABEL)
	XDrawString(d, w, gc, width / 2, height - 10, plot->axes[0].label, strlen(plot->axes[0].label));
    if (plot->axes[0].flags & AXIS_LIMITS) {
	snprintf(buffer, 8,"%.1f", plot->axes[0].start);
	buffer[9] = '\0';
	XDrawString(d, w, gc, 25, height - 10, buffer, strlen(buffer));
	snprintf(buffer, 8,"%.1f", plot->axes[0].end);
	buffer[9] = '\0';
	XDrawString(d, w, gc, width - 30, height - 10, buffer, strlen(buffer));
    }
    
    XSetForeground(d, gc, plot->axes[1].color);
    XDrawLine(d, w, gc, 20, 10, 20, height - 10);
    if (plot->axes[1].flags & AXIS_LABEL)
	XDrawString(d, w, gc, 5, 10, plot->axes[1].label, strlen(plot->axes[1].label));
    if (plot->axes[1].flags & AXIS_LIMITS) {
	snprintf(buffer,8, "%.1f", plot->axes[1].start);
		buffer[9] = '\0';
	XDrawString(d, w, gc, 2, height - 20, buffer, strlen(buffer));
	snprintf(buffer,8, "%.1f", plot->axes[1].end);
		buffer[9] = '\0';
	XDrawString(d, w, gc, 10, 8, buffer, strlen(buffer));
    }
    XFlush(d);
    for (i = 0; i < npoints; i++)
	space[i].x = 20 + (int) (((float) i / npoints) * (width - 30));
    t = plot->functions.head;
    while (t) {
	g = (_graph_t*) list_node_data(t);
	for (i = 0; i < npoints; i++) {	 
	    space[i].y = height - 20 - clampf(g->f((float) i * xlen / npoints), plot->axes[1].start, plot->axes[1].end) * (height - 20) / fabs(plot->axes[1].end - plot->axes[1].start);
	}
	XSetForeground(d, gc, g->color);
	XDrawLines(d, w, gc, space, npoints, CoordModeOrigin);
	XFlush(d);
	t = t->next;
    }

    t = plot->points.head;
    while (t) {
	p = (_point_t*) list_node_data(t);
	XSetForeground(d, gc, p->color);

	XFillArc(d, w, gc, 18 + (int) (p->x * (width - 30) / xlen), height - 22 - p->y * (height - 20) / fabs(plot->axes[1].end - plot->axes[1].start), 5, 5, 0, 360 * 64);
	t = t->next;
    }
    XFlush(d);
    t = plot->datasets.head;
    while (t) {
	ds = (_dataset_t*) list_node_data(t);
	for (i = 0; i < npoints && i < ds->n; i++) {
	    space[i].y = height - 20 - clampf(ds->data[i], plot->axes[1].start, plot->axes[1].end) * (height - 20) / fabs(plot->axes[1].end - plot->axes[1].start);
	}
	if (i == ds->n)
	    for (i = ds->n; i < npoints; i++)
		space[i].y = 0;
	XSetForeground(d, gc, ds->color);
	XDrawLines(d, w, gc, space, ds->n, CoordModeOrigin);
	t = t->next;
    }

    t = plot->functions.head;
    i = 0;
    while (t) {
	g = (_graph_t*) list_node_data(t);
	XSetForeground(d, gc, g->color);
	XFillRectangle(d, w, gc, width - 100, 20 + i - 8, 15, 8);
	XSetForeground(d, gc, 0);
	if (g->show_legend) {
	    XDrawString(d, w, gc, width - 80, 20 + i, g->name, strlen(g->name));
	    i += 8;
	} else
	    break;
	t = t->next;
    }
    free(space);
    
}

int plot_2D_create(plot_2D_t* plot, int width, int height) {
    frame_create("plot", width, height, plot_2D_draw, plot, &plot->frame);
    list_init(&plot->functions, sizeof(_graph_t), NULL);
    list_init(&plot->points, sizeof(_point_t), NULL);
    list_init(&plot->datasets, sizeof(_dataset_t), NULL);
    return 0;
}

int _plot_2D_set_axis(plot_2D_t* plot, f64 min, f64 max, f64 dx, int type, int axis) {
    plot->axes[axis].start = min;
    plot->axes[axis].end = max;
    plot->axes[axis].increment = dx;
    plot->axes[axis].type = type;
    plot->axes[axis].flags = 0;
    plot->axes[axis].color = 0x000000;
    
    return 0;
}
int _plot_2D_set_axis_label(plot_2D_t* plot, const char* name, int axis) {
    strncpy(plot->axes[axis].label, name, 255);
    plot->axes[axis].label[255] = '\0';
    plot->axes[axis].flags |= AXIS_LABEL;
    return 0;
}
int plot_2D_add_function(plot_2D_t* plot, function1x1 f, int color) {
    _graph_t g;
    g.f = f;
    g.color = color;
    list_push_back(&plot->functions, &g);
    return 0;
}

int _plot_2D_show_axis_limits(plot_2D_t* plot, int axis) {
    plot->axes[axis].flags |= AXIS_LIMITS;
    return 0;
}

int plot_2D_add_function_legend(plot_2D_t* plot, ...) {
    va_list vl;
    list_node_t *t;
    _graph_t* g;
    char *name;
    va_start(vl, plot);
    t = plot->functions.head;
    while (t) {
	g = (_graph_t*)list_node_data(t);
	name = va_arg(vl, char*);
	if (name) {
	    strncpy(g->name, name, 255);
	    g->name[255] = '\0';
	    g->show_legend = 1;
	} else
	    break;
	t = t->next;
    }
    va_end(vl);
    return 0;
}

int plot_2D_add_point(plot_2D_t* plot, f64 x, f64 y, int color) {
    _point_t p;
    p.x = x; p.y = y;
    p.color = color;
    list_push_back(&plot->points, &p);
    return 0;
}

int plot_2D_add_dataset(plot_2D_t* plot, f64* data, int n, int color) {
    _dataset_t d;
    d.data = data;
    d.n = n;
    d.color = color;
    list_push_back(&plot->datasets, &d);
    return 0;
}

int plot_2D_display(plot_2D_t* plot) {
    frame_display(&plot->frame);
    return 0;
}

