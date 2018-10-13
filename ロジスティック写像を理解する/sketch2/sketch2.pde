float r = 0;
float dr = 0.002;
int n = 30;
float x0_0 = 0.0000001;
float x0_1 = 0.0000002;
float p;

void setup() {
  size(800, 600);
  frameRate(30);  
  fill(255);
  noStroke();
  background(0);
  p = width / (float)n;
}

void draw() {
  fill(0);
  rect(0, 0, width, height);
  fill(255);
  float x;
  x = x0_0;
  for (int i=0; i<n; i++) {
    float xn1; /* xn+1 */
    xn1 = r * x * (1 - x);
    ellipse(i*p /* x */, xn1*height /* y */, 10 /* width */, 10 /* height */);
    x = xn1;
  }
  x = x0_1;
  fill(0, 255, 0);
  for (int i=0; i<n; i++) {
    float xn1; /* xn+1 */
    xn1 = r * x * (1 - x);
    ellipse(i*p /* x */, xn1*height /* y */, 10 /* width */, 10 /* height */);
    x = xn1;
  }

  if (r<3) {
    r += dr*4;
  } else if (3<r && r<3.5) {
    r += dr/2;
  } else if (3.5<r && r<3.8) {
    r += dr/4;
  } else if (3.8<r && r<3.95) {
    r += dr/8;
  } else {
    r += dr/32;
  }
  if (4 <= r) {
    r = 0;
  }

  fill(255);
  textAlign(CENTER);
  textSize(32);
  text(String.format("r=%.2f", r), width/2, height/2);
  //saveFrame("line-######.png");
}