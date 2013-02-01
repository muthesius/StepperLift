import processing.serial.*;

String info = "// Press the following @ own risk;"+"\n"+
"// s: one turn up"+"\n"+
"// d: one turn down"+"\n"+
"// 1-4: stepsize 1,2,4,8,16"+"\n"+
"// +: upwards permanent"+"\n"+
"// -: downwards permanent"+"\n"+
"// #: halt";

Serial port;

void setup(){
  size(640,320);
  smooth();
  textFont(createFont("Verdana", 16,true));
  println(Serial.list());
  String portName = Serial.list()[4]; /// <--- Port anpassen!
  port = new Serial(this, portName, 9600);
}

void draw() {
  background(10);
  text(info, 20, 20);
}

void keyPressed() {
  println(key);
  port.write(key);
}
