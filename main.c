#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fractal.h"

// typedef struct
// {
//     int width, height;
//     double thickness, resolution;
// } Figure;

// typedef struct
// {
//     int r, g, b;
// } Color;

// typedef struct {
//     int x, y;
// } Point2D;

// typedef struct {
//     int n, root;
// }Tree;

// Figure *start_figure(double width, double height);
// void set_thickness_resolution(Figure * fig, double thickness, double resolution);
// void set_color(Figure * fig, Color c);
// void draw_fx(Figure * fig, double f(double x), double start_x, double end_x);
// void draw_polyline(Point2D * poly_line, int n);
// void draw_circle(Point2D * center, double r);
// void draw_ellipse(/*...*/);
// void draw_binary_tree(Tree * root);
// void scale_figure(double scale_x, double scale_y);
// void resize_figure(Point2D start_roi, Point2D end_roi);
// void append_figures(Figure * fig1, Figure * fig2);
// void export_eps(Figure * fig, char * file_name);
// void export_svg(Figure * fig, char * file_name);
// void import_svg(char * file_name, Figure * fig);
// void draw_koch_snowflake(Point2D * center, double thickness, int size, int num_iterations);
// void draw_fractal_tree(double center_x, double center_y, int size, int num_iterations);
// void draw_fractal_atree(double center_x, double center_y, int size, int num_iterations);

double gx(double);

int main() 
{
    Figure * figure = start_figure(130.0, 160.0);
    set_thickness_resolution(figure, 3.1, 24);
    Color c = {.r=1, .g=2, .b=3};
    set_color(figure, c);
    draw_fx(figure, gx, 20.5, 80.5);
    export_svg(figure, "canvas1.svg");

    return 0;
}

double gx(double x){
    return 2*x;
}


