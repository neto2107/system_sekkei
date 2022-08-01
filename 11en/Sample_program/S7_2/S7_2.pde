/* For use with the colorview Arduino example sketch 
   Update the Serial() new call to match your serial port
   e.g. COM4, /dev/usbserial, etc!
*/

import processing.serial.*;
import java.awt.datatransfer.*;
import java.awt.Toolkit;

Serial port;
 
void setup(){
 size(640,480);
 // remember to replace COM20 with the appropriate serial port on your computer ex Serial.list()[1]
 port = new Serial(this, "COM5", 9600); 
 port.clear();
}
 
String buff = "";

int wRed, wGreen, wBlue, wClear;
String hexColor = "ffffff";
 

void draw(){
 //background(wRed,wGreen,wBlue);
 // check for serial, and process
 background(0);
  fill(wRed,0,0); rect(10,470,100,-wRed);
  fill(0,wGreen,0); rect(120,470,100,-wGreen);
  fill(0,0,wBlue); rect(230,470,100,-wBlue);
  fill(wRed, wGreen, wBlue); rect(360,110,260,260);
  //print("R="+wRed+", "); print("G="+wGreen+", "); println("B="+wBlue);
  fill(255);
  text("Red=",10,10); text(int(wRed),40,10);
  text("Green=",120,10); text(int(wGreen),160,10);
  text("Blue=",230,10); text(int(wBlue),265,10);

 while (port.available() > 0) {
   serialEvent(port.read());
 }
}
 
void serialEvent(int serial) {
 if(serial != '\n') {
   buff += char(serial);
 } else {
   int cRed = buff.indexOf("R");
   int cGreen = buff.indexOf("G");
   int cBlue = buff.indexOf("B");
   //int clear = buff.indexOf("C");
      
   //if(clear >=0){
   //  String val = buff.substring(clear+3);
   //  val = val.split("\t")[0];
   //  wClear = Integer.parseInt(val.trim());
   //} else { return; }
   
   if(cRed >=0){
     String val = buff.substring(cRed+3);
     val = val.split("\t")[0]; 
     wRed = Integer.parseInt(val.trim());
   } else { return; }
   
   if(cGreen >=0) {
     String val = buff.substring(cGreen+3);
     val = val.split("\t")[0]; 
     wGreen = Integer.parseInt(val.trim());
   } else { return; }
   
   if(cBlue >=0) {
     String val = buff.substring(cBlue+3);
     val = val.split("\t")[0]; 
     wBlue = Integer.parseInt(val.trim());
   } else { return; }
   
   //wRed *= 255; wRed /= wClear;
   //wGreen *= 255; wGreen /= wClear; 
   //wBlue *= 255; wBlue /= wClear; 

   print("Red: "); print(wRed);
   print("\tGrn: "); print(wGreen);
   print("\tBlue: "); print(wBlue);
   //print("\tClr: "); println(wClear);

   //hexColor = hex(color(wRed, wGreen, wBlue), 6);
   //print("\t HEX: ");
   //print(hexColor);
   println();
   buff = "";
 }
}
