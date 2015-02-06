 import processing.serial.*;

 static String DELIM = " "; // serial input deliminator
 static int lineHeight = 2; // hight of each spacing line in window
 static int rectWidth = 16; // number of pixels wide of each bar in the bar graph
 
 Serial myPort; // The serial port object
 
 // size paramaters, to be set dependent on declared window size
 int hTemp;
 int hHumid;
 int hLight;
 int graphSep; // height of seperation between plots
 
 int xPos = 0;         // starting horizontal position of the graph
 
 void setup () {
   size(700, 500); // set the window size  
   
   graphSep = height / 15; // distance between dashed and dotted lines in window
   
   // the following help break the window into three parts, one for each graph
   // I subtracted lineHeight to make sure there would be room in each section for the black base line
   hTemp = (height - lineHeight) / 3;
   hHumid = (2 * (height - lineHeight)) / 3;
   hLight = height - lineHeight;
  
   println(Serial.list()); // List available serial ports in monitor for reference
   
   myPort = new Serial(this, Serial.list()[0], 115200); // open serial port - tested laptop only uses one
   
   myPort.bufferUntil('\n');  // don't generate a serialEvent() unless you get a newline character   
   graphTemplate();
 }
 
 void draw () {
   // NOTE: THIS NEEDS TO BE HERE OR THINGS WONT WORK!
 }
 
 // serial events are called when new data is available
 // in this case it is detected by bufferUntil() in setup()
 void serialEvent (Serial myPort) {
   String inString = myPort.readStringUntil('\n'); // reads line from serial monitor
   
   inString = trim(inString); // trim trailing or leading whitespace

   float temp = float(inString.split(DELIM) [0]); // splits string into workable values
   float humid = float(inString.split(DELIM) [1]);
   float light = float(inString.split(DELIM) [2]);
     
   // converts each float to a range that will fit in the specified window
   // i.e. our humidity min - max is 0 - 100%,
   // however a relative 100% in the window is (height / 3) - graphSep
   temp = map(temp, 0, 40, 0, (height / 3) - graphSep);
   humid = map(humid, 0, 100, 0, (height / 3) - graphSep); 
   light = map(light, 20, 50, 0, (height / 3) - graphSep);
   
   // draw bars on graphs of data
   strokeWeight (1); // weight of black border around each bar
   strokeCap(SQUARE); // makes square line endings for bars
   
   fill(165, 6, 121); // set color purple
   rect(xPos, hTemp - temp, rectWidth, temp); // draw rectangle
   
   fill(221, 159, 7); // set color yellow
   rect(xPos, hHumid - humid, rectWidth, humid);
   
   fill(10,103,139); // set color blue
   rect(xPos, hLight - light, rectWidth, light);
   
   if (xPos >= width) { // at edge of screen, go back to the beginning
     xPos = 0;
     graphTemplate(); // reset window
   
 } else { // increment horizontal position for next reading
     xPos += rectWidth;
   }
 }
 
 void graphTemplate() {
   int dashLength = 15; // length of each dash in dashed line
   int spaceLength = 5; // length of each space in dashed line
   int wordSep = graphSep - 10; // height of seperation between previous line and graph label
   int wordStart = width / 80; // graph label starts - offset from side of display
   
   background(240);  // set inital background - off white
  
   strokeWeight(1);
   strokeCap(SQUARE);
  
   fill(0); // set color black  
   
   // draw solid base lines on graph
   rect(0, hTemp, width, lineHeight); // top
   rect(0, hHumid, width, lineHeight); // middle
   rect(0, hLight, width, lineHeight); // bottom
  
   // draw dashed lines on graph
   for (int i = 0; i < (width - dashLength); i += (dashLength + spaceLength)) {
     // TODO there should only be two here - test with board?
     rect(i, graphSep, dashLength, lineHeight); // middle dash
     rect(i, hTemp + graphSep, dashLength, lineHeight); // middle dash
     rect(i, hHumid + graphSep, dashLength, lineHeight); // bottom dash
   }
   
   // insert graph labels
   textSize(16); // sets size of text
   
   fill(165, 6, 121); // set color purple
   text("Temperature (C)", wordStart, wordSep);
   
   fill(221, 159, 7); // set coloryellow
   text("Humidity (%)", wordStart, hTemp + wordSep);
   
   fill(10,103,139); // set color blue
   text("Light (%)", wordStart, hHumid + wordSep); 
 }
