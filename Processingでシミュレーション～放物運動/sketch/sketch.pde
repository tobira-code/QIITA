float g = 1;
float vx0 = 10;
float vy0 = -30;
float rx0 = 0;
float rx1 = vx0 + rx0;
float ry0 = 480;
float ry1 = vy0 + ry0;
float rx2 = 2*rx1 - rx0;
float ry2 = 2*ry1 - ry0 + g;

void setup() {
  size(640,480);
  frameRate(30);
}

void draw() {
  ellipse(rx2, ry2, 10, 10);
  updateTime();
}

void updateTime()
{
  rx0 = rx1;
  ry0 = ry1;
  rx1 = rx2;
  ry1 = ry2;
  rx2 = 2*rx1 - rx0;
  ry2 = 2*ry1 - ry0 + g;
}