#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fractal.h"

Figure *start_figure(double width, double height){
    Figure * fig =  (Figure*) malloc(sizeof(Figure));
    fig->width = width;
    fig->height = height;
    //svg_node *svgHead = NULL;
    //fig->svg_head = svgHead;
    printf("Start figure: (%.2fx%.2f)\n",width,height);
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
    
    printf("g(start_x) = g(%.2f) = %.2f\n", start_x, f(start_x));
    printf("g(end_x) = g(%.2f) = %.2f\n", end_x, f(end_x));

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
    char * tag = (char*)malloc(sizeof(char)*13);
    strcpy(tag, "<svg height=\"");
    appendInteger(tag, (int)height);
    appendString(tag, "\" width=\"");
    appendInteger(tag, (int)width);
    appendString(tag,  "\">\n");
    return tag;
}

void append_svg_line_tag(char *tag, Figure *fig, Point2D *p1, Point2D *p2){
    //"<line x1=\"0\" y1=\"0\" x2=\"200\" y2=\"200\" style=\"stroke:rgb(255,0,0);stroke-width:2\" />\n"
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
    appendString(tag, "\" fill=\"white\" />\n");
}

void append_svg_rect_tag(char * tag, Figure *fig, int width, int height){
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


void append_rgb_part_of_tag(char * tag, Color *c){
    appendString(tag, "rgb(");
    appendInteger(tag, c->r);
    appendString(tag, ",");
    appendInteger(tag, c->g);
    appendString(tag, ",");
    appendInteger(tag, c->b);
    appendString(tag, ")");
}

// void append_axises(char *svg, Figure *fig){
//     Point2D *axis_start, *axis_end;
//     axis_start = (Point2D*)malloc(sizeof(Point2D));
//     axis_end = (Point2D*)malloc(sizeof(Point2D));
//     axis_start->x = (int)(fig->width)/3;
//     axis_start->y = (int) (fig->height);
//     axis_end->x = (int) (fig->width)/3;
//     axis_end->y = 0;
//     append_svg_line_tag(svg, fig, axis_start, axis_end); // x-axis
// }

void export_svg(Figure * fig, char * file_name){
    char * svg = svg_head_tag((int)fig->width, (int)fig->height);
    Point2D *axis_start, *axis_end;
    axis_start = (Point2D*)malloc(sizeof(Point2D));
    //axis_end = (Point2D*)malloc(sizeof(Point2D));
    
    append_svg_rect_tag(svg, fig, fig->width, fig->height);
    //append_tag(&svg, svg);
    appendString(svg, "<line x1=\"0\" y1=\"0\" x2=\"200\" y2=\"200\" style=\"stroke:rgb(255,0,0);stroke-width:2\" />\n");
    
    // axis_start->x = (int)(fig->width)/2;
    // axis_start->y = (int) (fig->height);
    // axis_end->x = (int) (fig->width)/2;
    // axis_end->y = 0;
   //append_svg_line_tag(svg, fig, axis_start, axis_end); // x-axis
    // axis_start->x = 0;
    // axis_start->y = (int) (fig->height)/2;
    // axis_end->x = (int) (fig->width);
    // axis_end->y = (int) (fig->height)/2;
    // append_svg_line_tag(svg, fig, axis_start, axis_end); // x-axis

   // append_axises(svg, fig);

    
    //appendString(svg, "<rect width=\"130\" height=\"160\" stroke=\"black\" stroke-width=\"1\" fill=\"white\" />\n");
    //appendString(svg, "<circle cx=\"50\" cy=\"50\" r=\"40\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" />\n");
    appendString(svg, "</svg>\n");
    FILE* fp;

    fp = fopen(file_name, "wb");

    if(fp != NULL)
    {
        fwrite(svg, 1, strlen(svg), fp);
        fclose(fp);
    }
}

void appendString(char *svg, char *text){
    svg = (char*) realloc(svg, strlen(svg) + strlen(text) + 1);
   // printf("\nsvg  size: %ld -> %s mem: %u", strlen(svg), svg, svg);
   // printf("\ntext size: %ld -> %s mem: %u", strlen(text), text, text);
    strcat(svg, text);
   // printf("\nconc size: %ld -> %s mem: %u\n", strlen(svg), svg, svg);
}

void appendInteger(char *svg, int n){
    char result[16];
    sprintf(result, "%d", n);
   // printf("\nOn appendInteger svg size: %ld -> %s mem: %u", strlen(svg), svg, svg);
   // printf("\nThe string for the int num is %s, length:%ld", result, strlen(result));
    appendString(svg, result);
}

void appendFloat(char *svg, float n){
    char * result = (char *) malloc(16*(sizeof(char)));
    sprintf(result, "%f", n);
   // printf("\n The string for the int num is %s, length:%ld", result, strlen(result));
    appendString(svg, result);
}

//linked list :
// void append_tag(svg_node** head_ref, char * new_tag)
// {
//     svg_node *new_node = (svg_node*) malloc(sizeof(svg_node));
//     svg_node *last = *head_ref;
//     new_node->tag = new_tag;
//     new_node->next = NULL;
//     if (*head_ref == NULL)
//     {
//        *head_ref = new_node;
//        return;
//     }
//     while (last->next != NULL) last = last->next;
//     last->next = new_node;
//     return;
// }

// void print_list(svg_node *node)
// {
//     printf("***************************");
//   while (node != NULL)
//   {
//      printf("%s\n", node->tag);
//      node = node->next;
//    }
//    printf("***************************");
// }