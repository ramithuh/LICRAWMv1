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

var line_array_threshold=500;

var serial;             // Declare a "SerialPort" object
var latestData = "Waiting for data.."; 
var infoData = "..";
let robot;
var cols, rows;
var offset=37.5 + 14;
var GridSize=50;
var Zoom=1;
var Enable_Rotation=true;

let arduino_mega;
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

var SelectLED=['Blue', 'Red','Green(Turns on/off Gyro)'];
var SelectRot=['z','y','x'];
var red=true;
var blue=true;
var green;
var SelectPID=['KP','KI','KD'];
var SelectVal=['+100','-100','+50','-50','+20','-20','+10','-10','+5','-5','+1','-1'];

var gui;
var menu;
var area ;
var console_area;
var Info_area;

var consoleLog=[];
var visualizationLog=[];
var infoLog=[];
var line_array_readings=[]
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

 
  robot = loadImage('pcb-small.png'); 
  robot2 = loadImage('pcb3.png'); 
  //robot = loadImage('robot-2.png'); 
  arduino_mega = loadImage('arduino_mega.png');
  line_array = loadImage('line_array.jpg');
  //arduino_mega2 = loadImage('arduino_mega_small.png');
  rectMode(CENTER);
  angleMode(DEGREES);


  
  gui = createGui('LICRAWM 2D Visualizer');
  gui.addButton("ResetBoard", function() {
    ResetBoard();
  });
  sliderRange(0, 90, 1);
  gui.addGlobals('GridSize');
  sliderRange(0, 360, 5);
  gui.addGlobals('OrientationOffSet');
  sliderRange(0.3, 3, 0.01);
  gui.addGlobals('Zoom');
  gui.addGlobals('Fix_Grid','Enable_Rotation','ShowMazeSquare','Enable_Pan_Zoom');
  
  gui.addButton("Reset Window", function() {
    ResetWindow();
  });

  gui.addGlobals('SelectLED');
  gui.addButton("Toggle LED", function() {
    ToggleLED();
  });
  sliderRange(0, 1023,1);
  gui.addGlobals('line_array_threshold');
  gui.addGlobals('SelectRot');
  gui.addGlobals('Command');
  gui.addButton("Serial Send", function() {
    SendCommand();
  });
  gui.addButton("Start/Stop Motors", function() {
    StartStop();
  });
  gui.addGlobals('SelectPID');
  gui.addGlobals('SelectVal');
  
  
  gui.addButton("PID Change", function() {
    SendValue();
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

  //console.log(currentString);             // println the string
  if(currentString[0]=='=' || currentString[0]=='|' || currentString=="-" || currentString==">"){
    infoLog.unshift(currentString);
    infoLog.unshift('\n');
  }

  //make sure valid serial read
  if(currentString[0]=='T'){
  

    visualizationLog.unshift(currentString);
    visualizationLog.unshift('\n');
    
    green=true;

    latestData = currentString.split(":");            // save it for the draw method

    TOF1 =parseInt(latestData[1]);
    if(TOF1>65000){
      TOF1=9.99;
    }
    TOF2 =parseInt(latestData[3]);
    if(TOF2>65000){
      TOF2=9.99;
    }
    TOF3 =parseInt(latestData[5]);
    if(TOF3>65000){
      TOF3=9.99;
    }
    TOF4 =parseInt(latestData[7]);
    if(TOF4>65000){
      TOF4=9.99;
    }
    TOF5 =parseInt(latestData[9]);
    if(TOF5>65000){
      TOF5=9.99;
    }
    
    
    switch(SelectRot){
      case 'x':
          angle=-parseFloat(latestData[15]);
          break;
      case 'y':
          angle=-parseFloat(latestData[13]);
          break;
      case 'z':
          angle=-parseFloat(latestData[11]);
          break;
    }
    //
    
    line_array_readings[1]=parseFloat(latestData[21]);
    line_array_readings[2]=parseFloat(latestData[22]);
    line_array_readings[3]=parseFloat(latestData[23]);
    line_array_readings[4]=parseFloat(latestData[24]);
    line_array_readings[5]=parseFloat(latestData[25]);
    line_array_readings[6]=parseFloat(latestData[26]);
    line_array_readings[7]=parseFloat(latestData[27]);
    line_array_readings[8]=parseFloat(latestData[28]);
    line_array_readings[9]=parseFloat(latestData[29]);
    line_array_readings[10]=parseFloat(latestData[30]);
    line_array_readings[11]=parseFloat(latestData[31]);
    line_array_readings[12]=parseFloat(latestData[32]);
    line_array_readings[13]=parseFloat(latestData[33]);
    line_array_readings[14]=parseFloat(latestData[34]);
    line_array_readings[15]=parseFloat(latestData[35]);

    line_array_threshold=parseFloat(latestData[39]);

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
  //background(254);
  for (var x = 0; x < width; x +=GridSize*Zoom) {
		for (var y = 0; y < height; y += GridSize*Zoom) {
      //background(234);
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
  background(240);

  
  image(line_array, windowWidth - 61*5 , windowHeight-20*5,61*4,20*4)
  push();
    fill(0,153,204,196);
    rect(windowWidth - 61*5 +61*2 ,windowHeight- 20*5 + 20*2,61*4,20*4);
    pop();
  var i;
  for(i=1;i<=15;i++){
    push();
    if(line_array_readings[i]>line_array_threshold){
      fill(0);
    }else{
      fill(255);
    }
    rect(windowWidth - 61*5 +8.1*(2*i-1),windowHeight- 20*5 + 32 ,10,10); //1
    pop();
  }
  


  console_area.elt.value=consoleLog.join("");
  area.elt.value =visualizationLog.join("");
  Info_area.elt.value =infoLog.join("");

  
  push();
  let fps = frameRate();
  fill(100);
  stroke(1);
  text("FPS: " + fps.toFixed(2),  9*width/10, 9*height/10 -20*5);
  pop();

  translate(bx,by); // pan on mouse drag

  var posx1=- robot.width/2;           //robot coords
  var posx2=  robot.width/2;           //robot coords
  var posy1= -robot.height/2+offset;  //robot coords
  var posy2=  robot.height/2+offset;  //robot coords
 
  translate(width/2, height/2);  //needed for rotation about robo axis(wheels)


  if(ShowMazeSquare){
    fill(255);
    rect(0, 0, 360*Zoom , 360*Zoom);    //36cm 36cm square
  }
  
 
  if(Fix_Grid){ draw_grid();}
  if(Enable_Rotation)rotate(angle);
  rotate(OrientationOffSet);
  if(!Fix_Grid){ draw_grid();}
 
  scale(Zoom); //zoom factor

  

 if(mouseX > bx+width/2+(posx1)*Zoom && mouseX < bx+width/2+(posx1+200)*Zoom && mouseY > by+height/2 + (posy1)*Zoom && mouseY < by+height/2 +(posy2)*Zoom){
  stroke(0);
  strokeWeight(2)
  fill(244, 122, 158);
  rect(0,-posy1+13,200,180); 
 }
  
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
  line(-10,posy2+TOF1, 10  ,posy2+TOF1);   //ToF1
  line(posx1- TOF2,posy2-65,  posx1 -TOF2  ,posy2-45);   //TOF2
  line(posx1- TOF3,posy2-100, posx1 -TOF3  ,posy2-80);   //TOF3
  line(posx2+ TOF4,posy2-100, posx2 +TOF4  ,posy2-80);   //ToF4
  line(posx2+ TOF5,posy2-65,  posx2 +TOF5  ,posy2-45);   //ToF5

  fill(0);
  strokeWeight(1)
  text(TOF1+"mm", -13, posy2+20+TOF1);
  text(TOF2 + "mm", posx1- TOF2 - 40, posy2- 50 );
  text(TOF3 + "mm", posx1- TOF3 - 40, posy2- 85 );
  text(TOF4 + "mm", posx2+ TOF4 + 5, posy2- 85 );
  text(TOF5 + "mm", posx2+ TOF5 + 5, posy2- 50 );
  



}
function mousePressed(){
  
  xOffset = mouseX - bx;
  yOffset = mouseY - by;
}
function mouseDragged() {
  if(Enable_Pan_Zoom)
  if(mouseX>windowWidth/5){
    bx = mouseX - xOffset;
    by = mouseY - yOffset;
  }
}

function mouseWheel(event) {


  print(event.delta);
  //move the square according to the vertical scroll amount
  //if(event.delta>0){
    if(mouseY < windowHeight*2/3 && Enable_Pan_Zoom){
      Zoom+=event.delta*0.01;
    }
  //}
  //uncomment to block page scrolling
  //return false;
  return false;
}


function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
}
function ResetBoard() {

  serial.write('R');

}

function ResetWindow() {

  Zoom=1;
  bx=0;
  by=0;

}

function SendCommand() {
  serial.write(Command);
}
function StartStop() {
  serial.write('*');
}

function send_pid_value(mode){
    if(mode==1){
      serial.write('+');
     // console.log('i');


    }else{
      serial.write('-');
    //  console.log('i');

    }
}
function delay(ms) {
  var cur_d = new Date();
  var cur_ticks = cur_d.getTime();
  var ms_passed = 0;
  while(ms_passed < ms) {
      var d = new Date();  // Possible memory leak?
      var ticks = d.getTime();
      ms_passed = ticks - cur_ticks;
      // d = null;  // Prevent memory leak?
  }
}

function multiplesend(l,mode){

  switch(SelectPID){
    case 'KP':
      serial.write('p');
      break;
    case 'KI':
      serial.write('i');
      break;
    case 'KD':
      serial.write('d');
      break;
  }
  for(var i=0;i<l;i++){
    send_pid_value(mode);
    delay(150);
  } 
  

}

function SendValue() {

  switch(SelectVal){
    case '+100':
      multiplesend(100,1);
      break;
    case '-100':
      multiplesend(100,0);
      break;
    case '+50':
      multiplesend(50,1);
      break;
    case '-50':
      multiplesend(50,0);
      break;
    case '+10':
      multiplesend(10,1);
      break;
    case '-10':
      multiplesend(10,0);
      break;
    case '+20':
      multiplesend(20,1);
      break;
    case '-20':
      multiplesend(20,0);
      break;
    case '+5':
      multiplesend(5,1);
      break;
    case '-5':
      multiplesend(5,0);
      break;
    case '+1':
      multiplesend(1,1);
      break;
    case '-1':
      multiplesend(1,0);
      break;
    
  }
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
    case 'Green(Turns on/off Gyro)':
      serial.write('g');
      break;
  }
}

