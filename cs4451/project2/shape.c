/*FileSmoother.c - reads in file, smooths*/
/*Brandon Yarbrough, Team Greenlee*/

#include <stdio.h>
#include <stdlib.h>

#include "shape.h"


Shape createShape(int numVertices, int numTris)
{
	Shape newShape;
	int i;

	newShape = malloc(sizeof(Shape_Typ));

	newShape->numTables = 4;
	newShape->vTables = malloc(sizeof(float*)*128);
	for(i=0;i<128;i++)
		newShape->vTables[i] = malloc(sizeof(float)*3*numVertices);

	newShape->curVTable = 0;
	newShape->vTable = newShape->vTables[0];
	newShape->tTable = malloc(sizeof(int)*3*numTris);
	if(newShape->vTable == NULL || newShape->tTable == NULL)
		printf("ahhh\n!");
	newShape->numVertices = numVertices;
	newShape->numTris = numTris;
	return newShape;
}

void destroyShape(Shape shape)
{
	free(shape->vTable);
	free(shape->tTable);
	free(shape);
}

typedef struct Point
{
	float x;
	float y;
	float z;
} Point;

/*Point midpoint(Point p1, Point p2)
{
	Point toReturn;
	toReturn.x = (p1.x + p2.x)/2;
	toReturn.y = (p1.y + p2.y)/2;
	toReturn.z = (p1.z + p2.z)/2;
	return toReturn;
}*/

Point fourPoints (Point pt1, Point pt2, Point pt3, Point pt4) {
  
  Point midPoint;
  Point midPointprime;
  Point toReturn;

  // find new midpoint between 2 and 3
  midPoint.x = (pt2.x + pt3.x) / 2;
  midPoint.y = (pt2.y + pt3.y) / 2;
  midPoint.z = (pt2.z + pt3.z) / 2;

  // find midpoint of p1 and p4
  midPointprime.x = (pt1.x + pt4.x) / 2;
  midPointprime.y = (pt1.y + pt4.y) / 2;
  midPointprime.z = (pt1.z + pt4.z) / 2;

  // find length of new midpoint and other midpoint
  // take 1/8 of this length
  // add to new midpoint

  toReturn.x = midPoint.x + (midPoint.x - midPointprime.x)/8;
  toReturn.y = midPoint.y + (midPoint.y - midPointprime.y)/8;
  toReturn.z = midPoint.z + (midPoint.z - midPointprime.z)/8;
  
  return toReturn;
}


static void bSpline(Shape shape)
{

	int curMax=4;
	int i;
	float* tmp;
	int laster,last,next,nexter;
	int vertexGroup;

	Point lasterPoint,lastPoint,thisPoint,nextPoint,nexterPoint;

	curMax = shape->numTables;
	//double the lines (create spaces) 0,1,2,3 gets 0,1x,2,3x,4,5x,6,7x
	//x means new midpoint.
	for(i=curMax-1;i>=0;i--)
	{
		tmp = shape->vTables[i];	
		shape->vTables[i] = shape->vTables[i*2];
		shape->vTables[i*2]=tmp;

	//	printf("moved %d to %d\n",i,i*2);
	}

	/*printf("the contents of the vTables are:\n");
	for(i=0;i<8;i++)
	{
		printf("table %d: ",i);
		for(vertexGroup=0;vertexGroup<shape->numVertices;vertexGroup++)
		{
			printf("(%f, %f, %f) ",shape->vTables[i][vertexGroup*3],
					       shape->vTables[i][vertexGroup*3 + 1],
					       shape->vTables[i][vertexGroup*3 + 2]);
		}
		printf("\n");
	}*/
	curMax*=2;
	//for each of the odd ones
	for(i=1;i<curMax;i+=2) //for 1x,3x,5x,7x
	{
		last = i-1; 
		laster = i-3; if(laster < 0) laster = curMax-2;
		next = i+1;   
		nexter = i+3; if (next >= curMax)
			      {
				    nexter = 2;
				    next   = 0;
			      }
			      else if (nexter >= curMax)
			      {
				    nexter = 0;
			      }
	//	printf("For %d, laster is %d, last is %d, next is %d, nexter is %d\n",
		//		i,laster,last,next,nexter);

		for(vertexGroup = 0;vertexGroup<shape->numVertices;vertexGroup++)
		{
			lastPoint.x = shape->vTables[last][vertexGroup*3];
			lastPoint.y = shape->vTables[last][vertexGroup*3 + 1];
			lastPoint.z = shape->vTables[last][vertexGroup*3 + 2];

			nextPoint.x = shape->vTables[next][vertexGroup*3];
			nextPoint.y = shape->vTables[next][vertexGroup*3 + 1];
			nextPoint.z = shape->vTables[next][vertexGroup*3 + 2];

			nexterPoint.x = shape->vTables[nexter][vertexGroup*3];
			nexterPoint.y = shape->vTables[nexter][vertexGroup*3 + 1];
			nexterPoint.z = shape->vTables[nexter][vertexGroup*3 + 2];

			lasterPoint.x = shape->vTables[laster][vertexGroup*3];
			lasterPoint.y = shape->vTables[laster][vertexGroup*3 + 1];
			lasterPoint.z = shape->vTables[laster][vertexGroup*3 + 2];

			thisPoint = fourPoints(lasterPoint,lastPoint,nextPoint,nexterPoint);
	//		printf("Assigning to vTable %d\n",i);
			shape->vTables[i][vertexGroup*3]=thisPoint.x;
			shape->vTables[i][vertexGroup*3 + 1]=thisPoint.y;
			shape->vTables[i][vertexGroup*3 + 2]=thisPoint.z;
		}
	}
	/*printf("the contents of the vTables are:\n");
	for(i=0;i<curMax;i++)
	{
		printf("table %d: ",i);
		for(vertexGroup=0;vertexGroup<shape->numVertices;vertexGroup++)
		{
			printf("(%f, %f, %f) ",shape->vTables[i][vertexGroup*3],
					       shape->vTables[i][vertexGroup*3 + 1],
					       shape->vTables[i][vertexGroup*3 + 2]);
		}
		printf("\n");
	}*/

	shape->numTables=curMax;	
	printf("got curMax up to %d\n",curMax);
	return;
}

Shape readShapeFromFileForDisplay(char* fileName)
{
	Shape shape = readShapeFromFile(fileName);
	bSpline(shape);
	bSpline(shape);
	bSpline(shape);
	bSpline(shape);
	bSpline(shape);

	return shape;
}

Shape readShapeFromFile(char* fileName)
{
	Shape shape;
	FILE* file;
	int numVertices=-1;
	char inputString[5];
	int i,y;
	int numTris;
	int doTimes;

	file = fopen(fileName,"r");

	if(file == NULL)
	{
		fprintf(stderr,"Read error!\n");
		return NULL;
	}

	/*first line is just BSV1*/
	
	if(fscanf(file,"%4[^\n]\n",inputString) == EOF)
	{
		fprintf(stderr,"File Read error!!\n");
		return NULL;
	}

	doTimes = 4;
	if(strcmp(inputString,"BSV3"))
	{
		if(strcmp(inputString,"BSV1"))
		{
			fprintf(stderr,"File is not BSV3 format!\n");
			return NULL;
		}
		doTimes = 1;
	}


	/*second line is number of vertices*/
	if(fscanf(file,"%d\n",&numVertices) == EOF)
	{
		fprintf(stderr,"File Read error!!\n");
		return NULL;
	}

	if(numVertices <=0)
	{
		fprintf(stderr,"You can't have less than 0 vertices!");
		return NULL;
	}

	/*second line is number of vertices*/
	if(fscanf(file,"%d\n",&numTris) == EOF)
	{
		fprintf(stderr,"File Read error!!\n");
		return NULL;
	}

	/*next is numVertices number of lines to fill up the shape with.*
	 * Create the shape now.*/
	shape = createShape(numVertices,numTris);

	for(i=0;i<numVertices;i++)
	{
		for(y=0;y<doTimes;y++)
		{
			if(fscanf(file,"%f %f %f\n",&(shape->vTables[y][i*3]),
					    &(shape->vTables[y][i*3 + 1]),
					    &(shape->vTables[y][i*3 + 2]))
			   == EOF)
			{
				destroyShape(shape);
				fprintf(stderr,"Read error in vTable read at %d\n",i);
				return NULL;
			}
			printf("hi: %d\n",y);
		}
		for(;y<4;y++)
		{
			shape->vTables[y][i*3] =
				shape->vTables[y-1][i*3];
			shape->vTables[y][i*3+1] =
				shape->vTables[y-1][i*3 + 1];
			shape->vTables[y][i*3+2] =
				shape->vTables[y-1][i*3 + 2];

		}
	}

	/*next up is to take in numVertices*2 -4 number of triangles*/
	numVertices = numTris;
	for(i=0;i<numVertices;i++)
	{
		if(fscanf(file,"%d %d %d\n",&(shape->tTable[i*3]),
				    &(shape->tTable[i*3 + 1]),
				    &(shape->tTable[i*3 + 2]))
		   == EOF)
		{
			destroyShape(shape);
			fprintf(stderr,"Read error in tTable read\n");
			return NULL;
		}

	}

	fclose(file);

	return shape;
}

int numTriangles(Shape shape)
{
/*	return (2*shape->numVertices)-4;*/
	return shape->numTris;
}

int writeShapeToFile(Shape shape, char* fileName)
{
	FILE* file;
	int i,y;
	int mult;

	if(shape==NULL)
		return 1;

	if(fileName == NULL)
		return 1;

	file = fopen(fileName,"w");
	if(file == NULL)
	{
		fprintf(stderr,"Unable to open %s for writing.\n",fileName);
		return 2;
	}

	/*First write BSV#*/
	fprintf(file,"BSV3\n");

	/*then write the number of vertices*/
	fprintf(file,"%d\n",shape->numVertices);

	/*then write the number of vertices*/
	fprintf(file,"%d\n",shape->numTris);

	/*then list all of the vertices*/
	for(i=0;i<shape->numVertices;i++)
	{
		for(y=0;y<4;y++)
		{
			mult=shape->numTables/4;
			fprintf(file,"%f %f %f\n",shape->vTables[y*mult][i*3],
					shape->vTables[y*mult][i*3 + 1],
					shape->vTables[y*mult][i*3 + 2]);
		}
	/*		fprintf(file,"----\n");*/
	}

	/*then list all the triangles*/
	for(i=0;i<numTriangles(shape);i++)
	{
		fprintf(file,"%d %d %d\n",shape->tTable[i*3],
					  shape->tTable[i*3+1],
				  	  shape->tTable[i*3+2]);
        }

	fclose(file);
	return 0;
}

/*the main function -- main*/
/*int main(int argc,char ** argv)
{
	Shape inShape; 
	Shape outShape;

	if(argc != 3)
	{
		fprintf(stderr,"\nfileSmoother - Team Greenlee\n"
			       "Usage:\n\tfileSmoother inFile outFile\n\n");
		return 1;
	}

	inShape = readShapeFromFile(argv[1]);
	if(inShape == NULL)
	{
		fprintf(stderr,"Read failure.\n");
		return 1;
	}	
	if(writeShapeToFile(inShape,argv[2]) != 0)
	{
		fprintf(stderr,"Write failure.\n");
		return 2;
	}
	return 0;
}*/
