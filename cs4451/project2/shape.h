/*shape.h - shape handling functions*/
/*Brandon Yarbrough for Team Greenlee*/

#ifndef __SHAPE_H_4242
#define __SHAPE_H_4242

typedef struct Shape  
{
        int numVertices; /*we must know how many vertices*/
        float *vTable;   /*this is an array with numVertices*3 elements.
                          * It is a list of vertices and their 3D locations*/

        int *tTable;     /*this is an array with (2*numVertices - 3)*3
                          * elements.  It is a list of triangles (counter-
                          * clockwise sides out)*/
	int numTris;
	int curVTable;   /* the current index that the vTable is set to*/
	int numTables;   /* the number of vTables*/
	float **vTables; /* an array of vTables*/
} *Shape, Shape_Typ;

Shape createShape(int,int); /*Allocates a shape.
                         * Takes number of vertices in new shape*/

void destroyShape(Shape shape); /*unallocates a shape*/

Shape readShapeFromFile(char* fileName); /*yup*/
Shape readShapeFromFileForDisplay(char *fileName);
int writeShapeToFile(Shape shape, char* fileName); /*ditto*/


int numTriangles(Shape shape); /*tells you how many edges a shape has*/

#endif
