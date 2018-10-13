float s2 = 0.2;
float[][] u0;
float[][] u1;
float[][] u2;
PImage img;
int step = 1000;

void setup() {
  size(500, 500);
  init();
  //noSmooth();
  frameRate(30);
}

void draw() {
  for (int i=0; i<step; i++)
    for (int j=0; j<step; j++)
      img.pixels[i*step+j] = color(255-u2[i][j], 100, 100);
  img.updatePixels();
  image(img, 0, 0, width, height);
  for (int i=0; i<4; i++) // speed up
    updateTime();
}

void init()
{ 
  u0 = new float[step][step];
  u1 = new float[step][step];
  u2 = new float[step][step];
  u2[step/2][step/2] = 10000;
  img = createImage(step, step, RGB);
  colorMode(HSB, 255*100/68, 100, 100);
}

void updateTime()
{
  for (int i=0; i<step; i++) {
    for (int j=0; j<step; j++) {
      u0[i][j] = u1[i][j];
      u1[i][j] = u2[i][j];      
    }
  }

  for (int i=0; i<step; i++) {
    u2[0][i] = 0;
    u2[step-1][i] = 0;
    u2[i][0] = 0;
    u2[i][step-1] = 0;
  }

  for (int i=1; i<step-1; i++) {
    for (int j=1; j<step-1; j++) {
          u2[i][j] = 2*u1[i][j] - u0[i][j] +
      s2*(u1[i-1][j]-2*u1[i][j]+u1[i+1][j] +
          u1[i][j-1]-2*u1[i][j]+u1[i][j+1]);
    }
  }
}