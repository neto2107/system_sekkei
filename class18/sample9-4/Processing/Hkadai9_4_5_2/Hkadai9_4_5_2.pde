import processing.serial.*;
Serial port;

int ax = 0, ay = 0, az = 0;
int mx = 0, my = 0, mz = 0;
int CX=250, CY=250;
int mode_G;

void settings() {

}

void setup() {
    size(1000,500,P3D);
   port = new Serial(this, "/dev/tty.usbserial-A400FRGN",9600);  // COM3 は各自の環境に合わせて修正せよ
   port.clear();
   
}

void drawBox(){
  background(0);
  translate(width/2,height/2);
  
  //ラジアン核で指定、Y軸に対して60ど回転
  rotateY(PI/3);
  box(150,150,150);
}


void draw() {
  drawBox();

}
int read1byteint(Serial p) {
  int x = p.read();
  x -= 128;
  return x;
}

int read2byte(Serial p) {
  int x = p.read();  x <<= 8;   x |=p.read();
  if (x>32767) x -= 65536;
  return x;
}

void serialEvent(Serial p) {
  if (p.available() >=8 ) {
    if (p.read() == 'H') {
      ax = read1byteint(p);  ay = read1byteint(p);  az = read1byteint(p);
      mx = read1byteint(p);  my = read1byteint(p);  mz = read1byteint(p);
      mode_G = p.read();  //read1byteunsigned;
//      p.clear();
    }
  }
}
