PShape s;
float angle = 0.1; //rads
s = loadShape("robot.svg");
s.rotate(angle);
//I change this angle manually or with my clickMouse function which isnt shown.

void setup(){
    size(400,350);
frameRate(30); //30 frames per seconds
}

void draw(){ //shape( shape, x, y, width, height);
    smooth();
    fill(153);
    ellipse(200, 350/2, 100, 100);
    shape(s, 200, 350/2, 20, 20);
    ellipse(200, 350/2, 2, 2);
}
