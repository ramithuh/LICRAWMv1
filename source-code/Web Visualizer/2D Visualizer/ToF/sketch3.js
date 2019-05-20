/**  Author : Ramith Hettiarachchi
 * 
 * 
 * Written for LICRAWM 1.5.4 PCB by our Robo Group
 * 
 *  im@ramith.fyi
 * 
 * 
 *  **/





var serial;             // Declare a "SerialPort" object
var latestData = "Waiting for data.."; 
var infoData = "..";



var m1_encoder_count=0;
var m2_encoder_count=0;

var angle = 0;

var TOF1=0.0;
var TOF2=0.0;
var TOF3=0.0;
var TOF4=0.0;
var TOF5=0.0;


function setup() {



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
 

  trim(currentString);                    // remove any trailing whitespace
  if (currentString) {

  
  }else{
    return;
  }

  //make sure valid serial read
  if(currentString[0]=='T'){

    latestData = currentString.split(":");            // save it for the draw method

    TOF2 =parseFloat(latestData[1]);
    TOF3 =parseFloat(latestData[3]);
    TOF4 =parseFloat(latestData[5]);
    TOF5 =parseFloat(latestData[7]);
    angle=parseFloat(latestData[9]);
    
    m1_encoder_count=parseInt(latestData[15]);
    m2_encoder_count=parseInt(latestData[17]);
  
 
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




function draw() {

}



