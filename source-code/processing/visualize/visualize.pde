/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */

float angle  =0;
float p_angle=0;
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


}

void draw(){
 
  rectMode(CENTER);
  translate(width/2, height/2);
  rotate(radians(angle));
  
  background(225);
  
  
  frame.setTitle(mouseX + ", " + mouseY);
  float posx= -robot.width/2;
  float posy= robot.height/2;
  

  shape(robot, posx, posy);  
  
  rect(posx,posy,5,5); //left bottom
  rect(posx+robot.width,posy,5,5); //left right
  rect(posx,posy-robot.height,5,5);
  rect(posx+robot.width,posy-robot.height,5,5);
   
  strokeWeight(4);
  stroke(255,0,0);
  
  if (myPort.available() > 0) {  // If data is available,
    val = myPort.readStringUntil(10);        // read it and store it in val
    myPort.clear();
    try{
    
    String c=val;
    
    
    if(c.length()<=18 && c.length()>13 && c.charAt(0)=='T'){ //got ToF line info list
      //print(c); 
      String[] list = split(c,":"); //split by :
      
      line(posx+robot.width+float(list[3]),posy-100,posx+robot.width+float(list[3]),posy-80);
      line(posx-float(list[1]),posy-100,posx-float(list[1]),posy-80);
    }else if(c.length()>35 && c.charAt(0)=='a'){ //got ToF line info list
      print(c); 
      String[] list = split(c,":"); //split by :

      angle=float(list[5]);
   
    }
  

    }catch (NullPointerException e) {
    }

  }
  
   


}
