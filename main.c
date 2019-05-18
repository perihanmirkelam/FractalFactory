#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fractal.h"

double g(double);

int main() 
{
    Figure * figure = start_figure(1000.1, 1000.1);
    set_thickness_resolution(figure, 1.1, 10);
    Color c = {.r=1, .g=2, .b=3};
    set_color(figure, c);
    draw_fx(figure, g, 0.0, 10.0);
    //export_svg(figure, "canvas3.svg");

    free(figure);
    return 0;
}

double g(double x){
    return x*x;
}


