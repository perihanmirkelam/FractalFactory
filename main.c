#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fractal.h"

double g(double);

int main() 
{
    Figure * figure = start_figure(200.1, 200.1);
    set_thickness_resolution(figure, 1.0, 60.0);
    Color c = {.r=1, .g=2, .b=3};
    set_color(figure, c);
    //draw_fx(figure, g, -50.0, 50.0);
    Point2D center= {.x=20.0, .y=60.0};
    //draw_circle(&center, 40.0, figure);
    draw_ellipse(&center, 120.0, 80.0, figure);
    //export_svg(figure, "canvas3.svg");

    free(figure);
    return 0;
}

double g(double x){
    return x*x*x /10;
}


