#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <utility>
#include<stdlib.h>
#include <vector>
#include <utility>
#include<math.h>
/// FUNCTIA AFINA: [x,y]=A*[x,y]+B

using namespace std;

int vp_width = 640;
int vp_height = 480;

pair<float, float> currentPt;
vector<pair<float, float> > pts;
vector<pair<float, float> > ptscopy;
float A[2][2]={{1,1}, {0,1}};
float B[2][1]={{4},{4}};
float C[2][1];
float D[2][1];
bool closed = false;

int ccw(pair<float, float>p1, pair<float, float>p2, pair<float, float>p3)        /// Three points are a counter-clockwise turn if ccw > 0, clockwise if ccw < 0, and collinear if ccw = 0 because ccw is a determinant that
   {

    return (p2.first - p1.first)*(p3.second - p1.second) - (p2.second - p1.second)*(p3.first - p1.first);

   }


bool convex(int i)
{
    if (i==0) {if(ccw(ptscopy.back(),ptscopy[0],ptscopy[1])>=0) return true;}
    else if(i==ptscopy.size()-1) {if (ccw(ptscopy[i-1],ptscopy[i],ptscopy[0])>=0) return true;}
    else {if(ccw(ptscopy[i-1],ptscopy[i],ptscopy[i+1])>=0) return true;}
    return false;
}


float Angle(pair<float, float>p1, pair<float, float>p2, pair<float, float>p3)    ///unghiul din p2
{   float angle;
    float l1=sqrt((p1.first-p2.first)*(p1.first-p2.first)+(p1.second-p2.second)*(p1.second-p2.second));
    float l2=sqrt((p3.first-p2.first)*(p3.first-p2.first)+(p3.second-p2.second)*(p3.second-p2.second));
    float dot=(p1.first-p2.first)*(p3.first-p2.first) + (p1.second-p2.second)*(p3.second-p2.second);
    if(dot/(l1*l2)<0)   angle=acos(dot/(l1*l2)+0.0001);
    else angle=acos(dot/(l1*l2)-0.0001);
    angle=angle*180/3.142;
    return angle;
}

bool triunghi(int i)
{ int j;
  float angle1,angle2,angle3;

     for(j=0;j<ptscopy.size();j++)
      {

       if (i==0) { angle1=Angle(ptscopy[i],ptscopy[j],ptscopy.back());
           angle2=Angle(ptscopy[i],ptscopy[j],ptscopy[i+1]);
           angle3=Angle(ptscopy[i+1],ptscopy[j],ptscopy.back());

        }
         else if(i==pts.size()-1){ angle1=Angle(ptscopy[i],ptscopy[j],ptscopy[i-1]);
           angle2=Angle(ptscopy[i],ptscopy[j],ptscopy[0]);
           angle3=Angle(ptscopy[0],ptscopy[j],ptscopy[i-1]);

         }
     else {angle1=Angle(ptscopy[i],ptscopy[j],ptscopy[i-1]);
           angle2=Angle(ptscopy[i],ptscopy[j],ptscopy[i+1]);
           angle3=Angle(ptscopy[i+1],ptscopy[j],ptscopy[i-1]);
     }

           if(angle1+angle2+angle3<=360 && angle1+angle2+angle3>300) return false;
      }

return true;
}



int FindEar()                             /// gasim o ureche
{
    int i=0;
    bool EarNotFound = true;
    while(EarNotFound)
    {
      if(convex(i)) if(triunghi(i))  {
      EarNotFound=false;

      }
       if(EarNotFound)  i++;
    }


      return i;


}

void Triangulate()
{ ptscopy=pts;
int ureche;
    while(ptscopy.size()>3)
    {
       ureche= FindEar();


       if( ureche==0) {glBegin(GL_LINE_STRIP);
        glVertex2f( (float)ptscopy.back().first, (float)ptscopy.back().second);
        glVertex2f( (float)ptscopy[ ureche+1].first, (float)ptscopy[ ureche+1].second);
        glEnd();
       }
       else if(ureche==ptscopy.size()-1) {
        glBegin(GL_LINE_STRIP);
        glVertex2f( (float)ptscopy[ureche-1].first, (float)ptscopy[ureche-1].second);
        glVertex2f( (float)ptscopy[ 0].first, (float)ptscopy[ 0].second);
        glEnd();
       }
       else {glBegin(GL_LINE_STRIP);
        glVertex2f( (float)ptscopy[ ureche-1].first, (float)ptscopy[ ureche-1].second);
        glVertex2f( (float)ptscopy[ ureche+1].first, (float)ptscopy[ ureche+1].second);
        glEnd();}
        ptscopy.erase(ptscopy.begin()+ureche);

    }
}


void draw_polygon(int button, int state, int x, int y)
{
    currentPt = pair<float, float>(x, vp_height-y);

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //if ( closed )
           // pts.clear(); // restart if last action was close
        closed = false;
        pts.push_back( currentPt );
    }
    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
        closed = true;

}

void mouse_move(int x, int y)
{
    currentPt = pair<float, float >(x, vp_height-y);
    glutPostRedisplay();
}

void display(void)
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if ( !pts.empty() )
    {
        glBegin(GL_LINE_STRIP);
        for (vector<pair<float, float> > :: iterator pt=pts.begin(); pt!=pts.end(); ++pt )
            glVertex2f( (float)pt->first, (float)pt->second);  ///DESENEAZA SEGMENTUL
      pair <int, int> endPt;
      if( closed)
      {
          endPt=pts.front();

          }
          else endPt = currentPt;
       glVertex2f( (float)endPt.first, (float)endPt.second);
 Triangulate();

        glEnd();
    }

    //glFlush();
    glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y )
{ int i;
    switch(key)
    {
        case 's':
     {
         for(i=0;i<pts.size();i++)
         {
             C[0][0]=pts[i].first;
             C[1][0]=pts[i].second;
             pts[i].first =A[0][0]*C[0][0]+A[0][1]*C[1][0]+B[0][0];
             pts[i].second=A[1][0]*C[0][0]+A[1][1]*C[1][0]+B[1][0];


         }
        // Triangulate();
         break;
    }
}
}

 int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("windows");
    glutDisplayFunc(display);
    glutPassiveMotionFunc (mouse_move);
    glutMouseFunc(draw_polygon);
    glutKeyboardFunc( keyboard );

    glMatrixMode( GL_PROJECTION );
    glOrtho(0.0f, (float)vp_width, 0.0f, (float)vp_height, -1.0, 1.0);



    glutMainLoop();


    return 0;
}


