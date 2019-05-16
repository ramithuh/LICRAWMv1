float angle=PI/8;
PShape robot;
void setup() {
  size(500, 500);
  smooth();
  angle = 0;
  robot = loadShape("robot.svg");
}

void draw() {
  background(150);
  rectMode(CENTER);

  translate(width/2, height/2);
  rotate(radians(angle));

  rect(0, 0, 200, 200);
  shape(robot, -robot.width/2, robot.height/2);  
 // angle += 1;
}
