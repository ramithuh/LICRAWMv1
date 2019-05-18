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
var wheel_factor=0.01;

let pg;
var serial;             // Declare a "SerialPort" object
var latestData = "Waiting for data.."; 
var infoData = "..";
let robot;
var cols, rows;
var offset=37.5 + 14;
var GridSize=20;
var Zoom=0.2;
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
var consoleBuffer=0;

function setup() {
  green=false;
  //pixelDensity(4);
  createCanvas(windowWidth, windowHeight);

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

  
  gui.addButton("Move Forward", function() {
    MoveForward();
  });

  gui.addGlobals('SelectedAngle');
  gui.addButton("Select Angle", function() {
    SelectAngle();
  });
  gui.addGlobals('Command');
  gui.addButton("Serial Send", function() {
    SendCommand();
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
  

    visualizationLog.unshift(currentString);
    visualizationLog.unshift('\n');


    
    green=true;

    latestData = currentString.split(":");            // save it for the draw method

    TOF2 =parseInt(latestData[1]);
    TOF3 =parseInt(latestData[3]);
    TOF4 =parseInt(latestData[5]);
    TOF5 =parseInt(latestData[7]);
    angle=parseFloat(latestData[9]);
    
    m2_encoder_count=parseInt(latestData[17]);
    infoLog.unshift(m1_encoder_count);
    infoLog.unshift('\n');

  
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
function draw_grid(){

  for (var x = 0; x < width; x +=GridSize) {
		for (var y = 0; y < height; y += GridSize) {
			stroke(195);
      strokeWeight(0.04);
      /*
      line(-x, -height, -x, height);
      line(x, -height, x, height);
      line(-width, y, width, y);
      line(-width, -y, width, -y);*/
      line(x, 0, x, height);
			line(0, y, width, y);
		}
  }
}

function draw_grid2(){
 
  for (var i = 0; i < 2*height ; i += GridSize) {
    for (var j = 0; j < 2*width ; j += GridSize) { 
 
        noFill();
        stroke(225);
        rect(i, j, GridSize, GridSize);
    }
  }


}

function draw() {

  
  push();
  translate(width/2, height/2);
  stroke(255,0,0);
  strokeWeight(3);
  line(-6,-6,6,6);
  line(-6,6,6,-6);
  pop();
  


  console_area.elt.value=consoleLog.join("");
  area.elt.value =visualizationLog.join("");
  Info_area.elt.value =infoLog.join("");

  var posx1= (-robot2.width/2)*Zoom;           //robot coords
  var posy1= (-robot2.height/2-offset)*Zoom;  //robot coords
 
  push();
    translate(bx, by);
    translate(width/2, height/2);
    rotate(angle);
    stroke(255,0,0);
    rect(0,0,1,1);
    image(robot2, posx1, posy1, 200*Zoom ,180*Zoom);  //robot image
  pop();

 draw_grid2();



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

function SendCommand() {
  serial.write(Command);
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

