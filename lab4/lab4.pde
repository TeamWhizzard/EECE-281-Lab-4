 import processing.serial.*;
 
 Serial myPort;        // The serial port TODO
 int tempPos = 2;         // horizontal position of the graph
 int humPos = 1
 
 void setup () {
   // set the window size:
   size(400, 300);        
   
   // List all the available serial ports
   println(Serial.list());
   // I know that the first port in the serial list on my mac
   // is always my  Arduino, so I open Serial.list()[0].
   // Open whatever port is the one you're using.
   myPort = new Serial(this, Serial.list()[0], 115200);
   // don't generate a serialEvent() unless you get a newline character:
   myPort.bufferUntil('\n');
   // set inital background:
   background(0);
 }
 
 void draw () {
   // everything happens in the serialEvent()
 }
 
 // runs each time something is written to serial monitor?
 void serialEvent (Serial myPort) {
   // get the ASCII string:
   String inString = myPort.readStringUntil('\n');
   
   if (inString != null) {
     // trim off any whitespace - leading and trailing
     inString = trim(inString);
     
     float temp = float(inString.split(" ")[0]);
     float humid = float(inString.split(" ")[1]);
     // converts range in temperature / humidity values to range that will fit on screen
     temp = map(temp, -30, 40, 0, height);
     humid = map(humid, 0, 100, 0 height);
     
     // draw the line:
     stroke(127,34,255);
     line(tempPos, height, tempPos, height - temp);
     line(humPos, height, humPos, height - humid);
     
     // at the edge of the screen, go back to the beginning:
     if (tempPos >= width) {
       tempPos = 0;
       background(0);
     }
     else {
       // increment the horizontal position:
       tempPos++;
     }
     
     if (humPos >= width) {
       humPos = 0;
       background(0);
     }
     else {
       // increment the horizontal position:
       humPos++;
     }
   }
 }
