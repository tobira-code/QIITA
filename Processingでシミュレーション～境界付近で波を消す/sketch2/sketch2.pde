float a = 0.5;
float b = 0.5;
float[][][] u;
float[][][] vx;
float[][][] vy;
float[][][] px; // psi
float[][][] py;
float[][][] pxy;
float[] sx; // sigma
float[] sy;
PImage img;
int step = 101;
int time = 0;

void setup() {
  size(500, 500);
  init();
  //noSmooth();
  frameRate(30);
}

void draw() {
  for (int i=0; i<step; i++)
    for (int j=0; j<step; j++)
      img.pixels[j*step+i] = color(127-u[i][j][1], 100, 100);
  img.updatePixels();
  image(img, 0, 0, width, height);
  for (int i=0; i<1; i++) // speed up
    updateTime();
}

void init()
{
  u   = new float[step][step][2];
  vx  = new float[step][step][2];
  vy  = new float[step][step][2];
  px  = new float[step][step][2];
  py  = new float[step][step][2];
  pxy = new float[step][step][2];
  sx  = new float[step];
  sy  = new float[step];
  int llen = 10; // length of layer
  float gain = 0.5;
  float inc_step = gain/llen;

  for (int i=step-llen; i<step; i++) {
      sx[i] = sy[i] = inc_step*(i-(step-llen));
  }
  for (int i=0; i<llen; i++) {
      sx[i] = sy[i] = gain - inc_step*i;
  }

  img = createImage(step, step, RGB);
  colorMode(HSB, 255*100/68, 100, 100);
}

void updateTime()
{
  u[step/2][step/2][0] = 3000*sin(2.0*PI/25.0*(float)time);

  /* du/dt */
  for (int i=1; i<step; i++) {
    for (int j=1; j<step; j++) {
      u[i][j][1] = u[i][j][0] + b*(vx[i][j][0]-vx[i-1][j][0]+vy[i][j][0]-vy[i][j-1][0])
        -(sx[i]+sy[j])*u[i][j][0]+px[i][j][0]+py[i][j][0]-pxy[i][j][0];
    }
  }
  /* dvx/dt */
  for (int i=0; i<step-1; i++) {
    for (int j=0; j<step; j++) {
      vx[i][j][1] = vx[i][j][0] + a*(u[i+1][j][1]-u[i][j][1])-sx[i]*vx[i][j][0];
    }
  }
  /* dvy/dt */
  for (int i=0; i<step; i++) {
    for (int j=0; j<step-1; j++) {
      vy[i][j][1] = vy[i][j][0] + a*(u[i][j+1][1]-u[i][j][1])-sy[j]*vy[i][j][0];
    }
  }
  /* dpx/dt */
  for (int i=0; i<step; i++) {
    for (int j=1; j<step; j++) {
      px[i][j][1] = px[i][j][0] + b*sx[i]*(vy[i][j][0]-vy[i][j-1][0]);
    }
  }
  /* dpy/dt */
  for (int i=1; i<step; i++) {
    for (int j=0; j<step; j++) {
     py[i][j][1] = py[i][j][0] + b*sy[j]*(vx[i][j][0]-vx[i-1][j][0]);
    }
  }
  /* dpxy/dt */
  for (int i=0; i<step; i++) {
    for (int j=0; j<step; j++) {
      pxy[i][j][1] = pxy[i][j][0] + sx[i]*sy[j]*u[i][j][0];
    }
  }

  for (int i=0; i<step; i++) {
    for (int j=0; j<step; j++) {
         u[i][j][0] =   u[i][j][1];
        vx[i][j][0] =  vx[i][j][1];
        vy[i][j][0] =  vy[i][j][1];
        px[i][j][0] =  px[i][j][1];
        py[i][j][0] =  py[i][j][1];
       pxy[i][j][0] = pxy[i][j][1];
    }
  }

  time++;
}