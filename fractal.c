#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fractal.h"



Figure *start_figure(double width, double height){
    Figure * fig =  (Figure*) malloc(sizeof(Figure));
    Point2D *start = (Point2D*)malloc(sizeof(Point2D));
    Point2D *end = (Point2D*)malloc(sizeof(Point2D));
    Point2D *origin = (Point2D*)malloc(sizeof(Point2D));
    char * line = "xyz";
    fig->width = width * 2;
    fig->height = height * 2;
    fig->start = start;
    fig->end = end;
    origin->x = (int)width;
    origin->y = (int)height;
    fig->origin = origin;
    //fig->lines = line;
    //svg_node *svgHead = NULL;
    //fig->svg_head = svgHead;
    printf("Start figure: (%dx%d)\n",fig->width,fig->height);
    return fig;
}

void set_thickness_resolution(Figure * fig, double thickness, double resolution){
    printf("Set thickenss:%.2f resolution:%.2f\n", thickness, resolution);
    fig->thickness = thickness;
    fig->resolution = resolution;
}

void set_color(Figure * fig, Color c){
    Color *color = (Color*)malloc(sizeof(Color));
    color->r = c.r;
    color->g = c.g;
    color->b = c.b;
    fig->color = color;
    printf("Set color:(%d,%d,%d)\n", fig->color->r, fig->color->g, fig->color->b);
}

void draw_fx(Figure * fig, double f(double x), double start_x, double end_x){
    //Point2D *start = (Point2D*)malloc(sizeof(Point2D));
    //Point2D *end = (Point2D*)malloc(sizeof(Point2D));
    int interval = (int)((end_x - start_x)/fig->resolution);
    int temp_start = start_x, temp_end, i, x1, y1, x2, y2, count=0;
    Point2D * points = (Point2D*)malloc(sizeof(Point2D)*fig->resolution*2);
    if(end_x > start_x){
        printf("INTERVAL: %d\n", interval);
        // printf("g(start_x) = g(%.2f) = %.2f\n", start_x, f(start_x));
        // printf("g(end_x) = g(%.2f) = %.2f\n", end_x, f(end_x));
        // fig->start->x = (int)start_x + fig->origin->x;
        // fig->start->y = fig->origin->y - (int)f(start_x);
        // fig->end->x = (int)end_x + fig->origin->x;
        // fig->end->y = fig->origin->y - (int)f(end_x);
        // printf("g(x) = g(%d) = %d\n", fig->start->x, fig->start->y);
        // printf("g(x) = g(%d) = %d\n", fig->end->x, fig->end->y);
        for(i = 0; i<fig->resolution; i++){
            temp_end = temp_start + interval;
            x1 = (int)temp_start + fig->origin->x;
            y1 = fig->origin->y - (int)f(temp_start);
            x2 = (int)temp_end + fig->origin->x;
            y2 = fig->origin->y - (int)f(temp_end);
            if( x1<=fig->width && x1>=0 && y1<=fig->height && y1>=0
                && x2<=fig->width && x2>=0 && y2<=fig->height && y2>=0){
                count++;           
                (*(points + i)).x = x1;
                (*(points + i)).y  = y1;
                (*(points + i + 1)).x = x2;
                (*(points + i + 1)).y = y2;
                printPoint(points + i);
                printPoint(points + i + 1);
            } else {
                printf("OUT OF BOUNDARIES! (%d,%d), (%d,%d)", x1, y1, x2, y2);
            }
            temp_start += 2;
        }
        draw_polyline(points, count, fig);
    }
}

void draw_polyline(Point2D * poly_line, int n, Figure * fig){
    printf("\nlines count: %d\n", n);
    int i;
    Point2D *axis_start = (Point2D*)malloc(sizeof(Point2D));
    Point2D *axis_end = (Point2D*)malloc(sizeof(Point2D));
    char * svg = svg_head_tag((int)fig->width, (int)fig->height);
    append_svg_rect_tag(svg, fig, fig->width, fig->height);
    axis_start->x = (int)(fig->width/2);
    axis_start->y = (int) (fig->height);
    axis_end->x = (int) (fig->width/2);
    axis_end->y = 0;
    append_svg_line_tag(svg, fig, axis_start, axis_end); // x-axis
    axis_start->x = 0;
    axis_start->y = (int) (fig->height)/2;
    axis_end->x = (int) (fig->width);
    axis_end->y = (int) (fig->height)/2;
    append_svg_line_tag(svg, fig, axis_start, axis_end); // y-axis

    for(i = 0; i<n; i++){
        append_svg_line_tag(svg, fig, (poly_line + i), (poly_line + i + 1));
    }

    appendString(svg, "</svg>\n");
    FILE* fp;

    fp = fopen("xyz.svg", "wb");

    if(fp != NULL)
    {
        fwrite(svg, 1, strlen(svg), fp);
        fclose(fp);
    }
    free(poly_line);
    free(axis_start);
    free(axis_end);
    free(svg);
}

void printPoint(Point2D * p){
    printf("(%d,%d)\n", p->x, p->y);
}

/*
Draws the given function in the figure initialized by “ start_figure ”. It will draw the function
within the range defined by “ start_x” and “end_x ”. You should draw the function as a set
of connected lines. Any such line should be no smaller in length than the resolution defined in
“resolution”. The lines drawing the graph should have the thickness in the given same named
argument. line>res
Make sure that the figure fits in the intended position in the canvas. If the portion of the graph
is outside the canvas, it should be removed properly. See the explanation for
draw_resize_figure.
*/

char * svg_head_tag(int width, int height){
    char * tag = (char*)malloc(sizeof(char)*strlen(SVG_SVG));
    strcpy(tag, SVG_SVG);
    appendInteger(tag, (int)height);
    appendString(tag, SVG_WIDTH);
    appendInteger(tag, (int)width);
    appendString(tag,  SVG_TAG_END);
    return tag;
}

void append_svg_line_tag(char *tag, Figure *fig, Point2D *p1, Point2D *p2){
    //"<line x1=\"0\" y1=\"0\" x2=\"200\" y2=\"200\" style=\"stroke:rgb(255,0,0);stroke-width:2\" />\n"
    //printf("%s", tag);
    appendString(tag, "<line x1=\"");
    appendInteger(tag, p1->x);
    appendString(tag, "\" y1=\"");
    appendInteger(tag, p1->y);
    appendString(tag, "\" x2=\"");
    appendInteger(tag, p2->x);
    appendString(tag, "\" y2=\"");
    appendInteger(tag, p2->y);
    appendString(tag, "\" stroke=\"");
    append_rgb_part_of_tag(tag, fig->color);
    appendString(tag, "\" stroke-width=\"");
    appendInteger(tag, fig->thickness);
    appendString(tag, "\" fill=\"black\" />\n");
}

void append_svg_rect_tag(char *tag, Figure *fig, int width, int height){
    appendString(tag, "<rect width=\"");
    appendInteger(tag, (int)width);
    appendString(tag, "\" height=\"");
    appendInteger(tag, (int)height);
    appendString(tag, "\" stroke=\"");
    append_rgb_part_of_tag(tag, fig->color);
    appendString(tag, "\" stroke-width=\"");
    appendInteger(tag, fig->thickness);
    appendString(tag, "\" fill=\"white\" />\n");
}


void append_rgb_part_of_tag(char *tag, Color *c){
    appendString(tag, "rgb(");
    appendInteger(tag, (int)c->r);
    appendString(tag, ",");
    appendInteger(tag, (int)c->g);
    appendString(tag, ",");
    appendInteger(tag, (int)c->b);
    appendString(tag, ")");
}

void append_axises(char *svg, Figure *fig){
    Point2D *axis_start, *axis_end;
    axis_start = (Point2D*)malloc(sizeof(Point2D));
    axis_end = (Point2D*)malloc(sizeof(Point2D));
    axis_start->x = (int)(fig->width)/3;
    axis_start->y = (int) (fig->height);
    axis_end->x = (int) (fig->width)/3;
    axis_end->y = 0;
    append_svg_line_tag(svg, fig, axis_start, axis_end); // x-axis
}

void export_svg(Figure * fig, char * file_name){
    Point2D *axis_start = (Point2D*)malloc(sizeof(Point2D));
    Point2D *axis_end = (Point2D*)malloc(sizeof(Point2D));
    char * svg = svg_head_tag((int)fig->width, (int)fig->height);
    //fig->svg = svg;

    append_svg_rect_tag(svg, fig, fig->width, fig->height);
    
    //appendString(svg, "<line x1=\"0\" y1=\"0\" x2=\"200\" y2=\"200\" style=\"stroke:rgb(255,0,0);stroke-width:2\" />\n");    
    axis_start->x = (int)(fig->width/2);
    axis_start->y = (int) (fig->height);
    axis_end->x = (int) (fig->width/2);
    axis_end->y = 0;
    append_svg_line_tag(svg, fig, axis_start, axis_end); // x-axis
    axis_start->x = 0;
    axis_start->y = (int) (fig->height)/2;
    axis_end->x = (int) (fig->width);
    axis_end->y = (int) (fig->height)/2;
    append_svg_line_tag(svg, fig, axis_start, axis_end); // y-axis
    append_svg_line_tag(svg, fig, fig->start, fig->end);

    //append_axises(svg, fig);
    //append_tag(&svg, svg);
    //appendString(svg, "<rect width=\"130\" height=\"160\" stroke=\"black\" stroke-width=\"1\" fill=\"white\" />\n");
    //appendString(svg, "<circle cx=\"50\" cy=\"50\" r=\"40\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" />\n");
    //appendString(svg, fig->lines);
    appendString(svg, "</svg>\n");
    FILE* fp;

    fp = fopen(file_name, "wb");

    if(fp != NULL)
    {
        fwrite(svg, 1, strlen(svg), fp);
        fclose(fp);
    }
    free(fig->start);
    free(fig->end);
    free(axis_start);
    free(axis_end);
    free(svg);
}

void appendString(char *svg, char *text){
    svg = (char*) realloc(svg, strlen(svg) + strlen(text));
    // printf("\nsvg  size: %ld -> %s mem: %u", strlen(svg), svg, svg);
    // printf("\ntext size: %ld -> %s mem: %u", strlen(text), text, text);
    strcat(svg, text);
   // printf("\nconc size: %ld mem: %u\n -> %s\n", strlen(svg), svg, svg);
}

void appendInteger(char *svg, int n){
    int digit = (int)((ceil(log10(n+1))));
    char text[digit];
    sprintf(text, "%d", n);
    // printf(" \n NUMBER %d, NUMBER OF DIGIT: %d\n", n, digit);
    // printf("\nOn appendInteger svg size: %ld -> %s mem: %u", strlen(svg), svg, svg);
    // printf("\nThe string for the int num is %s, length:%ld appending string...", text, strlen(text));
    appendString(svg, text);
}

void appendFloat(char *svg, float n){
    char * result = (char *) malloc(16*(sizeof(char))); // remove
    sprintf(result, "%f", n);
   // printf("\n The string for the float num is %s, length:%ld", result, strlen(result));
    appendString(svg, result);
}