 import processing.serial.*;
 
 Serial myPort;
 
 static String delim = " ";
 
 int tempPos = 3;         // horizontal position of the graph
 int humPos = 2;
 int lightPos = 1;

 int partH = height / 4; // partial height for deviding screen
 int xPos = 0;
 
 void setup () {
   // set the window size:
   size(800, 600);        
   
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
     
     float temp = int(inString.split(delim) [0]);
     float humid = int(inString.split(delim) [1]);
     float light = int(inString.split(delim) [2]);
     
     // converts range in temperature / humidity values to range that will fit on screen
     //temp = map(temp, 2, 150, 0, height);
     //humid = map(humid, 0, 100, 0, height);
     light = map(light, 0, 100, 0, height);
     
     // sample code from the thing..
     // TODO axis labels
     
     
     // draw the line:
     strokeWeight(10); // beastly
     strokeCap (SQUARE); // makes square line endings for bars    
     stroke(165,6,121); // line color
     //line(lightPos, height, lightPos, height - light);
     line (xPos, partH * lightPos, xPos, partH * lightPos - light); 
     
     //stroke(221, 159, 7); // yellow
     //stroke(10,103,139); // blue
     //line(tempPos, height, tempPos, height - temp);
     //line(humPos, height, humPos, height - humid);
     
     /*
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
     } */
     
     /*if (lightPos >= width) {
       lightPos = 0;
       background(0);
     }
     else {
       // increment the horizontal position:
       lightPos += 11;
     }*/
   }
 }
