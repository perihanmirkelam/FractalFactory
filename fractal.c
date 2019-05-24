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
    int interval = (int)((end_x - start_x)/fig->resolution); // interval distance
    int count = (int)((end_x - start_x)/interval); // line count
    int x = start_x, temp_end, i=0, j=0, x1, y1, x2, y2; // boundaries
    int svg_x = fig->origin->x, svg_y = fig->origin->y; // shift values for svg coordinate system
    Point2D * points = (Point2D*)malloc(sizeof(Point2D)*count*2);
    if(end_x > start_x && start_x<fig->width/2 && end_x<fig->width/2 && (end_x - start_x) > fig->resolution){
        printf("f: [%d,%d] -> [%d,%d] (INTERVAL: %d - Line Count: %d)\n", 
        (int)start_x, (int)end_x, -(int)fig->height/2, (int)fig->height/2, interval, count);
        for(j=0;j<(count);j++){
            x1 = svg_x + (int)x;
            y1 = svg_y - (int)f(x);
            x += interval;
            x2 = svg_x + (int)x ;
            y2 = svg_y - (int)f(x);
            if( x1<fig->width && x1>0 && y1<fig->height && y1>0
                && x2<fig->width && x2>0 && y2<fig->height && y2>0){
                (*(points + i)).x = x1;
                (*(points + i)).y  = y1;
                (*(points + i + 1)).x = x2;
                (*(points + i + 1)).y = y2;
                printPoint(points + i);
                printPoint(points + i + 1);
                i+=2;
            } else {
                printf("OUT OF BOUNDARIES! (%d,%d), (%d,%d)\n", 
                x1 - svg_x, svg_y - y1, x2 - svg_x, svg_y - y2);
            }
        }
        draw_polyline(points, i-1, fig);
    } else {
        printf("Not print the graph of function. Points are out of boundaries");
        free(points);
    }
}

void draw_polyline(Point2D * poly_line, int n, Figure * fig){
    printf("Lines count: %d\n", n);
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

    for(i = 0; i<n; i+=2){
        append_svg_line_tag(svg, fig, (poly_line + i), (poly_line + i + 1));
    }

    appendString(svg, "</svg>\n");
    FILE* fp;

    fp = fopen("polyline.svg", "w");

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

void draw_circle(Point2D * center, double r, Figure* fig){
    //(x-h)^2 + (y-k)^2 = r^2 -> y =  +- sqrt(r^2 - (x-h)^2)
    int interval = (int)((r*2)/fig->resolution); // interval distance 
    int count = (int)((r*2)/interval); // line count
    int start_x = center->x - r, end_x = center->x + r; // boundaries
    int svg_x = fig->origin->x, svg_y = fig->origin->y; // shift values for svg coordinate system
    int x=start_x, h=center->x, k=center->y, i=0, j=0, px1, py1, py1s, px2, py2, py2s;
    Point2D * points = (Point2D*)malloc(sizeof(Point2D)*count*4);
    printf("Center: ");
    printPoint(center);
    printf("f: [%d,%d] -> [%d,%d] (INTERVAL: %d - Line Count: %d)\n", 
    (int)start_x, (int)end_x, center->y - (int)r, center->y + (int)r, interval, count);
    if(end_x > start_x && start_x<fig->width/2 && end_x<fig->width/2 && (end_x - start_x) > fig->resolution){

        for(j=0;j<count;j++){
            px1 = svg_x + (int) x;
            py1 = svg_y - (int)sqrt((r*r)-(pow(x-h,2))) - k;
            py1s = svg_y + (int)sqrt((r*r)-(pow(x-h,2))) - k;
            // printf("sqrt(%d*%d - %d*%d)= %d\n", (int)r,(int)r,(int)x,(int)x, (int)sqrt((r*r)-(x*x)));
            x +=interval;
            px2 = svg_x + (int)x;
            py2 = svg_y - (int)sqrt((r*r)-(pow(x-h,2))) - k;
            py2s = svg_y + (int)sqrt((r*r)-(pow(x-h,2))) - k;
            if( px1<fig->width && px1>0 && py1<fig->height && py1>0 && py1s<fig->height && py1s>0
                && px2<fig->width && px2>0 && py2<fig->height && py2>0 && py2<fig->height && py2s>0){
                (*(points + i)).x = px1;
                (*(points + i)).y  = py1;
                (*(points + i + 1)).x = px2;
                (*(points + i + 1)).y = py2;
                (*(points + i + 2)).x = px1;
                (*(points + i + 2)).y = py1s;
                (*(points + i + 3)).x = px2;
                (*(points + i + 3)).y = py2s;
                printPoint(points + i);
                printPoint(points + i + 1);
                printPoint(points + i + 2);
                printPoint(points + i + 3);
                i+=4;
            } else {
                printf("OUT OF BOUNDARIES! (%d,%d), (%d,%d)\n",
                px1 - svg_x, svg_y - py1 , px2 - svg_x, svg_y - py2);
            }
        }
        draw_polyline(points, i -1, fig);
    } else {
         free(points);
         printf("Not print the circle. Points are out of boundaries");
    }
}

void draw_ellipse(Point2D * center, double a, double b, Figure *fig){
     //[(x-h)/a]^2 + [(y-k)/b]^2 = 1 -> y =  +-sqrt(1-[(x-h)/a]^2)*b + k
    int interval = (int)((a*2)/fig->resolution); // interval distance
    int count = (int)((a*2)/interval); // line count
    Point2D * points = (Point2D*)malloc(sizeof(Point2D)*count*4);
    int svg_x = fig->origin->x, svg_y = fig->origin->y; // shift values for svg coordinate system
    int h = center->x, k = center->y; // translation for axises
    int start_x = h - a, end_x = h + a, start_y = k + b, end_y = k - b; // boundaries
    int x = start_x, i=0, j=0, px1, py1, py1s, px2, py2, py2s;
    x = start_x;

    printf("f: [%d,%d] -> [%d,%d] (INTERVAL: %d - Line Count: %d)\n", (int)start_x, (int)end_x, start_y, end_y, interval, count);
    if(end_x > start_x && start_x<fig->width && end_x<fig->width && (end_x - start_x) > fig->resolution && b>0 && a>0){
        for(j=0;j<count;j++){
            px1 = svg_x + (int) x;
            py1 = svg_y + (int) (-(sqrt(1-(pow((x-h)/a,2)))*b) - k);
            py1s = svg_y + (int) ((sqrt(1-(pow((x-h)/a,2)))*b) - k);

            x += interval;
            px2 = svg_x + (int) x;
            py2 = svg_y + (int) (-(sqrt(1-(pow((x-h)/a,2)))*b) - k);
            int m = (int)1-(pow((102-h)/a,2));
            printf("x=102 y=%d", m);
            py2s = svg_y + (int) ((sqrt(1-(pow((x-h)/a,2)))*b) - k);
            if( px1<fig->width && px1>0 && py1<fig->height && py1>0 && py1s<fig->height && py1s>0
                && px2<fig->width && px2>0 && py2<fig->height && py2>0 && py2<fig->height && py2s>0){
                (*(points + i)).x = px1;
                (*(points + i)).y  = py1;
                (*(points + i + 1)).x = px2;
                (*(points + i + 1)).y = py2;
                (*(points + i + 2)).x = px1;
                (*(points + i + 2)).y = py1s;
                (*(points + i + 3)).x = px2;
                (*(points + i + 3)).y = py2s;
              //  printPoint(points + i);
              //  printPoint(points + i + 1);
              //  printPoint(points + i + 2);
              //  printPoint(points + i + 3);
                i+=4;
            } else {
                printf("\nOUT OF BOUNDARIES! (%d,%d), (%d,%d), (%d,%d), (%d,%d)\n", 
                px1-svg_x, py1-svg_y , px1-svg_x, py1s-svg_y, px2-svg_x, py2-svg_y, px2-svg_x, py2s-svg_y);
            }
        }
        draw_polyline(points, i-1, fig);
    } else {
         free(points);
         printf("Not print the ellipse. Points are out of boundaries.\n");
    }
}

void printPoint(Point2D * p){
    printf("(%d,%d)\n", p->x, p->y);
}

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
    //printf("Printing...\n");
   // printPoint(p1);
    //printPoint(p2);
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

    fp = fopen(file_name, "w");

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