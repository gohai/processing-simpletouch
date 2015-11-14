// Based on Gravitational Attraction (3D)
// by Daniel Shiffman

import processing.simpletouch.*;
SimpleTouch touchscreen;

Planet[] planets = new Planet[10];
Sun[] suns = new Sun[0];

void setup() {
  fullScreen();

  // println("Available input devices:");
  // printArray(SimpleTouch.list());

  touchscreen = new SimpleTouch(this);
  println("Opened device: " + touchscreen.name());

  // place some planets
  for (int i = 0; i < planets.length; i++) {
    planets[i] = new Planet(random(0.1, 2), random(0, width), random(0, height));
  }
}

void draw() {
  background(0);

  SimpleTouchEvt touches[] = touchscreen.touches();

  // create a new sun for each finger detected
  suns = new Sun[touches.length];
  for (int i=0; i < touches.length; i++) {
    // x and y are values from 0.0 to 1.0
    suns[i] = new Sun(width * touches[i].x, height * touches[i].y);
  }

  // draw suns
  for (Sun sun : suns) {
    sun.display();
  }

  // calculate planets
  for (Planet planet : planets) {
    // planet is attrachted by each sun
    for (Sun sun : suns) {
      PVector force = sun.attract(planet);
      planet.applyForce(force);
    }
    // update and draw planets
    boolean hasAttractors = false;
    if (0 < suns.length) {
      hasAttractors = true;
    }
    planet.update(hasAttractors);
    planet.display();
  }
}