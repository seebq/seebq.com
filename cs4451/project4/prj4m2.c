#include "defs.h"

/* prototypes */
void hitCueBall();
void predictCollision(float A[], float B[], float C[]);
void drawBoard();
void drawPoolBall(float x, float y, float mat_color[]);

/* the shadow matrix */
GLfloat M[16];

/* Variables allowing for the rotation of the scene using the mouse */
GLfloat startX, startY, endX, endY;
int motionX=0;
int motionY=0;
int xsize=0, ysize=0, aspect;
int Ctrl=0;

/* ambient light in direction (10, 10, 10) */
GLfloat light1_x = 10.0;
GLfloat light1_y = 10.0;
GLfloat light1_z = 10.0;

/* eye point coordinates for view */
GLfloat eye_x = 0.0001;
GLfloat eye_y = 0;
GLfloat eye_z = 20;
GLfloat set_eye_x = 0.0001;
GLfloat set_eye_y = 0;
GLfloat set_eye_z = 20;

GLUquadricObj *quadObj;

GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };

/* color variables */
GLfloat mat_yellow[] = {1.0, 1.0, 0.0, 0.0};
GLfloat mat_red[] = {1.0, 0.0, 0.0, 0.0};
GLfloat mat_green[] = {0.0, 1.0, 0.0, 0.0};
GLfloat mat_blue[]  = {0.0, 0.0, 1.0, 0.0};
GLfloat mat_white[]  = {1.0, 1.0, 1.0, 0.0};
GLfloat mat_black[]  = {0.0, 0.0, 0.0, 0.0};

float pullback = 0;

/* Radius dimensionsof the balls and the board */
float ball_r = 0.6;
float board_r = 6.55;

/* These arrays represent balls :
 *  [0],[1] => x,y
 *  [2] => angle at which it is moving
 *  [3] => whether the ball is in motion or not.
 */
float yellow_pos[] = { 1.0f, 0.0f,  0.0f, 0.0f};
float red_pos[] =    { 0.0f, 1.0f,  0.0f, 0.0f};
float blue_pos[] =   {-1.0f,-1.0f,  0.0f, 0.0f};
enum{X, Y, ANGLE, MOTION};

/* For cue ball hit detection */
enum {WALL, YELLOW, BLUE, RED, NONE=-1};
int hit_ball = 0;
int time = 0;
float angle;
float mv_increment = 0.1;

/* for displaying text */
int font=(int)GLUT_BITMAP_8_BY_13;
int bitmapHeight=13;
char s[30];

int window_main;
int window_3dview;
int window_score;

/*
 * OpenGL commands
 */
void init(void)
{    
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  /* lighting stuff */
  glShadeModel (GL_SMOOTH);


  glEnable(GL_LIGHTING);  /* enable lighting */
  glEnable(GL_LIGHT0);    /* enable light0 */
  glEnable(GL_NORMALIZE);
}

/* Necessary for the movement of the scene */
void MyIdleFunc(void) { glutPostRedisplay();}
void RunIdleFunc(void) {   glutIdleFunc(MyIdleFunc); }
void PauseIdleFunc(void) {   glutIdleFunc(NULL); }

void display(void)
{
  GLfloat light_position[] = {0.0,0.0,0.0,0.0};

  light_position[0] = light1_x;
  light_position[1] = light1_y;
  light_position[2] = light1_z;

  /* this will be your shadow matrix.  You need to specify what this contains.
   * OpenGL has a funky ordering for rows and columns
   * use this ordering for rows and columns.  
   * The identity matrix with Mr,c = M3,3 = 0;
   */
  M[0]= 1; M[4]= 0; M[8] =(-1*(light1_x)/light1_z); M[12]= 0;
  M[1]= 0; M[5]= 1; M[9] =(-1*(light1_y)/light1_z); M[13]= 0;
  M[2]= 0; M[6]= 0; M[10]= 0;                       M[14]= 0;
  M[3]= 0; M[7]= 0; M[11]= 0;                       M[15]= 1;


  /* specular highlights have color 'mat_specular'*/
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

  /* shininess ('N') = mat_shininess*/
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  /* replaces the matrix in the top of the stack with the identity matrix */
  glLoadIdentity(); 

  /* multiplies the current matrix by a tranformation matrix that puts the 
   * eyepoint 
   * at <10,0,5>, center of the image at <0,0,0>, and up vector of <0,0,1> 
   * (z-axis).
   */
  if (glutGetWindow() == window_main) {
    gluLookAt(set_eye_x,set_eye_y,set_eye_z, 0,0,0, 0,0,1); 
  }
  else {
    gluLookAt(eye_x,eye_y,eye_z, 0,0,0, 0,0,1); 
  }
  /* move light0 to 'light_position' */
  glLightfv(GL_LIGHT0, GL_POSITION, light_position); 

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* initialize the quadric object */
  quadObj = gluNewQuadric();

  /* Draws the TRING Board */
  drawBoard();

  /* Displays the cue arrow */
  glPushMatrix();
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_white);
   printf("line = <%f, %f> -> <%f, %f>\n", startX, startY, endX, endY);
   glTranslatef(0,0,0.1);
   glBegin(GL_POLYGON);
    glVertex3f(startX-0.1, startY+0.1, 1);
    glVertex3f(endX-0.1, endY+0.1, 1);
    glVertex3f(endX+0.1, endY-0.1, 1);
    glVertex3f(startX+0.1, startY-0.1, 1);
   glEnd();
  glPopMatrix();

  /* specular highlights have color 'mat_specular'*/
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

  /* shininess ('N') = mat_shininess*/
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

  if(time==0)
    {
      /* Detects which ball the cue is hitting */
      red_pos[MOTION] = 0;
      yellow_pos[MOTION] = 0;
      blue_pos[MOTION] = 0;

      hitCueBall();
    }
  else{    
    if(red_pos[MOTION])
      {
	predictCollision(red_pos, yellow_pos, blue_pos);

	red_pos[X] += -mv_increment*cos(red_pos[ANGLE]);
	red_pos[Y] += -mv_increment*sin(red_pos[ANGLE]);

	printf("red = <%f, %f>  angle = %f\n", red_pos[X], red_pos[Y], 
	       red_pos[ANGLE]);
      }
    if(yellow_pos[MOTION])
      {
	predictCollision(yellow_pos, red_pos, blue_pos);

	yellow_pos[X] += -mv_increment*cos(yellow_pos[ANGLE]);
	yellow_pos[Y] += -mv_increment*sin(yellow_pos[ANGLE]);

	printf("yellow = <%f, %f> angle = %f\n", yellow_pos[X], yellow_pos[Y], 
	       yellow_pos[ANGLE]);
      }
    if(blue_pos[MOTION])
      {
	predictCollision(blue_pos, red_pos, yellow_pos);

	blue_pos[X] += -mv_increment*cos(blue_pos[ANGLE]);
	blue_pos[Y] += -mv_increment*sin(blue_pos[ANGLE]);

	printf("blue = <%f, %f>  angle = %f\n", blue_pos[X], blue_pos[Y], 
	       blue_pos[ANGLE]);
      }
    else
      ;
    time--;
    glutPostRedisplay();
    glutSetWindow(window_3dview);
    glutPostRedisplay();
    glutSetWindow(window_main);
  }

  /* Display the balls */
  drawPoolBall(red_pos[X],red_pos[Y], mat_red);
  drawPoolBall(yellow_pos[X],yellow_pos[Y], mat_yellow);
  drawPoolBall(blue_pos[X],blue_pos[Y], mat_blue);
 
  glutSwapBuffers();    
  glFlush();
}

void renderBitmapString(float x, float y, void *font,char *string)
{  
  char *c;
  glRasterPos2f(x, y);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void draw_text(GLint x, GLint y, char* s, GLfloat r, GLfloat g, GLfloat b)
{
  int lines;
  char* p;
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 
	  0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glColor3f(r,g,b);
  glRasterPos2i(x, y);
  for(p = s, lines = 0; *p; p++) {
    if (*p == '\n') {
      lines++;
      glRasterPos2i(x, y-(lines*18));
    }
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
  }
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void display_3d() 
{
  gluLookAt(eye_x+5,eye_y+5,eye_z, 0,0,0, 0,0,1); 
  display();

}

void display_score() 
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  draw_text(5, 70, "Turn: Player 1", 1.0, 0.0, 0.0);
  draw_text(5, 50, "Ball: Red", 1.0, 0.0, 0.0);
  draw_text(5, 30, "Player 1: 0", 1.0, 0.0, 0.0);
  draw_text(5, 10, "Player 2: 0", 1.0, 0.0, 0.0);
  

/*   glColor3f(0.0,1.0,1.0); */
/*   glMatrixMode(GL_PROJECTION); */
/*   glLoadIdentity(); */
/*   gluOrtho2D(0, 200, 0, 50); */
/*   glScalef(1, -1, 1); */
/*   glTranslatef(0, -50, 0); */

/*   renderBitmapString(5,15,(void *)font,"Ali never listens.");  */
/*   renderBitmapString(5,35,(void *)font,"Esc - Quit");		 */

  glutSwapBuffers();
}

void reshape(int w, int h)
{
  /* Necessary to move scene with the mouse */
  xsize = w;
  ysize = h;
  aspect = (float)xsize/(float)ysize;

  glViewport(0, 0, xsize, ysize);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 500.0); 

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();

  /* assume a square window of size WINDOW_SIZE */
  gluLookAt (WINX/2.0,WINX/2.0,3.0*(WINX)/4.0,  WINX/2.0,WINX/2.0,0.0,
	     0.0,1.0,0.0);

}

void keyboard (unsigned char key, int x, int y)
{
  /* Keyboard keys that move the view and light position */
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


/* Mouse handler function */
void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      /* Scaling the screen map to the board */
      startX = (GLfloat) (WINY - y - 400) * -0.033333; 
      startY = (GLfloat) (x - 400) * 0.033333; 
      printf("mouse down = <%f, %f>\n", startX, startY);
    }
    else {
      /* Scaling the screen map to the board */
      endX = (GLfloat) (WINY - y - 400) * -0.033333;
      endY = (GLfloat) (x - 400) * 0.033333; 
      printf("mouse up = <%f, %f>\n", endX, endY);

      pullback = sqrt((startX-endX)*(startX-endX) + 
		      (startY-endY)*(startY-endY));

      printf("pullback = %f\n", pullback);
      glutPostRedisplay();
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN) {
      
    }
  default:
    break;
  }
}


void motion(int x, int y)
{
  endX = (GLfloat) (WINY - y - 400) * -0.033333;
  endY = (GLfloat) (x - 400) * 0.033333; 
  glutPostRedisplay();
} 

/*********************************************************/
/********************* PHYSICS ***************************/
/*********************************************************/

/* Detects which ball has been hit by the cue stick */
void hitCueBall()
{
  if((startX<(yellow_pos[X]+ball_r))&&(startX>(yellow_pos[X]-ball_r))
     && (startY<(yellow_pos[Y]+ball_r))&&(startY>(yellow_pos[Y]-ball_r)))
    {
      time = (int)pullback * 20;
      yellow_pos[ANGLE] = atan2(endY-yellow_pos[Y], endX-yellow_pos[X]);
      yellow_pos[MOTION] = 1.0f;
    }
  else if((startX<(blue_pos[X]+ball_r))&&(startX>(blue_pos[X]-ball_r))
	  && (startY<(blue_pos[Y]+ball_r))&&(startY>(blue_pos[Y]-ball_r)))
    {
      time = (int)pullback * 20;
      blue_pos[ANGLE] = atan2(endY-blue_pos[X], endX-blue_pos[Y]);
      blue_pos[MOTION] = 1.0f;
    }
  else if((startX<(red_pos[X]+ball_r))&&(startX>(red_pos[X]-ball_r))
	  && (startY<(red_pos[Y]+ball_r))&&(startY>(red_pos[Y]-ball_r)))
    {
      time = (int)pullback * 20;
      red_pos[ANGLE] = atan2(endY-red_pos[Y], endX-red_pos[X]);
      red_pos[MOTION] = 1.0f;
    }
  else
    ;
}

/* Function to take care of collisions ball A may encounter */
void predictCollision(float A[], float B[], float C[])
{
  float board_circ, magnitude;
  float temp[2];
  /*  float Ax, Ay, Bx, By, Cx, Cy;*/

  /* For wall collision detection */
  board_circ = sqrt( 2*board_r*board_r);
  
  temp[X] = A[X] + -mv_increment*cos(A[ANGLE]);
  temp[Y] = A[Y] + -mv_increment*sin(A[ANGLE]);

  magnitude = sqrt(A[X]*A[X] + A[Y]*A[Y]);

  if(magnitude > board_circ)
    {
      A[ANGLE] += 180/3.14159;
    }
  /*
  Ax = A[0] + time*-mv_increment*cos(A[2]);
  Ay = A[1] + time*-mv_increment*sin(A[2]);

  Bx = B[0] + time*-mv_increment*cos(B[2]);
  By = B[1] + time*-mv_increment*sin(B[2]);

  Cx = C[0] + time*-mv_increment*cos(C[2]);
  Cy = C[1] + time*-mv_increment*sin(C[2]);
  */
}


/*********************************************************/
/********************* DRAWING ***************************/
/*********************************************************/

/* Draws the green circular carum table with border and bumpers */
void drawBoard()
{
  GLfloat mat_border[] = {0.6, 0.6, 0.6, 0.0};

  glPushMatrix();  
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_black);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_black);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_border);

    glTranslatef(0,0,0.3);
    gluDisk(quadObj, 10, 10.5, 70, 70);

    glTranslatef(0,0,-0.4);
    gluCylinder( quadObj, 10, 10, 0.4, 50, 50);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_green);
    gluDisk(quadObj, 0, 10, 50, 50);

  glPopMatrix();
}

/* Generic ball drawing function that also displays the balls' shadows */
void drawPoolBall(float x, float y, float mat_color[])
{
  glPushMatrix();  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_color);
   glTranslatef(x, y, 0.5);
   gluSphere(quadObj, ball_r, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glMultMatrixf(M);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_black);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_black);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_black);

   glTranslatef( x, y, 0.5);
   gluSphere(quadObj, ball_r, 20, 20);
   
  glPopMatrix();

  /* specular highlights have color 'mat_specular'*/
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);

  /* shininess ('N') = mat_shininess*/
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
}


/*
 * main() - receives a command line argument and initializes openGL stuff
 */
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WINX, WINY);
  glutInitWindowPosition(50, 50);
  window_main = glutCreateWindow(TITLE);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  glutInitWindowSize(300, 300);
  glutInitWindowPosition(WINX + 100, 50);
  window_3dview = glutCreateWindow(TITLE_3D);
  init();
  glutDisplayFunc(display_3d);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);

  glutInitWindowSize(200, 100);
  glutInitWindowPosition(WINX + 100, 400);
  window_score = glutCreateWindow(TITLE_SCORE);
  init();
  glutDisplayFunc(display_score);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0; 
}
