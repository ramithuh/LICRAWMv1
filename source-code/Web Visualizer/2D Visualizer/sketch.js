// Declare a "SerialPort" object
var serial;
var latestData = "Waiting for data..";  // you'll use this to write incoming data to the canvas
var infoData = "..";
let robot;
var cols, rows;
var offset=37.5 + 14;
var GridSize=50;

let arduino_mega;
var ShowMazeSquare=false;
var OrientationOffSet=0;
var Fix_Grid=false;


var angle = 0;

var TOF1=0;
var TOF2=0;
var TOF3=0;
var TOF4=0;
var TOF5=0;

var SelectLED=['Blue', 'Red','Green(Turns on/off Gyro)'];
var red=true;
var blue=true;
var green;

var gui;
function setup() {
  green=false;
  //pixelDensity(4);
  createCanvas(windowWidth, windowHeight);
  
  robot = loadImage('pcb-small.png'); 
  robot2 = loadImage('pcb2.png'); 
  //robot = loadImage('robot-2.png'); 
  arduino_mega = loadImage('arduino_mega.png');
  //arduino_mega2 = loadImage('arduino_mega_small.png');
  rectMode(CENTER);
  angleMode(DEGREES);


  sliderRange(0, 90, 1);
  gui = createGui('LICRAWM 2D Visualizer');
  gui.addButton("ResetBoard", function() {
    ResetBoard();
  });
  gui.addGlobals('GridSize','ShowMazeSquare');
  sliderRange(0, 360, 5);
  gui.addGlobals('OrientationOffSet','Fix_Grid');
  gui.addGlobals('SelectLED')
  gui.addButton("Toggle LED", function() {
    ToggleLED();
  });
 
 


  // Instantiate our SerialPort object
  serial = new p5.SerialPort();

  // Get a list the ports available
  // You should have a callback defined to see the results
  serial.list();

  // Assuming our Arduino is connected, let's open the connection to it
  // Change this to the name of your arduino's serial port
  serial.open("/dev/tty.LICRAWM-1_5_4-DevB");

  // Here are the callbacks that you can register

  // When we connect to the underlying server
  serial.on('connected', serverConnected);

  // When we get a list of serial ports that are available
  serial.on('list', gotList);
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
  //serial.onRawData(gotRawData); 
  
}

// We are connected and ready to go
function serverConnected() {
  infoData="Connected to Server";
  println("Connected to Server");
}

// Got the list of ports
function gotList(thelist) {
 
  println("List of Serial Ports:");
  // theList is an array of their names
  for (var i = 0; i < thelist.length; i++) {
    // Display in the console
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

// There is data available to work with from the serial port
function gotData() {
  infoData="Getting Data";
  green=true;
  var currentString = serial.readLine();  // read the incoming string

  trim(currentString);                    // remove any trailing whitespace
  if (!currentString) return;             // if the string is empty, do no more

  console.log(currentString);             // println the string
  
  //make sure valid serial read
  if(currentString[0]=='T'){
    latestData = currentString.split(":");            // save it for the draw method

    TOF2 =parseInt(latestData[1]);
    TOF3 =parseInt(latestData[3]);
    TOF4 =parseInt(latestData[5]);
    TOF5 =parseInt(latestData[7]);
    angle=parseFloat(latestData[9]);
  }
}

// We got raw from the serial port
function gotRawData(thedata) {
  println("gotRawData" + thedata);
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
			stroke(204, 102, 0);
			strokeWeight(0.04);
      line(-x, -height, -x, height);
      line(x, -height, x, height);
      line(-width, y, width, y);
      line(-width, -y, width, -y);
		}
  }
}
function draw() {
  background(220);
 


  var posx1=- robot.width/2;
  var posx2=  robot.width/2;
  var posy1= -robot.height/2+offset;
  var posy2=  robot.height/2+offset;
 
  translate(width/2, height/2);

  

  if(ShowMazeSquare){
    fill(255);
    rect(0, 0, 360 , 360); //36cm 36cm square
  }
 
  if(Fix_Grid)draw_grid();
  rotate(angle + OrientationOffSet);
 
  if(!Fix_Grid)draw_grid();
 

 // robot.resize(200, 180);
  image(robot2, posx1 , posy1,200,180);  //robot image
  image(arduino_mega, -26.5 , posy1-5,53,108);//arduino mega pic
  rect(0, 0, 2 , 2);
  
  
  if(red){fill(225,0,0);strokeWeight(0.8); stroke(255); ellipse(posx1 + 30, posy1 +   7, 7, 7);}
  if(blue){fill(0,0,255);strokeWeight(0.8);  stroke(255); ellipse(posx1 + 52, posy1 + 7, 7, 7);}
  if(green){fill(0,255,0);strokeWeight(0.8);  stroke(255); ellipse(posx1 + 41, posy1 + 7, 7, 7);}

  
  

  fill(0);
  text(latestData, posx2, posy2);
  text(infoData, -width/4, posy2+150);
  fill(100, 102, 153, 51);
  text("RotZ: " + angle, -10, posy2+15);  //rotation text

  stroke(255,0,0);
  strokeWeight(4)
 // line(posx1 ,50, posx1   ,300);   //ToF3
  line(posx1- TOF2,posy2-65,  posx1 -TOF2  ,posy2-45);   //TOF2
  line(posx1- TOF3,posy2-100, posx1 -TOF3  ,posy2-80);   //TOF3
  line(posx2+ TOF4,posy2-100, posx2 +TOF4  ,posy2-80);   //ToF4
  line(posx2+ TOF5,posy2-65,  posx2 +TOF5  ,posy2-45);   //ToF5
}



function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
}
function ResetBoard() {
  serial.write('R');
}

function ToggleLED() {

  switch(SelectLED){
    case 'Blue':
      serial.write('b');
      blue=!blue
      break;
    case 'Red':
      serial.write('r');
      red=!red;
      break;
    case 'Green':
      serial.write('g');
      break;


  }
  
}
