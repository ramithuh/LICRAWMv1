// Declare a "SerialPort" object
var serial;
var latestData = "Waiting for data..";  // you'll use this to write incoming data to the canvas
var infoData = "..";
let robot;
var cols, rows;

var videoScale=50;


var ax = 0;
var ay = 0;
var az = 0;
var TOF1=0;
var TOF2=0;
var TOF3=0;
var TOF4=0;
var TOF5=0;


function setup() {
  createCanvas(windowWidth, windowHeight,WEBGL);
  

  robot = loadImage('robot-tex.jpg'); 
  rectMode(CENTER);
  angleMode(DEGREES);
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
    az=parseFloat(latestData[9]);
    ay=parseFloat(latestData[11]);
    ax=parseFloat(latestData[13]);
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

function draw() {
  let locX = mouseX - height / 2;
  let locY = mouseY - width / 2;

  ambientLight(50);
  directionalLight(255, 0, 0, 0.25, 0.25, 0);
  pointLight(0, 0, 255, locX, locY, 250);
  background(0);

 
  for (var x = 0; x < width; x +=50) {
		for (var y = 0; y < height; y += 50) {
			stroke(204, 102, 0);
			strokeWeight(0.04);
      line(-x, -height, -x, height);
      line(x, -height, x, height);
      line(-width, y, width, y);
      line(-width, -y, width, -y);
		}
  }


 
  translate(width/height, height/width);
  rotate(-az);

  rotateX(ax);
  rotateY(-ay);

  //rotateY(-ax);
  push();
 // specularMaterial(0);
  texture(robot);
  //specularMaterial(250);
  box(200,180,10);
  pop();

  
  var posx1=- robot.width/2;
  var posx2=  robot.width/2
  var posy1= -robot.height/2;
  var posy2=  robot.height/2

 // image(robot, posx1 , posy1);  //robot image
    /*


  text(latestData, posx2, posy2);
  text(infoData, -width/4, posy2+150);
  //fill(100, 102, 153, 51);
  text("RotZ: " + angle, -10, posy2+15);

  stroke(255,0,0);
  strokeWeight(4)
 // line(posx1 ,50, posx1   ,300);   //ToF3
  line(posx1- TOF2,posy2-65,  posx1 -TOF2  ,posy2-45);   //TOF2
  line(posx1- TOF3,posy2-100, posx1 -TOF3  ,posy2-80);   //TOF3
  line(posx2+ TOF4,posy2-100, posx2 +TOF4  ,posy2-80);   //ToF4
  line(posx2+ TOF5,posy2-65,  posx2 +TOF5  ,posy2-45);   //ToF5*/


}



function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
}