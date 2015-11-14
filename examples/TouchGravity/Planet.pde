class Planet {
  PVector location;
  PVector velocity;
  PVector acceleration;
  float mass;

  Planet(float m, float x, float y) {
    mass = m;
    location = new PVector(x, y);
    // random starting velocity
    velocity = new PVector(random(1.0), random(1.0));
    acceleration = new PVector(0, 0);
  }

  // Newton's 2nd law (F = M*A) applied
  void applyForce(PVector force) {
    PVector f = PVector.div(force, mass);
    acceleration.add(f);
  }

  // our motion algorithm (aka Euler Integration)
  void update(boolean hasAttractors) {
    velocity.add(acceleration);
    location.add(velocity);
    acceleration.mult(0);

    // wrap around the display window when there are no attractors
    if (!hasAttractors) {
      while (location.x < 0) {
        location.x += width;
      }
      while (width <= location.x) {
        location.x -= width;
      }
      while (location.y < 0) {
        location.y += height;
      }
      while (height <= location.y) {
        location.y -= height;
      }
    }
  }

  void display() {
    noStroke();
    fill(255);
    ellipse(location.x, location.y, mass*8, mass*8);
  }
}
