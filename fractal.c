#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fractal.h"


Figure * start_figure(double width, double height){
    Figure * fig =  (Figure*) malloc(sizeof(Figure));
    fig->width = width;
    fig->height = height;
    printf("Start figure: (%.2fx%.2f)\n",width,height);
    export_svg(fig, "ehue");

}

/* <svg height="100" width="100">
  <circle cx="50" cy="50" r="40" stroke="black" stroke-width="3" fill="red" />
  Sorry, your browser does not support inline SVG.  
</svg>
*/ 

void export_svg(Figure * fig, char * file_name){
    char* tempNum = to_string_from_int((int)fig->height);
    char* tempStr = concat("<svg height=\"", tempNum);
    tempStr = concat(tempStr, "\" width=\"");
    tempNum = to_string_from_int((int)fig->width);
    tempStr = concat(tempStr, tempNum);
    tempStr = concat(tempStr, "\">\n");
    tempStr = concat(tempStr, "<circle cx=\"50\" cy=\"50\" r=\"40\" stroke=\"black\" stroke-width=\"3\" fill=\"red\" />\n");
    tempStr = concat(tempStr, "</svg>\n");
    printf("\n%s\n", tempStr); 

    FILE* fp;

    fp = fopen("test.svg", "w");

    if(fp != NULL)
    {
        fwrite(tempStr, 1, strlen(tempStr), fp);
        fclose(fp);
    }
    free(tempNum);
    free(tempStr);

}

char* to_string_from_int(int num){
    char * result = (char *) malloc(50*(sizeof(char))); 
    sprintf(result, "%d", num); 
    printf("\n The string for the int num is %s", result); 
    return result;
}

char* to_string_from_float(float num){
    char * result = (char *) malloc(50*(sizeof(char))); 
    sprintf(result, "%f", num); 
    printf("\n The string for the float num is %s", result); 
    return result;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

