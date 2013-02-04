/* Charles Brian Quinn 
 * CS4451 B - Computer Graphics
 * Project 3 - Cylinders, Trees, and Shadows
 * tree.c - Generates two significantly different trees in scene
 * Date:  22 October 2002
 * Initial code provided at class website.  Additions made. */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

/*  Create checkerboard texture  */
#define checkImageWidth 16
#define checkImageHeight 16

/* 16x16 array of colors */
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;

/* storage object for quadratic */
GLUquadricObj *quadratic;

void makeCheckImage(void)
{
	int i, j, c;
    
	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			/* if the row xor column is odd color = white (c=255)
			* otherwise color = black (c=0)
			* for components of type GLubyte, values range [0, 255].
			*/
			c = ((((i&0x1)==0)^((j&0x1)==0)))*255; 
			checkImage[i][j][0] = (GLubyte) c;		/* red component */
			checkImage[i][j][1] = (GLubyte) c;		/* green component */
			checkImage[i][j][2] = (GLubyte) c;		/* blue component */
			checkImage[i][j][3] = (GLubyte) 255;	/* completely opaque */
		}
	}
}

void init(void)
{    
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 50.0 };

        glClearColor (0.0, 0.0, 0.0, 0.0);
        glShadeModel(GL_FLAT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        /* texture stuff */

        /* fill in checkimage array*/
        makeCheckImage();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        /* generate '1' texture and return the ID in address starting at &texName*/
        glGenTextures(1, &texName);

        /* set the current texture to texture ID in texName*/
        glBindTexture(GL_TEXTURE_2D, texName); 

        /* wrap the texture around the s- and t-dimension*/
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        /* if texture is too small or too big for the surface,
         * magnify or minify it by using the texel coordinate 
         * nearest the pixel to be colored.
         */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                   GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                   GL_NEAREST);

        /* apply the checkimage array to the current texture*/
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
                                checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
                                checkImage);

        /* lighting stuff */
        glShadeModel (GL_SMOOTH);

        /* specular highlights have color 'mat_specular'*/
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

        /* shininess ('N') = mat_shininess*/
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

        glEnable(GL_LIGHTING);  /* enable lighting */
        glEnable(GL_LIGHT0);    /* enable light0 */

	/* quadratic stuff */
	quadratic=gluNewQuadric();	
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);	

	/* normalize all normal vectors for lighting and shading */
	glEnable(GL_NORMALIZE);
}

/* draw a tree with an integer @rec # of recursions (branching) 
 * and using the unit float @length specified  
 * adapted from lecture slides found at: 
 * http://www.gvu.gatech.edu/~jarek/courses/4451&6491/08_Transforms.ppt */
void tree(int rec, float length)
{
  
  if (rec != 0) {
    /* draw cone of length of L along z-axis */
    gluCylinder(quadratic,0.1f,0.0f,length*1.0f,32,32);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 3*length/5);
    glRotatef(42, 1.0, 0.0, 0.0);
    glRotatef(63, 0.0, 0.0, 1.0);
    glScalef(0.72, 0.72, 0.72);
    tree(rec-1, length);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 4*length/6);
    glRotatef(67, 0.0, 0.0, 1.0);
    glRotatef(32, 1.0, 0.0, 0.0);
    glRotatef(131, 0.0, 0.0, 1.0);
    glScalef(0.91, 0.91, 0.91);
    tree(rec-1, length);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 5*length/7);
    glRotatef(-17, 0.0, 0.0, 1.0);
    glRotatef(26, 1.0, 0.0, 0.0);
    glRotatef(-33, 0.0, 0.0, 1.0);
    glScalef(0.79, 0.79, 0.79);
    tree(rec-1, length);
    glPopMatrix();

  }
  
}

/* draw a tree with an integer @rec # of recursions (branching) 
 * and using the unit float @length specified  
 * this tree2 method is a little more complex */
void tree2(int rec, float length)
{
  
  if (rec != 0) {
    /* draw cone of length of L along z-axis */
    gluCylinder(quadratic,0.05f,0.0f,length*1.0f,32,32);
    glPushMatrix();
    glTranslatef(0.0, 0.0, 3*length/5);
    glRotatef(79, 1.0, 0.0, 0.0);
    glRotatef(47, 0.0, 0.0, 1.0);
    glScalef(0.59, 0.59, 0.59);
    tree(rec-1, length);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 4*length/6);
    glRotatef(59, 0.0, 0.0, 1.0);
    glRotatef(58, 1.0, 0.0, 0.0);
    glRotatef(120, 0.0, 0.0, 1.0);
    glScalef(0.78, 0.78, 0.78);
    tree(rec-1, length);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 5*length/7);
    glRotatef(-15, 0.0, 0.0, 1.0);
    glRotatef(38, 1.0, 0.0, 0.0);
    glRotatef(-31, 0.0, 0.0, 1.0);
    glScalef(0.62, 0.62, 0.62);
    tree(rec-1, length);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 6*length/8);
    glRotatef(75, 0.0, 0.0, 1.0);
    glRotatef(38, 1.0, 0.0, 0.0);
    glRotatef(131, 0.0, 0.0, 1.0);
    glScalef(0.42, 0.42, 0.42);
    tree(rec-1, length);
    glPopMatrix();

  }
  
}

void display(void)
{
        /* number of recurses for tree */
        int rec = 0;

        GLfloat mat_red[] = {1.0, 0.0, 0.0, 0.0};
        GLfloat mat_green[] = {0.0, 1.0, 0.0, 0.0};
        GLfloat mat_blue[]  = {0.0, 0.0, 1.0, 0.0};
        GLfloat mat_black[] = {0.0, 0.0, 0.0, 0.0};

        /* light placed at infinity in the direcion <10,10,10>*/
        GLfloat light_position[] = {10.0, 10.0, 10.0, 0.0}; 

        /* this will be your shadow matrix.  You need to specify what this contains.
         * OpenGL has a funky ordering for rows and columns
         * use this ordering for rows and columns.  The identity matrix with Mr,c = M3,3 = 0;
         */
        GLfloat m[16];
        m[0]= 1; m[4]= 0; m[8] = 0; m[12]= 0;
        m[1]= 0; m[5]= 1; m[9] = 0; m[13]= 0;
        m[2]= 0; m[6]= 0; m[10]= 0; m[14]= 0;
        m[3]= 0; m[7]= 0; m[11]= 0; m[15]= 1;


        /* replaces the matrix in the top of the stack with the identity matrix */
        glLoadIdentity(); 

        /* multiplies the current matrix by a tranformation matrix that puts the eyepoint 
         * at <10,0,5>, center of the image at <0,0,0>, and up vector of <0,0,1> (z-axis).
         */
        gluLookAt(10,0,5, 0,0,0, 0,0,1); 

        /* move light0 to 'light_position' */
        glLightfv(GL_LIGHT0, GL_POSITION, light_position); 

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        /* draw checkered floor */
        /* enable textures */
        glEnable(GL_TEXTURE_2D);
        /* set texture mode */
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        /* set current texture to 'texName' */
        glBindTexture(GL_TEXTURE_2D, texName);
        /* draw polygon floor */
        glBegin(GL_QUADS);
                /* z=-0.01 so that the floor doesn't overlap your shadows
                 * glTexCoord fixes a point in the texture to the vertex that follows it.
                 */
                glTexCoord2f(0.0, 0.0); glVertex3f(-4.0, -4.0, -0.01); 
                glTexCoord2f(0.0, 1.0); glVertex3f(-4.0,  4.0, -0.01);
                glTexCoord2f(1.0, 1.0); glVertex3f( 4.0,  4.0, -0.01);
                glTexCoord2f(1.0, 0.0); glVertex3f( 4.0, -4.0, -0.01);
        glEnd();
        glFlush();
        /* disable textures so that we can draw other polys w/o the texture */
        glDisable(GL_TEXTURE_2D); 

	/* display tree */

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_green);

	rec = 5;
	glPushMatrix();
	glTranslatef(0, -2.5, 0);
	//tree(rec, 1.5);
	glPopMatrix();

	rec = 7;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_red);
	glPushMatrix();
	//glTranslatef(0, 2.5, 0);
	tree2(rec, 1.8);
	glPopMatrix();

        glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow(argv[0]);
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0; 
}


