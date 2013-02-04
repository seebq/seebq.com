#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shape.h"

/* the viewport is 500x500 with the origin at (0,0) */
int Vx_min=0, Vx_max=500, Vy_min=0, Vy_max=500, Vz_min=0, Vz_max=0;
int frame_width=500;
int frame_height=500;
int dontSpin=0;

int curVTable = 0;

/* for some reason we have to flip the image vertically */
void flipVertical(unsigned char *data, int w, int h){
	int x, y, i1, i2;
	unsigned char temp;
	for (x=0;x<w;x++){
		for (y=0;y<h/2;y++){
			i1 = (y*w + x)*3; // this pixel
			i2 = ((h - y - 1)*w + x)*3; // its opposite (across x-axis)
			if (i1 == 226953) {
				int k = 5;
			}

			// swap pixels
			temp = data[i1];
			data[i1] = data[i2];
			data[i2] = temp;

			i1++; i2++;
			temp = data[i1];
			data[i1] = data[i2];
			data[i2] = temp;

			i1++; i2++;
			temp = data[i1];
			data[i1] = data[i2];
			data[i2] = temp;

		}
	}
}

/* write the current frame buffer to the file 'filename' */
void writeFrameBuffer(char *filename){
  FILE *fp = fopen(filename, "wb");
  int data_size = frame_width*frame_height*3;
  unsigned char *framebuffer = malloc(data_size * sizeof(unsigned char));
  glReadPixels(0, 0, frame_width, frame_height, GL_RGB, GL_UNSIGNED_BYTE, framebuffer);
  flipVertical(framebuffer, frame_width, frame_height);
  
  fprintf(fp, "P6\n%d %d\n%d\n", frame_width, frame_height, 255);
  fwrite(framebuffer, data_size, 1, fp);
  fclose(fp);
}


void cross_product(float u[3], const float v[3], const float w[3]) {

  u[0] = v[1]*w[2] - v[2]*w[1];
  u[1] = v[2]*w[0] - v[0]*w[2];
  u[2] = v[0]*w[1] - v[1]*w[0];

}

void init(void) 
{   
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { -10.0, 10.0, -10.0, 1.0 };
   GLfloat light1_position[] = { 0.5, 0.5, 0.5, 0.5 };
   GLfloat light1_color[] = { 0.0, 0.0, 1.0, 1.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT1,	GL_AMBIENT, light1_position);
   glLightfv(GL_LIGHT1,	GL_AMBIENT, light1_color);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_DEPTH_TEST);

   /* do we need this? */
    glEnable(GL_NORMALIZE);

	glLoadIdentity ();             /* clear the matrix */
	gluLookAt (-9.0, 7.0,9.0, 0.0, 0.0, 2.0, 0.0, 1.0, 0.0);
}

Shape shape;
Shape shape2;
float **normals;
int *didIt;

void display(void)
{
	int i;
	float edge1[3];
	float edge2[3];
	float tri_normal[3];

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity ();             /* clear the matrix */
	  /* viewing transformation  */
	/* torus LookAt */
	//gluLookAt (3.0, 8.0,4.0, 0.0, 0.0, 2.0, 0.0, 1.0, 0.0);
	/* lshape LookAt */
	//gluLookAt (-3.0, -3.0,5.0, 0.0, 0.0, 2.0, 0.0, 1.0, 0.0);
//	gluLookAt (-9.0, 7.0,9.0, 0.0, 0.0, 2.0, 0.0, 1.0, 0.0);
	/* cube LookAt */
	//gluLookAt (5.0, 5.0,5.0, 0.0, 0.0, 2.0, 0.0, 1.0, 0.0);
	/* tetrahedron LookAt */
	//gluLookAt (-2.5, -0.75, -0.1, 0.0, 0.0, 2.0, 0.0, 1.0, 0.0);

//glScalef (1.0, 2.0, 1.0);      /* modeling transformation */

	/* testing a 3d shape */
	//glutSolidSphere(1.0,20,16);

	if(shape!=NULL)
	{
	    /* green */
	    glColor3f(0.0,1.0,0.0); /* params are floats in range [0.0,1.0] */
	    for(i=0;i<numTriangles(shape);i++)
	      {
//		glBegin(GL_POLYGON);
//		glBegin(GL_LINE_STRIP);
		glBegin(GL_TRIANGLES);
		/* assign normal */
		glNormal3fv(normals[shape->tTable[i*3]]);
		glVertex3f(shape->vTable[3*shape->tTable[i*3]],
			   shape->vTable[3*shape->tTable[i*3]+1],
			   shape->vTable[3*shape->tTable[i*3]+2]);

		glNormal3fv(normals[shape->tTable[i*3+1]]);
		glVertex3f(shape->vTable[3*shape->tTable[i*3+1]],
			   shape->vTable[3*shape->tTable[i*3+1]+1],
			   shape->vTable[3*shape->tTable[i*3+1]+2]);

		glNormal3fv(normals[shape->tTable[i*3+2]]);
		glVertex3f(shape->vTable[3*shape->tTable[i*3+2]],
			   shape->vTable[3*shape->tTable[i*3+2]+1],
			   shape->vTable[3*shape->tTable[i*3+2]+2]);
		glVertex3f(shape->vTable[3*shape->tTable[i*3]],
			   shape->vTable[3*shape->tTable[i*3]+1],
			   shape->vTable[3*shape->tTable[i*3]+2]);

		glEnd();
		glFlush();
	      }
	}
	if(shape2 != NULL)
	  {
	    glColor3f(1.0,0.0,0.0);
	    for(i=0;i<numTriangles(shape2);i++)
	      {
	///	glBegin(GL_LINE_STRIP);
		glBegin(GL_POLYGON);
		glVertex3f(shape2->vTable[3*shape2->tTable[i*3]],
			   shape2->vTable[3*shape2->tTable[i*3]+1],
			   shape2->vTable[3*shape2->tTable[i*3]+2]);
		glVertex3f(shape2->vTable[3*shape2->tTable[i*3+1]],
			   shape2->vTable[3*shape2->tTable[i*3+1]+1],
			   shape2->vTable[3*shape2->tTable[i*3+1]+2]);
		glVertex3f(shape2->vTable[3*shape2->tTable[i*3+2]],
			   shape2->vTable[3*shape2->tTable[i*3+2]+1],
			   shape2->vTable[3*shape2->tTable[i*3+2]+2]);
/*		glVertex3f(shape2->vTable[3*shape2->tTable[i*3]],
			   shape2->vTable[3*shape2->tTable[i*3]+1],
			   shape2->vTable[3*shape2->tTable[i*3]+2]);*/
		glEnd();
		glFlush();
		sleep(3);
	      }
	  }
	glFlush ();
	glutSwapBuffers();
}

void reshape (int w, int h)
{
   
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   /* 2d views */
   //gluOrtho2D(Vx_min, Vx_max, Vy_min, Vy_max);
   //glOrtho(-500.0, 500.0, -500.0, 500.0, -500.0, 500.0);
   /* 3d views */
   gluPerspective(90, 1, 1, 1000);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity ();
}
void mouse(int button, int state, int x, int y)
{
   static double at = 0;
   static double TWOPI = 6.28318;
   static frame = 0;
   char files[25];
   double X,Y;


   if(state == GLUT_UP) return;
   switch (button) {
      case GLUT_LEFT_BUTTON:
	/*	  if (state == GLUT_DOWN) {
			curVTable++;
			if(curVTable>=128)
				curVTable = 0;
			printf("vTable moved to %d\n",curVTable);

			shape->vTable = shape->vTables[curVTable];
			glutPostRedisplay();
		  }*/
	 glLoadIdentity ();             /* clear the matrix */
	 if(dontSpin == 0)
	 	at+=.05;
	 
	 while(at>TWOPI)
		 at-=TWOPI;

	 X=(double)cos(at)*15.0;
	 Y=(double)sin(at)*15.0;

	 printf("Looking from %2.2e, %2.2e, %2.2e\n",X,Y,5.0);fflush(stdout);
	 gluLookAt (X, 5.0,Y, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);
	 glScalef (1.0, 2.0, 1.0);      /* modeling transformation */
	 glutPostRedisplay();

	 frame++;
	 sprintf(files,"frame%04d.ppm",frame);
         writeFrameBuffer(files);
         break;
      default:
         break;
   }
}

void update(int value)
{
			curVTable++;
			if(curVTable>=shape->numTables)
				curVTable = 0;
			printf("vTable moved to %d\n",curVTable);
			shape->vTable = shape->vTables[curVTable];
	
			mouse(GLUT_LEFT_BUTTON,GLUT_DOWN,0,0);
			glutTimerFunc(100,update,0);
}

int main(int argc, char** argv)
{
   int c,i;
   float v[3], w[3];
   float *vTable;

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (Vx_max-Vx_min, Vy_max-Vy_min); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   
   if(argc > 3)
   {
	printf("Usage: %s fileName [noSpin]\n",argv[0]);
 	return 1;
   } 
   if(argc > 1)
   	shape = readShapeFromFileForDisplay(argv[1]);
   if((argc > 2) && !strcmp("noSpin",argv[2]))
      dontSpin=1;
   else
      dontSpin=0;

   if(shape == NULL)
	   return 1;
   if(shape != NULL)
   {
	printf("We have %d vertices.\n",shape->numVertices); 
	normals = malloc(sizeof(float*)*shape->numVertices);
	for(c=0;c<shape->numVertices;c++)
	{
		normals[c]=malloc(sizeof(float)*3);
		normals[c][0] = normals[c][1] = normals[c][2] = 0;
		for(i=0;i<numTriangles(shape);i++)
		{
			vTable = shape->vTable;
			if(c == shape->tTable[i*3])
			{
			v[0]=vTable[shape->tTable[i*3 +1]*3] - vTable[shape->tTable[i*3]*3];
			v[1]=vTable[shape->tTable[i*3 +1]*3+1]-vTable[shape->tTable[i*3]*3+1];
			v[2]=vTable[shape->tTable[i*3 +1]*3+2]-vTable[shape->tTable[i*3]*3+2];

			w[0]=vTable[shape->tTable[i*3 +2]*3] - vTable[shape->tTable[i*3]*3];
			w[1]=vTable[shape->tTable[i*3 +2]*3+1]-vTable[shape->tTable[i*3]*3+1];
			w[2]=vTable[shape->tTable[i*3 +2]*3+2]-vTable[shape->tTable[i*3]*3+2];

  			normals[c][0] += v[1]*w[2] - v[2]*w[1];
  			normals[c][1] += v[2]*w[0] - v[0]*w[2];
  			normals[c][2] += v[0]*w[1] - v[1]*w[0];
			}
			else if (c == shape->tTable[i*3 +1])
			{
			v[0]=vTable[shape->tTable[i*3 +2]*3] - vTable[shape->tTable[i*3+1]*3];
			v[1]=vTable[shape->tTable[i*3 +2]*3+1]-vTable[shape->tTable[i*3+1]*3+1];
			v[2]=vTable[shape->tTable[i*3 +2]*3+2]-vTable[shape->tTable[i*3+1]*3+2];

			w[0]=vTable[shape->tTable[i*3]*3] - vTable[shape->tTable[i*3+1]*3];
			w[1]=vTable[shape->tTable[i*3]*3+1]-vTable[shape->tTable[i*3+1]*3+1];
			w[2]=vTable[shape->tTable[i*3]*3+2]-vTable[shape->tTable[i*3+1]*3+2];

  			normals[c][0] += v[1]*w[2] - v[2]*w[1];
  			normals[c][1] += v[2]*w[0] - v[0]*w[2];
  			normals[c][2] += v[0]*w[1] - v[1]*w[0];
			}
			else if (c == shape->tTable[i*3 + 2])
			{
			v[0]=vTable[shape->tTable[i*3 ]*3] - vTable[shape->tTable[i*3+2]*3];
			v[1]=vTable[shape->tTable[i*3 ]*3+1]-vTable[shape->tTable[i*3+2]*3+1];
			v[2]=vTable[shape->tTable[i*3 ]*3+2]-vTable[shape->tTable[i*3+2]*3+2];

			w[0]=vTable[shape->tTable[i*3 +1]*3] - vTable[shape->tTable[i*3+2]*3];
			w[1]=vTable[shape->tTable[i*3 +1]*3+1]-vTable[shape->tTable[i*3+2]*3+1];
			w[2]=vTable[shape->tTable[i*3 +1]*3+2]-vTable[shape->tTable[i*3+2]*3+2];

  			normals[c][0] += v[1]*w[2] - v[2]*w[1];
  			normals[c][1] += v[2]*w[0] - v[0]*w[2];
  			normals[c][2] += v[0]*w[1] - v[1]*w[0];
			}
		}
	}
   }

   init ();

   glutTimerFunc(100,update,0);

   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMainLoop();

   return 0;
}
