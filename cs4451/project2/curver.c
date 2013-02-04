/* curver.c - takes in a bh1 file and subdivides it one level.*/

#include <stdio.h>
#include <stdlib.h>
#include "shape.h"

int V(Shape s, int i)
{
	return s->tTable[i];
}


int L(Shape s, int i,int* oTable)
{
	return V(s,oTable[P(i)]);
}

int R(Shape s, int i,int *oTable)
{
	return V(s,oTable[N(i)]);
}

int N(int i)
{
	i++;
	if(i % 3 == 0)
		i-= 3;
	return i;
}

int P(int i)
{
	return N(N(i));
}

int main(int argc, char** argv)
{
   Shape inShape = NULL;
   Shape outShape= NULL;
   int i,c;
   int doVTable=0;

   int *oTable, *mTable;
   float *vTable;
   int counter;

   float tester;

   if(argc != 3)
   {
	fprintf(stderr,"Usage:\n\t%s inFile outFile.\n",argv[0]);
 	return 1;
   }

   inShape = readShapeFromFile(argv[1]);

   if(inShape == NULL)
	return 1;
 
/*   printf("Let's see what's in the inShape!\n");*/

#ifdef DEBUG
   printf("There are %d vertices.\n",inShape->numVertices);
#endif

   /*printf("The vTable contains:\n");*/
   for(c=0;c<inShape->numVertices*3;c+=3)
   {
/*	   printf("%d => %f %f %f\n",c,inShape->vTable[c],
			   inShape->vTable[c+1],
			   inShape->vTable[c+2]);*/
   }
   //printf("The tTable contains %d triangles\n\n",numTriangles(inShape));
 //  for(c=0;c<numTriangles(inShape)*3;c+=3)
  // {
//	   printf("%d => %d %d %d\n", c,inShape->tTable[c],
//			   inShape->tTable[c+1],
//			   inShape->tTable[c+2]);
  // }

   oTable = malloc(sizeof(int)*numTriangles(inShape)*3);
   /*oTable = malloc(sizeof(int)*99999);*/
  /* for(c=0;c<inShape->numVertices*3;c++)*/
   for(c=0;c<numTriangles(inShape)*3;c++)
   {
	   oTable[c] = -1;
   }


/*   for(c=0;c<numTriangles(inShape)*3;c++)
	   for(i=c+1;i<inShape->numVertices*3;i++)*/
   // printf("There are %d iterations of c and i\n",numTriangles(inShape)*3);
     for(c=0;c<numTriangles(inShape)*3;c++)
	     for(i=c+1;i<numTriangles(inShape)*3;i++)
	{
//		if(c<10)
//		printf("Trying to match %d and %d..V(N(c))=%d, V(P(i))=%d, V(P(c)=%d,v(N(i))=%d\n",c,i,V(inShape,N(c)),V(inShape,P(i)),V(inShape,P(c)),V(inShape,N(i)));
		   if((V(inShape,N(c)) == V(inShape,P(i)))
 		    &&(V(inShape,P(c)) == V(inShape,N(i))))
		   {
		//	   printf("Match found!\n");
			   oTable[c] = i;
			   oTable[i] = c;
		   }
	}

   printf("This is the oTable:\n");
   for(c=0;c<numTriangles(inShape)*3;c++)
   {
	   printf("%d => %d\n",c,oTable[c]);
   }
   
    for(c=0;c<numTriangles(inShape)*3;c++)
      if(oTable[c] == -1)
      {
	printf("MISALIGNMENT in corner %d!!!\n",c);
	printf("Cannot find a corner (opposite than at %d) with N(c)=%d and P(c)=%d\n",
		V(inShape,c),V(inShape,P(c)),V(inShape,N(c)));
        return 1;
      }
   /*mTable = malloc(sizeof(int)*inShape->numVertices*3);*/
   mTable = malloc(sizeof(int)*numTriangles(inShape)*3);
  /* for(c=0;c<inShape->numVertices*3;c++)*/
   for(c=0;c<numTriangles(inShape)*3;c++)
	   mTable[c] = -1;

   /*outShape = createShape(4*inShape->numVertices-6);*/
   outShape = createShape(4*inShape->numVertices,4*numTriangles(inShape));
   printf("outShape has %d vertices\n",outShape->numVertices);
   printf("outShape has %d Triangles\n",numTriangles(outShape));
   if(outShape == NULL)
	   return 5;



for(doVTable = 0;doVTable < inShape->numTables; doVTable++)
{
   outShape->vTable = outShape->vTables[doVTable];
   inShape->vTable = inShape->vTables[doVTable];

   for(c=0;c<inShape->numVertices*3;c++)
  /* for(c=0;c<numTriangles(inShape)*3;c++)*/
   {
	   outShape->vTable[c] = inShape->vTable[c];
   }
}

   counter = inShape->numVertices - 1;

   printf("Counter starts at: %d\n",counter);

   /*for(c=0;c<inShape->numVertices*3;c++)*/
   for(c=0;c<numTriangles(inShape)*3;c++)
   {
	if(mTable[c] == -1)
	{

		counter++;


		for(doVTable=0;doVTable<inShape->numTables;doVTable++)
		{
  		outShape->vTable = outShape->vTables[doVTable];
  		inShape->vTable = inShape->vTables[doVTable];


   		vTable = inShape->vTable;

		outShape->vTable[counter*3]=0;
		outShape->vTable[counter*3]=
			(vTable[3*V(inShape,N(c))]+
			vTable[3*V(inShape,P(c))])/2;
		outShape->vTable[counter*3]+=
			(
			 ( vTable[3*V(inShape,c)] +
			   vTable[3*V(inShape,oTable[c])]
			 )/2
			 - 
			 (
			  vTable[3*L(inShape,c,oTable)] + vTable[3*R(inShape,c,oTable)]
			 +vTable[3*L(inShape,oTable[c],oTable)] +
			  vTable[3*R(inShape,oTable[c],oTable)]
		         )/4	
			 
			 )/4;

		outShape->vTable[counter*3+1]=
			(vTable[3*(V(inShape,N(c)))+1]+
			 vTable[3*(V(inShape,P(c)))+1])/2
			+
			(
			 ( vTable[3*(V(inShape,c))+1] +
			   vTable[3*(V(inShape,oTable[c]))+1]
			 )/2
			 - 
			 (
			  vTable[3*(L(inShape,c,oTable))+1] + vTable[3*(R(inShape,c,oTable))+1]
			 +vTable[3*(L(inShape,oTable[c],oTable))+1] +
			  vTable[3*(R(inShape,oTable[c],oTable))+1]
		         )/4	
			 
			 )/4;
		outShape->vTable[counter*3+2]=
			(vTable[3*(V(inShape,N(c)))+2]+
			 vTable[3*(V(inShape,P(c)))+2])/2
			+
			(
			 ( vTable[3*(V(inShape,c))+2] +
			   vTable[3*(V(inShape,oTable[c]))+2]
			 )/2
			 - 
			 (
			  vTable[3*(L(inShape,c,oTable))+2] + vTable[3*(R(inShape,c,oTable))+2]
			 +vTable[3*(L(inShape,oTable[c],oTable))+2] +
			  vTable[3*(R(inShape,oTable[c],oTable))+2]
		         )/4	
			 
			 )/4;
		}

/*		outShape->vTable[counter*3] =
			(vTable[3*V(inShape,N(c))] + vTable[3*V(inShape,P(c))])/2;
		outShape->vTable[counter*3+1] =
			(vTable[3*(V(inShape,N(c)))+1] + vTable[3*(V(inShape,P(c)))+1])/2;
		outShape->vTable[counter*3+2] =
			(vTable[3*(V(inShape,N(c)))+2] + vTable[3*(V(inShape,P(c)))+2])/2;
*/
	printf("counter is %d\n",counter);
	printf("----\n");
		mTable[c] = counter;
		mTable[oTable[c]] = counter;
	}
   }

   outShape->numVertices = counter+1;
   printf("outShape has %d vertices\n",outShape->numVertices);
/*
   printf("mTable:\n");
   for(c=0;c<inShape->numVertices*3;c++)
	   printf("\t%d => %d\n",c,mTable[c]);*/

   /*printf("new vTable:\n");*/
/*   for(c=0;c<outShape->numVertices;c++)
   	  printf("\t%d => %f %f %f\n",c,outShape->vTable[c*3],
			  		outShape->vTable[c*3+1],
					outShape->vTable[c*3+2]);*/
  /* printf("Counter finishes at: %d\n",counter);*/
   
   printf("there are %d triangles\n",numTriangles(inShape));
   for(c=0;c<numTriangles(inShape)*3;c+=3)
   {
	printf("Triangle %d %d %d becomes: \n",V(inShape,c),V(inShape,c+1),
					       V(inShape,c+2));
	outShape->tTable[c*4] = V(inShape,c);
	outShape->tTable[c*4+1]=mTable[c+2];
	outShape->tTable[c*4+2]=mTable[c+1];
/*	printf("\tTriangle %d %d %d\n",
		outShape->tTable[c*4],
		outShape->tTable[c*4+1],
		outShape->tTable[c*4+2]);*/

	outShape->tTable[c*4+3]=mTable[c+1];
	outShape->tTable[c*4+4]=mTable[c];
	outShape->tTable[c*4+5]=V(inShape,c+2);
/*	printf("\tTriangle %d %d %d\n",
		outShape->tTable[c*4+3],
		outShape->tTable[c*4+4],
		outShape->tTable[c*4+5]);*/

	outShape->tTable[c*4+6]=mTable[c+2];
	outShape->tTable[c*4+7]=V(inShape,c+1);
	outShape->tTable[c*4+8]=mTable[c];
/*	printf("\tTriangle %d %d %d\n",
		outShape->tTable[c*4+6],
		outShape->tTable[c*4+7],
		outShape->tTable[c*4+8]);*/

	outShape->tTable[c*4+9]=mTable[c];
	outShape->tTable[c*4+10]=mTable[c+1];
	outShape->tTable[c*4+11]=mTable[c+2];
/*	printf("\tTriangle %d %d %d\n",
		outShape->tTable[c*4+9],
		outShape->tTable[c*4+10],
		outShape->tTable[c*4+11]);*/
  }
   
  printf("new tTable:\n");
  for(c=0;c<numTriangles(outShape);c++)
  {
	  printf("%d => %d %d %d\n",c,outShape->tTable[c*3],
			  	      outShape->tTable[c*3+1],
				      outShape->tTable[c*3+2]);
  }

  writeShapeToFile(outShape,argv[2]);

  printf("Hey there!!!!!\n\n\n");fflush(stdout);
  destroyShape(inShape);
  destroyShape(outShape);
  printf("Hey there!!!!!\n\n\n");fflush(stdout);
   return 0;
} 
