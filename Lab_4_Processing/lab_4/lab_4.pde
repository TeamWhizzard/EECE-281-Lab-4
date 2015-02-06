 import processing.serial.*;

 static String DELIM = " ";
 
 Serial myPort;        // The serial port
 int xPos = 0;         // horizontal position of the graph
 
 int hTemp;
 int hHumid;
 int hLight;
 
 int lineHeight = 2; // hight of each line
 int graphSep; // height of seperation between plots
 
 void setup () {
   size(700, 500); // set the window size  
   
   graphSep = height / 15;
   
   hTemp = (height - lineHeight) / 3;
   hHumid = (2 * (height - lineHeight)) / 3;
   hLight = height - lineHeight;
  
   println(Serial.list()); // List all the available serial ports
   
   myPort = new Serial(this, Serial.list()[0], 115200); // open port
   
   myPort.bufferUntil('\n');  // don't generate a serialEvent() unless you get a newline character
   
   graphTemplate();
 }
 
 void draw () {
   // NOTE: THIS NEEDS TO BE HERE OR THINGS WONT WORK!
 }
 
 void serialEvent (Serial myPort) {
   String inString = myPort.readStringUntil('\n');

   if (inString != null) {
     int rectWidth = 16;
     
     inString = trim(inString); // trim trailing or leading whitespace
  
     float temp = float(inString.split(DELIM) [0]);
     float humid = float(inString.split(DELIM) [1]);
     float light = float(inString.split(DELIM) [2]);
     
     temp = map(temp, 0, 40, 0, (height / 3) - graphSep); // convert to range that will fit on screen
     humid = map(humid, 0, 100, 0, (height / 3) - graphSep); 
     light = map(light, 20, 50, 0, (height / 3) - graphSep);
     
     // draw bars of graphs
     strokeWeight (1);
     strokeCap(SQUARE); // makes square line endings for bars
     
     fill(165, 6, 121); // purple
     rect(xPos, hTemp - temp, rectWidth, temp);
     
     fill(221, 159, 7); // yellow
     rect(xPos, hHumid - humid, rectWidth, humid);
     
     fill(10,103,139); // blue
     rect(xPos, hLight - light, rectWidth, light);
     
     if (xPos >= width) { // at edge of screen, go back to the beginning
       xPos = 0;
       graphTemplate(); 
     } else { // increment horizontal position
       xPos += rectWidth;
     }
   }
 }
 
 void graphTemplate() {
   int dashLength = 15; // length of each dash in dashed line
   int spaceLength = 5; // length of each space in dashed line
   int wordSep = graphSep - 10; // height of seperation between previous line and graph label
   int wordStart = width / 80; // graph label starts - offset from side of display
   background(240);  // set inital background - off white
  
   strokeWeight(1); // thick line
   strokeCap(SQUARE); // makes square line endings for bars
  
   fill(0); // black  
   
   // draw solid lines on graph
   rect(0, hTemp, width, lineHeight); // top
   rect(0, hHumid, width, lineHeight); // middle
   rect(0, hLight, width, lineHeight); // bottom
  
   // draw dashed lines on graph
   for (int i = 0; i < (width - dashLength); i += (dashLength + spaceLength)) {
     rect(i, graphSep, dashLength, lineHeight); // middle dash
     rect(i, hTemp + graphSep, dashLength, lineHeight); // middle dash
     rect(i, hHumid + graphSep, dashLength, lineHeight); // bottom dash
   }
   
   // insert graph labels
   textSize(16); // sets size of text
   fill(165, 6, 121); // purple
   text("Temperature (C)", wordStart, wordSep);
   fill(221, 159, 7); // yellow
   text("Humidity (%)", wordStart, hTemp + wordSep);
   fill(10,103,139); // blue
   text("Light (%)", wordStart, hHumid + wordSep);
   
 }
