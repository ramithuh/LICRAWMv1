/**  Author : Ramith Hettiarachchi
 * 
 * 
 * Written for LICRAWM 1.5.4 PCB by our Robo Group
 * 
 *  im@ramith.fyi
 * 
 * 
 *  **/



let xOffset = 0.0;
let yOffset = 0.0;
let bx=0;
let by=0;
let px=0;
let py=0;
var wheel_factor=0.01;

let pg;
var serial;             // Declare a "SerialPort" object
var latestData = "Waiting for data.."; 
var infoData = "..";
let robot;
var cols, rows;
var offset=37.5 + 14;



var GridSize=200;
var Zoom=0.4;
var Enable_Rotation=true;

var m1_encoder_count=0;
var m1_pre=0;

var m2_encoder_count=0;
var m2_pre=0;

var ShowMazeSquare=false;
var OrientationOffSet=0;
var Fix_Grid=false;
var Enable_Pan_Zoom=false;
var Command='b';

var angle = 0;

var TOF1=0;
var TOF2=0;
var TOF3=0;
var TOF4=0;
var TOF5=0;

var SelectedAngle=['90clock', '45clock'];
var red=true;
var blue=true;
var green;

var gui;
var menu;
var area ;
var console_area;
var Info_area;

var consoleLog=[];
var visualizationLog=[];
var infoLog=[];
var posHistory=[];
var consoleBuffer=0;

function setup() {
  green=false;
  //pixelDensity(4);
  canvas= createCanvas(windowWidth, windowHeight);

  canvas.parent('sketch-holder');


  console_area= createElement('textarea', 'Console ');
  console_area.attribute("rows","33");
  console_area.attribute("cols","76");
  console_area.attribute("readonly",true);

  area= createElement('textarea', 'Visualization Log');
  area.attribute("rows","33");
  area.attribute("cols","76");
  area.attribute("readonly",true);


  Info_area= createElement('textarea', 'Info Log');
  Info_area.attribute("rows","33");
  Info_area.attribute("cols","60");
  Info_area.attribute("readonly",true);

 
  robot = loadImage('pcb2.png'); 
  robot2 = loadImage('robot-move-encoder.png'); 

  
  rectMode(CENTER);
  angleMode(DEGREES);


  
  gui = createGui('LICRAWM Path Visualizer');
  gui.addButton("ResetBoard", function() {
    ResetBoard();
  });
  sliderRange(0, 90, 1);
  gui.addGlobals('GridSize');
  sliderRange(0.2, 3, 0.01);
  gui.addGlobals('Zoom');
  gui.addGlobals('Enable_Pan_Zoom');
  gui.addButton("Move Forward", function() {
    MoveForward();
  });

  gui.addGlobals('SelectedAngle');
  gui.addButton("Select Angle", function() {
    SelectAngle();
  });
  gui.addButton("Reset Position", function() {
    Reset_Pos();
  });
  gui.addButton("Clear Path", function() {
    Clear_path();
  });


  // Instantiate our SerialPort object
   serial = new p5.SerialPort();

  // Get a list the ports available
  // You should have a callback defined to see the results
  //serial.list();

  // Assuming our Arduino is connected, let's open the connection to it
  // Change this to the name of your arduino's serial port
  //serial.open("/dev/tty.LICRAWM-1_5_4-DevB");//
  //serial.on('list', printList);
  // Here are the callbacks that you can register
  serial.on('list', gotList);
  

  // When we connect to the underlying server
  serial.on('connected', serverConnected);

  // When we get a list of serial ports that are available
 
  // OR
  //serial.onList(gotList);

  // When we some data from the serial port
  serial.on('data', gotData);
  // OR
  //serial.onData(gotData);

  // When or if we get an error
  serial.on('error', gotError);
  // OR
  //serial.onError(gotError);

  // When our serial port is opened and ready for read/write
  serial.on('open', gotOpen);

 
  // OR
  //serial.onOpen(gotOpen);

  // Callback to get the raw data, as it comes in for handling yourself
  //serial.on('rawdata', gotRawData);
  // OR
  //serial.onRawData(gotRawData); */
  
   posHistory.push(createVector(0,0)); //dummy value

 

}


// We are connected and ready to go
function serverConnected() {
  infoData="Connected to /node backend";
  println("Connected to /node backend");
}

function openPort() {
  portName = menu.elt.value;
  println("haha");
  infoData="Port :" + portName + " opened!";
  serial.open(portName);
}


// Got the list of ports
function gotList(thelist) {
 
  println("List of Serial Ports:");


  menu = createSelect();
  var title = createElement('option', 'Choose a port:');
  menu.child(title);
  menu.position(windowWidth/2, windowHeight-50);
  menu.changed(openPort);

  for (var i = 0; i < thelist.length; i++) {
    var thisOption = createElement('option', thelist[i]);
    thisOption.value = thelist[i];
    menu.child(thisOption);
    println(i + " " + thelist[i]);
  }
}

// Connected to our serial device
function gotOpen() {
  infoData="Serial Port is Open";
  println("Serial Port is Open");
}

// Ut oh, here is an error, let's log it
function gotError(theerror) {
  infoData=theerror;
  println(theerror);
}


/// ********************* ///
// There is data available to work with from the serial port
function gotData() {
  infoData="Getting Data";
 

  var currentString = serial.readLine();  // read the incoming string
 if(visualizationLog.length>40){
    visualizationLog.pop();
  } 
  
  if(consoleLog.length>50){
    consoleLog.pop();
  }
  if(infoLog.length>150){
    infoLog.pop();
  }
 

  trim(currentString);                    // remove any trailing whitespace
  if (currentString) {
    consoleLog.unshift(currentString);
    consoleLog.unshift('\n');
  
  }else{
    return;
  }

  //make sure valid serial read
  if(currentString[0]=='T'){
  

 


    
    green=true;

    latestData = currentString.split(":");            // save it for the draw method

    TOF2 =parseInt(latestData[1]);
    TOF3 =parseInt(latestData[3]);
    TOF4 =parseInt(latestData[5]);
    TOF5 =parseInt(latestData[7]);
    angle=parseFloat(latestData[9]);
    
    m1_encoder_count=parseInt(latestData[15]);
    m2_encoder_count=parseInt(latestData[17]);
  
    visualizationLog.unshift(latestData.slice(14,18));
    visualizationLog.unshift('\n');
  
    //m2_encoder_count=parseInt(latestData[15]);
   
    by-=(m2_encoder_count-m1_pre)*cos(angle)*5;
    bx+=(m2_encoder_count-m1_pre)*sin(angle)*5;
    m1_pre=m2_encoder_count;

  
  }
}

// We got raw from the serial port
function gotRawData(thedata) {

 // println("gotRawData" + thedata);
}

// Methods available
// serial.read() returns a single byte of data (first in the buffer)
// serial.readChar() returns a single char 'A', 'a'
// serial.readBytes() returns all of the data available as an array of bytes
// serial.readBytesUntil('\n') returns all of the data available until a '\n' (line break) is encountered
// serial.readString() retunrs all of the data available as a string
// serial.readStringUntil('\n') returns all of the data available as a string until a specific string is encountered
// serial.readLine() calls readStringUntil with "\r\n" typical linebreak carriage return combination
// serial.last() returns the last byte of data from the buffer
// serial.lastChar() returns the last byte of data from the buffer as a char
// serial.clear() clears the underlying serial buffer
// serial.available() returns the number of bytes available in the buffer
// serial.write(somevar) writes out the value of somevar to the serial device


function draw_grid2(){
  var x=(width/2)%(GridSize*Zoom);
  var y= (height/2)%(GridSize*Zoom);
  
  for (var i = -width/2+2*x; i < 3*height ; i += GridSize*Zoom) {
    for (var j =  -height/2+2*y; j < 3*width; j += GridSize*Zoom) { 
        stroke(230);
        rect(i, j, GridSize*Zoom, GridSize*Zoom);
    }
  }
}
function draw_maze(){
  var x=(width/2)%(360*Zoom);
  var y= (height/2)%(360*Zoom);


  for (var i = -width/2+2*x; i < 3*height ; i += 360*Zoom) {
    for (var j = -height/2+2*y; j < 3*width; j += 360*Zoom) { 
        noFill();
        stroke(230,0,0,50);
        rect(i, j, 360*Zoom, 360*Zoom);
    }
  }
}

function draw() {
  console_area.elt.value=consoleLog.join("");  //// UPDATE TEXT AREA
  area.elt.value =visualizationLog.join("");   //// UPDATE TEXT AREA
  Info_area.elt.value =infoLog.join("");       //// UPDATE TEXT AREA


  translate(px,py)
  fill(255);
  draw_grid2(); 
  draw_maze();


  push();
      translate(-px,-py)
      let fps = frameRate();
      fill(100);
      stroke(1);
      text("FPS: " + fps.toFixed(2),  9*width/10, 9*height/10);
  pop();

  //scale(Zoom);

  for(var i=0;i < posHistory.length;i++){ //trails print
    fill(0,0,255);
    rect(posHistory[i].x+width/2,posHistory[i].y+height/2,20*Zoom,20*Zoom);
  }


  push();                 //cross mark
     translate(width/2, height/2);
     stroke(255,0,0);
     strokeWeight(3);
     line(-6,-6,6,6);
     line(-6,6,6,-6);
  pop();
  


  var posx1= (-robot2.width/2);           //robot coords
  var posy1= (-robot2.height/2-offset);  //robot coords
 
  push();
    translate(bx, by);
    /*****/
    if(posHistory[posHistory.length-1].x!=bx || posHistory[posHistory.length-1].y!=by){
      var v=createVector(bx,by);
      posHistory.push(v);
   }
    /*****/
    translate(width/2, height/2);
    scale(Zoom);

    rotate(angle);
    stroke(255,0,0);
    rect(0,0,2,2);
    image(robot2, posx1, posy1, 200 ,180);  //robot image
  pop();

}



function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
}
function ResetBoard() {
  serial.write('R');
}

function MoveForward() {
  m1_encoder_count+=10;

  by-=(m1_encoder_count-m1_pre)*cos(angle);
  bx+=(m1_encoder_count-m1_pre)*sin(angle);
  m1_pre=m1_encoder_count;
    
  //redraw();
}

function Reset_Pos() {
  bx=0;
  by=0;
  angle=0;
}
function Clear_path() {
  posHistory=[];
  posHistory.push(createVector(0,0));
}
function SelectAngle() {

  switch(SelectedAngle){
    case '90clock':
      angle+=90;
      //redraw();
      break;
    case '45clock':
      angle+=45;
     // redraw();
      break;
  }
}

function mousePressed(){
  
  xOffset = mouseX - px;
  yOffset = mouseY - py;
}
function mouseDragged() {
  if(Enable_Pan_Zoom)
  if(mouseX>windowWidth/5){
    px = mouseX - xOffset;
    py = mouseY - yOffset;
  }
}

