#include <stdio.h>
#include <stdlib.h>
#include "fractal.h"

double g(double);
void menu(Figure *);

int main() 
{
    Color* color = (Color*)malloc(sizeof(Color));
    double width, height, thickness, resolution;
    int r, g, b;
    printf("\n***Welcome to the Fractal Factory***\n");
    printf("\nEnter width of your figure(decimal):\n");
    scanf("%lf", &width);
    printf("\nEnter height of your figure(decimal):\n");
    scanf("%lf", &width);
    Figure * figure = start_figure(width, height);

    printf("\nEnter thickness(decimal):\n");
    scanf("%lf", &thickness);
    printf("\nEnter resolution(decimal):\n");
    scanf("%lf", &resolution);
    set_thickness_resolution(figure, thickness, resolution);

    printf("\nEnter red, blue and green value of your color(integer integer integer):\n");
    scanf("%d%d%d", &r, &g, &b);
    color->r = r;
    color->g = g;
    color->b = b;
    set_color(figure, *color);

    menu(figure);
    // Figure * figure = start_figure(200.1, 200.1);
    // set_thickness_resolution(figure, 1.0, 60.0);
    // Color c = {.r=1, .g=2, .b=3};
    // set_color(figure, c);
    // //draw_fx(figure, g, -50.0, 50.0);
    // Point2D center= {.x=20.0, .y=60.0};
    // draw_circle(&center, 40.0, figure);
    // //draw_ellipse(&center, 120.0, 80.0, figure);
    // //export_svg(figure, "canvas3.svg");

    return 0;
}

void menu(Figure * figure){
    int choice, x, y;
    double start_x, end_x, r, a, b;
    Point2D *center = (Point2D*)malloc(sizeof(Point2D));
 
    printf("\nSelect your choice:\n");
    printf("1) Draw function f(x) = x^3\n");
    printf("2) Draw circle\n");
    printf("3) Draw ellipse\n");
    printf("4) Exit\n");
    printf("\nChoice:");
    scanf("%d", &choice);

    switch(choice){
        case 1:
            printf("\n\nEnter domain of the function [decimal,decimal]:");
            scanf("%lf%lf", &start_x, &end_x);
            draw_fx(figure, g, start_x, end_x);
            break;
        case 2:
            printf("\n\nEnter center point(x,y) of the circle (integer integer):");
            scanf("%d%d", &x, &y);
            center->x = x;
            center->y = y;
            printf("\n\nEnter radius of the circle (integer):");
            scanf("%lf", &r);
            draw_circle(center, r, figure);
            break;
        case 3:
            printf("\n\nEnter center point of the ellipse (integer x, integer y):");
            scanf("%d%d", &x, &y);
            center->x = x;
            center->y = y;
            printf("\nEnter major and minor axises of the ellipse (decimal, decimal):");
            scanf("%lf%lf", &a, &b);
            draw_ellipse(center, a, b, figure);
            break;
        case 4:
            return;
            break;
        default:
            printf("Wrong input!");
            break;
    }
}

double g(double x){
    return x*x*x /10;
}

double h(double x){
    return x*x;
}

double k(double x){
    return 1/x;
}


