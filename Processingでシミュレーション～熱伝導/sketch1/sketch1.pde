void setup() 
{
  size(680, 120);
  int step = 100;
  PImage img = createImage(step, 1, RGB);
  colorMode(HSB, step*100/68, 100, 100);
  noSmooth();
  for (int i=0; i<step; i++) {
    img.pixels[i] = color(i+1, 100, 100);
  }
  img.updatePixels();
  image(img, 0, 0, width, height);
}