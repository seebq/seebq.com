/* Charles Brian Quinn
 * CS4451 B - Computer Graphics
 * Project 3 - Cylinders, Trees, and Shadows
 * p3.c - Generates two chairs using series of translations, 
 *        rotations, and scaling in scene and two 
 *        significantly different trees using a recursive
 *        procedure.
 * Date:  22 October 2002 
 * Initial code provided at class website.  Additions made. 
 *
 * Texture mapping code and bmp loader.c 
 * provided by Rob Fletcher 2001
 * http://www2.york.ac.uk/services/cserv/sw/graphics/OPENGL/L16.c
*/

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

/* texture name variable */
static GLuint texName;

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

#include "BMPloader.c"  /* include the BMP loader code (see source) */

/* storage object for quadratic */
GLUquadricObj *quadratic;

/* the shadow matrix */
GLfloat m[16];

/* ambient light in direction (10, 10, 10) */
GLfloat light1_x = 10.0;
GLfloat light1_y = 10.0;
GLfloat light1_z = 10.0;

/* eye point coordinates for view */
GLfloat eye_x = 10;
GLfloat eye_y = 0;
GLfloat eye_z = 5;

/* code provided to make checkerboard image 
 * texture_choice is a boolean to use the white and black 
 * or green and gray 
 */
void makeCheckImage(int texture_choice)
{
    int i, j, c;
    
    if (texture_choice == 0) {
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
    else if (texture_choice == 1) {
	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			/* if the row xor column is odd color = white (c=255)
			* otherwise color = black (c=0)
			* for components of type GLubyte, values range [0, 255].
			*/
			c = ((((i&0x1)==0)^((j&0x1)==0)))*255; 
			checkImage[i][j][0] = (GLubyte) c*128;		/* red component */
			checkImage[i][j][1] = (GLubyte) c;		/* green component */
			checkImage[i][j][2] = (GLubyte) c*128;		/* blue component */
			checkImage[i][j][3] = (GLubyte) 128;	/* completely opaque */
		}
	}
    }
}

/* code provided by Rob Fletcher 2001 
 * http://www2.york.ac.uk/services/cserv/sw/graphics/OPENGL/L16.c
 */
GLvoid LoadTexture(GLvoid)
{	
Image *TextureImage;
    
    TextureImage = (Image *) malloc(sizeof(Image));
    if (TextureImage == NULL) {
        printf("Error allocating space for image");
        exit(1);
    }

    if (!ImageLoad("checkerboard.bmp", TextureImage)) {
        exit(1);
    }  
      
    /*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
    /*  x size from image, y size from image,                                              */    
    /*  border 0 (normal), rgb color data, unsigned byte data, and finally the data itself. */ 

    glTexImage2D(GL_TEXTURE_2D, 0, 3,
                 TextureImage->sizeX, TextureImage->sizeY,
                 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); /*  cheap scaling when image bigger than texture */    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); /*  cheap scaling when image smalled than texture*/

    free (TextureImage->data);
    free( TextureImage );
}

/* default init procedure for GLUT */
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
        makeCheckImage(0);
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

/* draws a unit cone */
void draw_cone() 
{
        glPushMatrix(); 
        glTranslatef(0, 0, 1.75); 
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix();
}

/* draws a unit cylinder */
void draw_cylinder() 
{
        glPushMatrix(); 
        //glTranslatef(0, 0, .75); 
	gluCylinder(quadratic,1.0f,1.0f,1.0f,32,32);
        glPopMatrix();
}

/* draws a chair with a circular back and large leges */
void draw_chair1()
{  
	/* draw first chair */

	/* chair legs */
        glPushMatrix(); /* push curr matrix down stack and copy to top of stack */
        glTranslatef(0, 0, 0);      /* translate cylinder front and left. */
	glScalef(0.15, 0.15, 1);          /* scale to be narrow and tall */
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix(); /* replace curr matrix with previously pushed matrix */

        glPushMatrix(); /* push curr matrix down stack and copy to top of stack */
        glTranslatef(0, -0.35, 0);      /* translate cylinder front and left. */
	glScalef(0.15, 0.15, 1);          /* scale to be narrow and tall */
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix(); /* replace curr matrix with previously pushed matrix */

        glPushMatrix(); /* push curr matrix down stack and copy to top of stack */
        glTranslatef(0.35, 0, 0);      /* translate cylinder front and left. */
	glScalef(0.15, 0.15, 1);          /* scale to be narrow and tall */
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix(); /* replace curr matrix with previously pushed matrix */

         glPushMatrix(); /* push curr matrix down stack and copy to top of stack */
        glTranslatef(0.35, -0.35, 0);      /* translate cylinder front and left. */
	glScalef(0.15, 0.15, 1);          /* scale to be narrow and tall */
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix(); /* replace curr matrix with previously pushed matrix */

	/* seat */
	glPushMatrix();
        glTranslatef(0.175, -0.175, 0.95); 
	glScalef(0.35, 0.35, 0.10);
	gluCylinder(quadratic,0.0f,1.0f,1.0f,32,32);
	glPopMatrix();

	glPushMatrix();
        glTranslatef(0.175, -0.175, 1.10); 
	glScalef(0.35, 0.35, 0.15);
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
	glPopMatrix();

	/* back */
	glPushMatrix();
        glTranslatef(0.35, -0.35, 1.75);
	glRotatef(110, 1, 0, 0);         /* rotate 110 degree angle */ 
	glScalef(0.35, 0.55, 0.25);
	gluCylinder(quadratic,0.0f,1.0f,1.0f,32,32);
	glPopMatrix();

}

/* draws a chair with high back and peg type supports and legs */
void draw_chair2()
{
	/* chair legs */
        glPushMatrix(); 
        glTranslatef(2.5, 2.5, 0); 
	glScalef(0.05, 0.05, 1);    
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix(); 

        glPushMatrix(); /* push curr matrix down stack and copy to top of stack */
        glTranslatef(2.85, 2.5, 0);      /* translate cylinder front and left. */
	glScalef(0.05, 0.05, 1);          /* scale to be narrow and tall */
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix(); /* replace curr matrix with previously pushed matrix */

        glPushMatrix(); /* push curr matrix down stack and copy to top of stack */
        glTranslatef(2.85, 2.85, 0);      /* translate cylinder front and left. */
	glScalef(0.05, 0.05, 1);          /* scale to be narrow and tall */
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix(); /* replace curr matrix with previously pushed matrix */

	/* seat */
	glPushMatrix();
        glTranslatef(2.675, 2.675, 0.99); 
	glScalef(0.35, 0.35, 0.05);
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
	glPopMatrix();

	/* back */
        glPushMatrix(); 
        glTranslatef(2.5, 2.85, 1); 
	glScalef(0.05, 0.05, 1.5);    
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(2.85, 2.85, 1); 
	glScalef(0.05, 0.05, 1.5);    
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(2.5, 2.85, 1.75);
	glRotatef(90, 0, 1, 0);
	glScalef(0.05, 0.05, 0.5);    
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix();

        glPushMatrix(); 
        glTranslatef(2.5, 2.85, 1.85);
	glRotatef(90, 0, 1, 0);
	glScalef(0.05, 0.05, 0.5);    
	gluCylinder(quadratic,1.0f,0.0f,1.0f,32,32);
        glPopMatrix();

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
    glRotatef(-29, 1.0, 0.0, 0.0);
    glRotatef(47, 0.0, 0.0, 1.0);
    glScalef(0.59, 0.59, 0.59);
    tree(rec-1, length);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0, 0.0, 4*length/6);
    glRotatef(39, 0.0, 0.0, 1.0);
    glRotatef(28, 1.0, 0.0, 0.0);
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
    glRotatef(55, 0.0, 0.0, 1.0);
    glRotatef(38, 1.0, 0.0, 0.0);
    glRotatef(131, 0.0, 0.0, 1.0);
    glScalef(0.42, 0.42, 0.42);
    tree(rec-1, length);
    glPopMatrix();

  }
  
}

/* draw_scene takes in a n int to determine whether or not to draw objects
 * or shadows.  0 will draw objects, 1 will draw shadows. */
void draw_scene(int shade)
{
        /* variable for recursion level */
        int rec = 0;

        GLfloat mat_red[] = {1.0, 0.0, 0.0, 0.0};
        GLfloat mat_green[] = {0.0, 1.0, 0.0, 0.0};
        GLfloat mat_blue[]  = {0.0, 0.0, 1.0, 0.0};
        GLfloat mat_black[] = {0.0, 0.0, 0.0, 0.0};
        GLfloat mat_brown[]  = {0.43, 0.16, 0.08, 0.0};
        GLfloat mat_dark_green[]  = {0.23, 0.43, 0.21, 0.0};

	if (shade == 0) {
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_blue);
	}
	else if (shade == 1) {
	  glPushMatrix();
	  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_black);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_black);
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_black);
	  glMultMatrixf(m); /* apply shading matrix */
	}

	glPushMatrix();
	glTranslatef(2.0, -2.0, 0);
	glRotatef(20, 0, 0, 1);
	draw_chair1();
	glPopMatrix();
	
	if (shade == 0) {
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_red);
	}
	glPushMatrix();
	glTranslatef(-1.75, 0.25, 0);
	glRotatef(-20, 0, 0, 1);
	draw_chair2();
	glPopMatrix();

	if (shade == 0) {
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_green);
	}
	glPushMatrix();
	glTranslatef(-1.0, -2.0, 0);
	glRotatef(40, 0, 0, 1);
	draw_chair2();
	glPopMatrix();

	if (shade == 0) {
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_green);
	}
	glPushMatrix();
	glTranslatef(-1.75, -2.0, 0);
	glRotatef(-50, 0, 0, 1);
	draw_chair1();
	glPopMatrix();

	if (shade == 0) {
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_brown);
	}
	rec = 7;
	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	tree2(rec, 1.8);
	glPopMatrix();

	if (shade == 0) {
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_dark_green);
	}
	rec = 6;
	glPushMatrix();
	glTranslatef(-3.0, 3.0, 0);
	tree(rec, 1.1);
	glPopMatrix();

	if (shade == 0) {
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_dark_green);
	}
	rec = 7;
	glPushMatrix();
	glTranslatef(-3.0, -3.5, 0);
	tree(rec, 1.5);
	glPopMatrix();

	if (shade == 0) {
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_brown);
	}
	rec = 6;
	glPushMatrix();
	glTranslatef(3.0, -3.0, 0);
	glRotatef(-90, 0, 0, 1);
	tree(rec, 1.3);
	glPopMatrix();

	if (shade == 0) {
	  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_dark_green);
	}
	rec = 6;
	glPushMatrix();
	glTranslatef(3.0, 3.0, 0);
	glRotatef(90, 0, 0, 1);
	tree(rec, 1.5);
	glPopMatrix();

	if (shade == 1) {
	  glPopMatrix();
	}
}

/* simple display function for GLUT */
void display(void)
{
        GLfloat mat_red[] = {1.0, 0.0, 0.0, 0.0};
        GLfloat mat_green[] = {0.0, 1.0, 0.0, 0.0};
        GLfloat mat_blue[]  = {0.0, 0.0, 1.0, 0.0};
        GLfloat mat_black[] = {0.0, 0.0, 0.0, 0.0};

        /* light placed at infinity in the direcion <10,10,10>*/
        GLfloat light_position[] = {light1_x, light1_y, light1_z, 0.0}; 

        /* this will be your shadow matrix.  You need to specify what this contains.
         * OpenGL has a funky ordering for rows and columns
         * use this ordering for rows and columns.  The identity matrix with Mr,c = M3,3 = 0;
         */
        m[0]= 1; m[4]= 0; m[8] = -(light1_x/light1_z); m[12]= 0;
        m[1]= 0; m[5]= 1; m[9] = -(light1_y/light1_z); m[13]= 0;
        m[2]= 0; m[6]= 0; m[10]= 0;                    m[14]= 0;
        m[3]= 0; m[7]= 0; m[11]= 0;                    m[15]= 1;

        /* replaces the matrix in the top of the stack with the identity matrix */
        glLoadIdentity(); 

        /* multiplies the current matrix by a tranformation matrix that puts the eyepoint 
         * at <eye_x,eye_y,eye_z>, center of the image at <0,0,0>, 
	 * and up vector of <0,0,1> (z-axis).
         */
        gluLookAt(eye_x,eye_y,eye_z, 0,0,0, 0,0,1); 

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

	/* to load texture image */
	LoadTexture();

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

	/* draw unit cone of radius of 1 and height 1 */
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_blue);

	/* draw all objects with colors */
	draw_scene(0);
	/* draw all objects shadows */
	draw_scene(1);

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
      case 'w':
	 light1_x++;
         break;
      case 's':
         light1_x--;
         break;
      case 'd':
	 light1_y++;
         break;
      case 'a':
	 light1_y--;
         break;
      case 'j':
	 eye_x--;
         break;
      case 'l':
	 eye_x++;
         break;
      case 'i':
	 eye_y++;
         break;
      case 'k':
	 eye_y--;
         break;
      case 'u':
	 eye_z--;
         break;
      case 'o':
	 eye_z++;
         break;
      default:
         break;
   }
   glutPostRedisplay();
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


