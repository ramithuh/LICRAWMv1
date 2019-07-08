/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */

float angle=0;
import processing.serial.*;
PShape robot;
PShape robot2;
Serial myPort;  // Create object from Serial class
String val;      // Data received from the serial port


int sizex=700;
int sizey=700;
void setup() 
{
  size(700, 700);
 
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
   printArray(Serial.list());
  String portName = Serial.list()[8];
  myPort = new Serial(this, portName, 230400);
  

  robot = loadShape("robot.svg");
  robot2 = loadShape("robot.svg");

 
  robot.rotate(3.1428571429);  
}

void draw()
{
  rectMode(CENTER);
  rotate(radians(angle));
  translate(width/2, height/2);
  rect(0, 0, 200, 200);
  angle+=1;
  
  
  background(225);
  strokeWeight(2);
  stroke(255,0,0);
   frame.setTitle(mouseX + ", " + mouseY);
  int posx=0;
  //sizex-(sizex-200)/2;
  int posy=0;
  //(sizey-200)/2;
  shape(robot, posx, posy);  
  
  rect(posx,posy,5,5);
  rect((sizex-200)/2,posy,5,5);
  
 

  
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.readStringUntil(10);        // read it and store it in val
    myPort.clear();
    try{
    
    String c=val;
    
    
    if(c.length()<=18 && c.length()>13 && c.charAt(0)=='T'){ //got ToF line info list
      print(c); 
      String[] list = split(c,":"); //split by :
      println(list[3]);
      line(posx+int(list[1]),posy+75, posx+int(list[1]), posy+105);
      line((sizex-200)/2-float(list[3]),posy+75,(sizex-200)/2-float(list[3]), posy+105);
    }
  

      }catch (NullPointerException e) {
      }

  }
  

}
