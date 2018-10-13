ParticleSet set; //<>//
float k = 500.00; // spring constant 
float c = -1.0; // damping coefficient
float g = 9.80; // unit:m/s/s, gravitational acceleration
float fps = 60;
float dt = 0.01; // unit:s, time step
float mpp = 0.01; // unit:m/pixel
float remin = 0.05, remax = remin; // unit:m, radius
int numOfParticles = 2000;
float mmin = 0.1, mmax = mmin; // unit:kg, range of mass
PVector r0min, r0max, v0min, v0max; // unit:m, m/s, range of initial value
float hsv_max = 100;
float color_s = hsv_max, color_v = hsv_max;
int speed = 1;
boolean collision_enb = true;

void setup() {
  size(640, 480);
  frameRate(fps);

  // initial value
  r0max = new PVector(mpp*width-remax, mpp*height-remax);
  r0min = new PVector(remax, remax);
  v0max = new PVector( 1.0, 1.0);
  v0min = new PVector(-1.0,-1.0);

  set = new ParticleSet(numOfParticles);
  colorMode(HSB, hsv_max, hsv_max, hsv_max);   
}

void draw() {
  background(100);
  for (int i=0; i<speed; i++)
    set.step();
  set.draw();
  // Saves each frame as line-000001.png, line-000002.png, etc.
  //saveFrame("line-######.png");  
}

class ParticleSet {
  Particle[] set;

  ParticleSet (int num) {
    set = new Particle[num];
    for (int i=0; i<set.length; i++) {
      float re = random(remin, remax);
      float x = random(r0min.x, r0max.x);
      float y = random(r0min.y, r0max.y);
      float vx = random(v0min.x, v0max.x);
      float vy = random(v0min.y, v0max.y);
      float m = random(mmin, mmax);
      set[i] = new Particle(new PVector(x, y), new PVector(vx, vy), m, re);
    }
  }

  void draw() {
    for (int i=0; i<set.length; i++) {
      set[i].draw();
    }
  }

  void step() {
    calc_f();
    for (int i=0; i<set.length; i++) {
      set[i].step();
    }
  }

  void calc_f() {
    for (int i=0; i<set.length; i++) {
      set[i].Fsd = new PVector();
    }
    for (int i=0; i<set.length; i++) {
      for (int j=0; j<set.length && j < i; j++) {
        PVector rij = PVector.sub(set[j].r, set[i].r);
        float dij = rij.mag();
        float reij = set[i].re + set[j].re;
        float d = reij - dij;

        if (0 < d) {
          PVector nij = rij.normalize().copy();
          PVector Fs = PVector.mult(nij, k*d);
          PVector vij = PVector.sub(set[j].v, set[i].v);
          float v = PVector.dot(nij, vij);
          PVector Fd = PVector.mult(nij, v*c);
          PVector Fsd = PVector.add(Fs, Fd);
          set[i].Fsd = PVector.sub(set[i].Fsd, Fsd);
          set[j].Fsd = PVector.add(set[j].Fsd, Fsd);
        }
      }
    }
  }
}

class Particle {
  PVector F, Fsd;
  PVector v;
  PVector r;
  float m;
  float re;
  float color_h;

  Particle(PVector r0, PVector v0, float m, float re) {
    r = r0;
    v = v0;
    this.m = m;
    this.re = re;
    color_h = random(0, hsv_max);
  }

  void draw() {
    float vmag = abs(v.x)+abs(v.y); color_h = map(vmag, 0, 10, 70, 0);
    stroke(color_h, color_s, color_v); /* HSV */
    fill(color_h, color_s, color_v, 50); /* HSVA */
    ellipse(r.x/mpp, r.y/mpp, re*2/mpp, re*2/mpp);
  }

  void step() {
    F = new PVector(0, m*g); // Fout
    calc_f();
    if (collision_enb)
      F = PVector.add(F, Fsd);
    v = PVector.add(v, PVector.mult(F, dt/m));
    r = PVector.add(r, PVector.mult(v, dt));
  }

  void calc_f() {
    calc_f_wall();
  }

  void calc_f_wall() {
    float d, n;
    // x direction
    if (r.x < re) {
      n =  1.0;
      d = re-r.x;
    } else if (mpp*width < r.x+re) {
      n = -1.0;
      d = r.x+re-mpp*width;
    } else {
      n = 0;
      d = 0;
    }
    if (0 < d) {
      F.x += k*d*n;
      F.x += c*v.x;
    }

    // y direction
    if (r.y < re) {
      n =  1.0;
      d = re-r.y;
    } else if (mpp*height < r.y+re) {
      n = -1.0;
      d = r.y+re-mpp*height;
    } else {
      n = 0;
      d = 0;
    }
    if (0 < d) {
      F.y += k*d*n;
      F.y += c*v.y;
    }
  }
}