import gohai.simpletouch.*;
SimpleTouch touchscreen;

void setup() {
  fullScreen();
  colorMode(HSB, 360, 100, 100);
  noStroke();

  println("Available input devices:");
  String[] devs = SimpleTouch.list();
  printArray(devs);

  if (devs.length == 0) {
    println("No input devices available");
    exit();
  }

  touchscreen = new SimpleTouch(this, devs[0]);
  println("Opened device: " + touchscreen.name());
}

void draw() {
  background(0);

  SimpleTouchEvt touches[] = touchscreen.touches();
  for (SimpleTouchEvt touch : touches) {
    // the id value is used to track each touch
    // we use it to assign a unique color
    fill((touch.id * 100) % 360, 100, 100);
    // x and y are values from 0.0 to 1.0
    ellipse(width * touch.x, height * touch.y, 100, 100);
  }
}
