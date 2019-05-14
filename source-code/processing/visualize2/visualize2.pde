/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */
import processing.serial.*;
float angle  =0;

PShape robot;

Serial myPort;  // Create object from Serial class
String val;      // Data received from the serial port
int tracking=0;

int sizex=1000;
int sizey=700;
int cols, rows;
int videoScale=50;

void setup() 
{
  frameRate(300);
  size(1000, 700);
 

  printArray(Serial.list());
  String portName = "/dev/tty.LICRAWM-1_5_4-DevB";
  myPort = new Serial(this, portName, 230400);

  robot = loadShape("robot.svg");
  
  cols = width/videoScale;
  rows = height/videoScale;

}

void draw(){
  

 
  rectMode(CENTER);
  translate(width/2, height/2);
  rotate(radians(angle));
  
  background(225);
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      int x = i*videoScale;
      int y = j*videoScale;
      fill(205);
      stroke(70);
      strokeWeight(0.5);
      rect(-x+width/2, y-height/2, videoScale, videoScale);
    }
   }
  
    /**/
  if(tracking==1){
     textSize(15);
     fill(255, 102, 153);
     text("Click to restart board", -10, 30);
     
   }else{
     textSize(15);
     fill(255, 102, 153);
     text("Click to start Tracking", -10, 30);
   }
  
  float posx= -robot.width/2;
  float posy= robot.height/2;
  

  shape(robot, posx, posy);  
  
  rect(posx,posy,5,5); //left bottom
  rect(posx+robot.width,posy,5,5); //left right
  rect(posx,posy-robot.height,5,5);
  rect(posx+robot.width,posy-robot.height,5,5);
  
   if(mousePressed)
    if(mouseX>width/2+posx && mouseX<width/2+posx+robot.width && mouseY<height/2+posy && mouseY>height/2+posy-robot.height){
     
       background(0);
  
       
       if(tracking==0){
          myPort.write('o');
          tracking=1;
       }else{
          myPort.write('R');
          rotate(0);
       }
   }
  
   
  strokeWeight(4);
  stroke(255,0,0);
  
  if(myPort.available() > 0) {  // If data is available,
  
    val = myPort.readStringUntil(10);        // read it and store it in val
    myPort.clear();
    
    try{
      String c=val;
    
        if(c.length()>=18 && c.charAt(0)=='T'){ //got ToF line info list
    
           String[] list = split(c,":"); //split by :
            
           line(posx+robot.width+float(list[3]),posy-100,posx+robot.width+float(list[3]),posy-80);
           line(posx-float(list[1]),posy-100,posx-float(list[1]),posy-80);
           
           textSize(13);
           fill(0, 0, 0);
           text(list[3]+"mm", posx+robot.width+float(list[3])+10, posy-85);
           text(list[1]+"mm", posx-float(list[1])- 55, posy-85);
            
            
            angle=float(list[5]);
            text(list[5].substring(0,list[5].length()-1)+"°",posx+robot.width/2-10, posy+20);
            
            tracking=1;
          
          }

    }catch (NullPointerException e) {
     
    }

  }
  
  
  /*grid info */
  textSize(10);
  fill(0, 0, 0);
  text("1 Square = 5cm",posx+robot.width/2-10, posy+120);
      


}
