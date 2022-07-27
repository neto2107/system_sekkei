//
// RPM monitor
// Copyright (C) Kenji Ikeda, 2016, 2017
//
import processing.serial.*;

Serial port;

int actionTime = 0;
String mymessage = "";

int count = 0;
int npd=1;
int N = 1000*npd;
int Nchmax = 4;
int Nch = Nchmax;
int xmin = 100;
int xmax = 1100;
int ymin = 20;
int ymax = 620;

int[][] y = new int[Nchmax][N];
int[] offset = {0,0,0,0};
int[] scale = {1000,20,1000,1000};
int[][] chColor = {{0,255,0},{255,255,0},{0,255,255},{255,0,255}};
int[] axisX = {(xmin-10),(xmax+40),(xmin-50),(xmax+90)};
int[] scaleCan = {1,2,5,10,20,50,100,200,500,1000,2000,5000};
int nsc = 11; // size of scaleCan

void settings() {
  size(1200,720);
}

void setup() {
  port = new Serial(this,"/dev/ttyACM0",115200);
  port.clear();
  frameRate(50);
}


void draw() {
  fill(128);
  rect(0,0,1200,720);
 
  textAlign(RIGHT);
  stroke(255);
  strokeWeight(1);
  fill(0);
  rect(xmin-5,ymin-5,(xmax-xmin)+10,(ymax-ymin)+10);
  stroke(128);
  for (int i=0; i<=6; i++)
    line(xmin-5,ymin+i*100,xmax+5,ymin+i*100);
  for (int i=0; i<=10; i++)
    line(xmin+i*100,ymin-5,xmin+i*100,ymax+5);
  for (int j=0; j<Nch; j++) {
    fill(chColor[j][0],chColor[j][1],chColor[j][2]);
    for (int i=0; i<=6; i++)
      text(""+(offset[j]+i*scale[j]),axisX[j],ymax-i*100);
  }
  strokeWeight(3);
  for (int j=0; j<Nch; j++) {
    stroke(chColor[j][0],chColor[j][1],chColor[j][2]);
    for (int i=0; i<N; i++) {
      float yy =(float)(y[j][i]-offset[j]);
      yy *= 100.0/(float)scale[j];
      point(i/npd+xmin,ymax-(int)yy);
    }
  }
  stroke(128,0,0);
  line(xmin+count/npd,ymin,xmin+count/npd,ymax);
  fill(0);
  text("SCALE",xmin-10,ymax+60);
  text("OFFSET",xmin-10,ymax+80);
  strokeWeight(1);
  for (int j=0; j<Nch; j++) {
    stroke(255,255,255);
    fill(0);
    rect(xmin+j*45,ymax+45,40,20);
    rect(xmin+j*45,ymax+65,40,20);
    fill(chColor[j][0],chColor[j][1],chColor[j][2]);
    text(""+scale[j],xmin+40+j*45,ymax+60);
    text(""+offset[j],xmin+40+j*45,ymax+80);
  }
  stroke(255,255,255);
  fill(0);
  text("[sample]",xmax,ymax+40);
  rect(xmax-20,ymax+65,20,20);	// # of channel
  rect(xmax-200,ymax+65,80,20);	// SAVE DATA
  text("# of channels",xmax-20,ymax+80);
  textAlign(CENTER);
  fill(255);
  text(""+Nch,xmax-10,ymax+80);
  text("SAVE DATA",xmax-160,ymax+80);
  fill(0);
  for (int i=0; i<=1000; i+=100)
    text(""+(i*npd),xmin+i, ymax+20);
  textAlign(LEFT);
  fill(255);
  text("RIGHT CLICK to increase",xmin+45*Nch,ymax+60);
  text("LEFT CLICK to decrease",xmin+45*Nch,ymax+80);
  if (millis()-actionTime>2000) {
    mymessage="";
  }
  text(mymessage,600,700);
}


int scaleDown(int scale) {
  int ii = nsc-1;
  for (int i=0; i<nsc; i++) {
    if (scale == scaleCan[i]) {
      ii = i-1;
      if (ii<0)
        ii = 0;
      break;
    }
  }
  return scaleCan[ii];
}

int scaleUp(int scale) {
  int ii = nsc-1;
  for (int i=0; i<nsc; i++) {
    if (scale == scaleCan[i]) {
      ii = i+1;
      if (ii>=nsc)
        ii = nsc-1;
      break;
    }
  }
  return scaleCan[ii];
}

int offsetUp(int scale, int offset) {
  offset /= scale;
  offset *= scale;
  
  offset += scale;
  if (offset>6000)
    offset=6000;
  return offset;
}

int offsetDown(int scale, int offset) {
  offset /= scale;
  offset *= scale;
  
  offset -= scale;
  if (offset<-3*scale)
    offset = -3*scale;   
  return offset;
}

void mouseClicked() {
// SCALE
  if ((mouseY>ymax+45)&&(mouseY<ymax+65)&&(mouseX<xmin+Nch*45)) {
    for (int j=0; j<Nch; j++) {
      if ((mouseX>xmin+j*45)&&(mouseX<xmin+j*45+40)) {
        if (mouseButton==RIGHT) {
          scale[j] = scaleUp(scale[j]);
        } else if (mouseButton==LEFT) {
          scale[j]=scaleDown(scale[j]);
        }
        offset[j] /= scale[j];
        offset[j] *= scale[j];
      }
    }
  }
// OFFSET
  if ((mouseY>ymax+65)&&(mouseY<ymax+85)&&(mouseX<xmin+Nch*45)) {
    for (int j=0; j<Nch; j++) {
      if ((mouseX>xmin+j*45)&&(mouseX<xmin+j*45+40)) {
        if (mouseButton==RIGHT) {
          offset[j] = offsetUp(scale[j],offset[j]);
        } else if (mouseButton==LEFT) {
          offset[j] = offsetDown(scale[j],offset[j]);
        }
      }
    }
  }
// # of Channels
  if ((mouseY>ymax+65)&&(mouseY<ymax+85)&&(mouseX<xmax)&&(mouseX>xmax-20)) {
    if (mouseButton==RIGHT) {
      Nch++;
      if (Nch>Nchmax)
        Nch = Nchmax;
    } else if (mouseButton==LEFT) {
      Nch--;
      if (Nch<1)
        Nch = 1;
    }
  }
// SAVE DATA
  if ((mouseY>ymax+65)&&(mouseY<ymax+85)&&(mouseX<xmax-120)&&(mouseX>xmax-200)) {
    saveData();
  }
}

int ErrCount=0;

void serialEvent(Serial p) {
  int x;
  int ep;
  if (p.available() >= 1+2*Nch) {
    x = p.read();
    if (x==72) {
      ep = ErrCount;
      for (int j=0; j<Nch; j++) {
        x = p.read();
        x <<= 8;
        x |= p.read();
        y[j][count] = x;
        x >>= 8;
        if ((x==72)&&(++ErrCount>10)) {
      	  ErrCount=0;
	        if (--Nch<1) Nch=1;
        }
      }
      count++;
      if (count>=N)
        count=0;
      if (ep==ErrCount)
        if (--ErrCount<0) ErrCount=0;
    }
  }
}

void saveData() {
  int ye = year()%100;
  int mo = month();
  int da = day();
  int s = second();
  int m = minute();
  int h = hour();
  int n;

  String fname = "data"+nf(ye,2)+nf(mo,2)+nf(da,2)+nf(h,2)+nf(m,2)+nf(s,2)+".csv";
  PrintWriter output = createWriter(fname);
  for (int i=0; i<N; i++) {
    n = (count+i)%N;
    String str = ""+i;
    for (int j=0; j<Nch; j++) {
    	str += ","+y[j][n];
    }
    output.println(str);
  }
  output.flush();
  output.close();
  mymessage = "save to "+fname;
  actionTime = millis();
}
