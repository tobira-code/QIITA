float k = 0.5;
float[] t0;
float[] t1;
float xmin;
float xmax;
PImage img;
int step = 100;

void setup() {
  size(640, 100);
  init();
  noSmooth();
  frameRate(60);
}

void draw() {
  for (int i=0; i<step; i++) {
    img.pixels[i] = color(255-t1[i], 100, 100);
  }
  img.updatePixels();
  image(img, 0, 0, width, height);
  for (int i=0; i<10; i++) // speed up
    updateTime();
}

void init()
{ 
  t0 = new float[step];
  t1 = new float[step];
  xmin = 0;
  xmax = 255;
  img = createImage(step, 1, RGB);
  colorMode(HSB, 255*100/68, 100, 100);
}

void updateTime()
{
  for (int i=0; i<step; i++) {
    t0[i] = t1[i];
  }

  t1[0] = t0[0] + k * (xmin - 2 * t0[0] + t0[1]);
  for (int i=1; i<step-1; i++) {
    t1[i] = t0[i] + k * (t0[i-1] - 2 * t0[i] + t0[i+1]);
  }
  t1[step-1] = t0[step-1] + k * (t0[step-2] - 2 * t0[step-1] + xmax);
}