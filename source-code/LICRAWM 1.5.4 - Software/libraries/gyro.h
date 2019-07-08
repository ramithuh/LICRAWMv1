
float gyroXCalli = 0 ;
float gyroYCalli = 0 ;
float gyroZCalli = 0 ;
float gyroXPresent = 0 ;
float gyroYPresent = 0 ;
float gyroZPresent = 0 ;
float accelX = 0 ;
float accelY = 0 ;
float accelZ = 0 ;
float gForceX = 0 ;
float gForceY = 0 ;
float gForceZ = 0 ;
float gyroXPast = 0 ;
float gyroYPast = 0 ;
float gyroZPast = 0 ;
float timePast = 0 ;
float timePresent = 0 ;
float rotX = 0 ;
float rotY = 0 ;
float rotZ = 0 ;
float angleX = 0 ;
float angleY = 0 ;
float angleZ = 0 ;

void setUpMPU() {
  // power management
  Wire.beginTransmission(0b1101000);          // Start the communication by using address of MPU
  Wire.write(0x6B);                           // Access the power management register
  Wire.write(0b00000000);                     // Set sleep = 0
  Wire.endTransmission();                     // End the communication

  // configure gyro
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B);                           // Access the gyro configuration register
  Wire.write(0b00011000);
  Wire.endTransmission();

  // configure accelerometer
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C);                           // Access the accelerometer configuration register
  Wire.write(0b00000000);
  Wire.endTransmission();  
}

void getGyroValues() {
  Wire.beginTransmission(0b1101000);                          // Start the communication by using address of MPU 
  Wire.write(0x43);                                           // Access the starting register of gyro readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);                              // Request for 6 bytes from gyro registers (43 - 48)
  while(Wire.available() < 6);                                // Wait untill all 6 bytes are available
  gyroXPresent = Wire.read()<<8|Wire.read();                  // Store first two bytes into gyroXPresent
  gyroYPresent = Wire.read()<<8|Wire.read();                  // Store next two bytes into gyroYPresent
  gyroZPresent = Wire.read()<<8|Wire.read();                  //Store last two bytes into gyroZPresent
}

void callibrateGyroValues() {
    for (int i=0; i<100; i++) {
      getGyroValues();
      gyroXCalli = gyroXCalli + gyroXPresent;
      gyroYCalli = gyroYCalli + gyroYPresent;
      gyroZCalli = gyroZCalli + gyroZPresent;
    }
    gyroXCalli = gyroXCalli/100;
    gyroYCalli = gyroYCalli/100;
    gyroZCalli = gyroZCalli/100;
}

void processAccelData() {
  gForceX = accelX/16384.0;
  gForceY = accelY/16384.0; 
  gForceZ = accelZ/16384.0;
}

void readAndProcessAccelData() {
  Wire.beginTransmission(0b1101000); 
  Wire.write(0x3B); 
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); 
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); 
  accelY = Wire.read()<<8|Wire.read(); 
  accelZ = Wire.read()<<8|Wire.read(); 
  processAccelData();
}

void getAngularVelocity() {
  rotX = gyroXPresent / 16.4;                                
  rotY = gyroYPresent / 16.4; 
  rotZ = gyroZPresent / 16.4;
}

void calculateAngle() {  
  // same equation can be written as 
  // angleZ = angleZ + ((timePresentZ - timePastZ)*(gyroZPresent + gyroZPast - 2*gyroZCalli)) / (2*1000*131);
  // 1/(1000*2*131) = 0.00000382
  // 1000 --> convert milli seconds into seconds
  // 2 --> comes when calculation area of trapezium
  // substacted the callibated result two times because there are two gyro readings
  angleX = angleX + ((timePresent - timePast)*(gyroXPresent + gyroXPast - 2*gyroXCalli)) * 0.00003049;
  angleY = angleY + ((timePresent - timePast)*(gyroYPresent + gyroYPast - 2*gyroYCalli)) * 0.00003049;
  angleZ = angleZ + ((timePresent - timePast)*(gyroZPresent + gyroZPast - 2*gyroZCalli)) * 0.00003049;
}

void readAndProcessGyroData() {
  gyroXPast = gyroXPresent;                                   // Assign Present gyro reaging to past gyro reading
  gyroYPast = gyroYPresent;                                   // Assign Present gyro reaging to past gyro reading
  gyroZPast = gyroZPresent;                                   // Assign Present gyro reaging to past gyro reading
  timePast = timePresent;                                     // Assign Present time to past time
  timePresent = millis();                                     // get the current time in milli seconds, it is the present time
  
  getGyroValues();                                            // get gyro readings
  getAngularVelocity();                                       // get angular velocity
  calculateAngle();                                           // calculate the angle  
}

/*
void setup(){
  Serial.begin(9600);
  setUpMPU();
  callibrateGyroValues();
  }
void loop() {
  
  readAndProcessGyroData();
  
  Serial.print("angleX : ");
  Serial.print(angleX);
  Serial.print(" angleY : ");
  Serial.print(angleY);
  Serial.print(" angleZ : ");
  Serial.println(angleZ);
  
  delay(50);
  }
  */
