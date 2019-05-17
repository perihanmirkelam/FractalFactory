#ifndef fractal
#define fractal

typedef struct
{
    int width, height;
    double thickness, resolution;
    struct Color_s *color;
    struct svg_node_s *svg_head;
    char* svg;
} Figure;

typedef struct svg_node_s{
    char * tag;
    struct svg_node* next;
} svg_node;

typedef struct Color_s
{
    int r, g, b;
} Color;

typedef struct Point2D_s
{
    int x, y;
} Point2D;

typedef struct{
    int n, root;
} Tree;

Figure *start_figure(double width, double height);
void set_thickness_resolution(Figure * fig, double thickness, double resolution);
void set_color(Figure * fig, Color c);
void draw_fx(Figure * fig, double f(double x), double start_x, double end_x);
void draw_polyline(Point2D * poly_line, int n);
void draw_circle(Point2D * center, double r);
void draw_ellipse(/*...*/);
void draw_binary_tree(Tree * root);
void scale_figure(double scale_x, double scale_y);
void resize_figure(Point2D start_roi, Point2D end_roi);
void append_figures(Figure * fig1, Figure * fig2);
void export_eps(Figure * fig, char * file_name);
void export_svg(Figure * fig, char * file_name);
void import_svg(char * file_name, Figure * fig);
void draw_koch_snowflake(Point2D * center, double thickness, int size, int num_iterations);
void draw_fractal_tree(double center_x, double center_y, int size, int num_iterations);
void draw_fractal_atree(double center_x, double center_y, int size, int num_iterations);

char* concat(const char *, const char *);
char* concatWithInteger(const char *, float);
char* to_string_from_int(int);
char* to_string_from_float(float);


void appendString(char *, char *);
void appendInteger(char *, int);

void append_svg_tag(svg_node **, char *);
void append_svg_rect_tag(char *, Figure *, int, int);
void append_rgb_part_of_tag(char *, Color *);
void append_svg_line_tag(char * tag, Figure *fig, Point2D *p1, Point2D *p2);

#endif
