float G = 100.0;
float m1 = 100.0;
float m2 = 100.0;
PVector[] r1; // r1[time]
PVector[] r2;

void setup() {
  size(800, 500);
  init();
  frameRate(30);  
  fill(255, 255, 255, 255);
  noStroke();
  background(0, 0, 0);
}

void draw() {
  fill(0, 20);
  rect(0, 0, width, height);
  fill(255);
  ellipse(r1[2].x, r1[2].y, 10, 10);
  ellipse(r2[2].x, r2[2].y, 10, 10);
  for (int i=0; i<4; i++) // speed up
    updateTime();
}

void init()
{
  r1 = new PVector[3];
  r2 = new PVector[3];
  float left_mgn = 100;
  float right_mgn = 100;
  float centor = height/2;

  r1[2] = new PVector(left_mgn, centor);
  r2[2] = new PVector(width-right_mgn, centor);  
  r1[1] = new PVector(r1[2].x, r1[2].y+1);
  r2[1] = new PVector(r2[2].x, r2[2].y-1);  
  r1[0] = new PVector(r1[1].x, r1[1].y+2);
  r2[0] = new PVector(r2[1].x, r2[1].y-2);
}

void updateTime()
{
  float r = r1[1].dist(r2[1]);
  float rr = pow(r,2);
  PVector e12 = PVector.mult(PVector.sub(r2[1], r1[1]), 1/r);
  PVector e21 = PVector.mult(PVector.sub(r1[1], r2[1]), 1/r);
  r1[2].x = 2*r1[1].x - r1[0].x + G*m2/rr * e12.x;
  r1[2].y = 2*r1[1].y - r1[0].y + G*m2/rr * e12.y;
  r2[2].x = 2*r2[1].x - r2[0].x + G*m1/rr * e21.x;
  r2[2].y = 2*r2[1].y - r2[0].y + G*m1/rr * e21.y;

  r1[0] = r1[1].copy();
  r2[0] = r2[1].copy();
  r1[1] = r1[2].copy();
  r2[1] = r2[2].copy();
}