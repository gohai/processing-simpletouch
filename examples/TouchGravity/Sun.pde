class Sun {
  float mass;
  PVector location;
  // universal gravitational constant (arbitrary value)
  float G;

  Sun(float x, float y) {
    location = new PVector(x, y);
    mass = 20;
    G = 0.4;
  }

  PVector attract(Planet p) {
    PVector force = PVector.sub(location, p.location);   // calculate direction of force
    float d = force.mag();                               // distance between objects
    d = constrain(d, 5.0, 25.0);                         // limiting the distance to eliminate "extreme" results for very close or very far objects
    float strength = (G * mass * p.mass) / (d * d);      // calculate gravitional force magnitude
    force.setMag(strength);                              // get force vector -> magnitude * direction
    return force;
  }

  void display() {
    stroke(255);
    noFill();
    ellipse(location.x, location.y, mass*4, mass*4);
  }
}
