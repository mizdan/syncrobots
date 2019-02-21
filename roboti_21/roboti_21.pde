import processing.net.*;

Client c,c1;
String data;
int x,y,dir;
boolean esc = true;

int[][] puncte= {{-342,1611}, {10,0}, {30,30}, {40,30}, {30,20}};

void setup() {
  x=y=dir=0;
  c = new Client(this, "192.168.1.83", 23); 
  c1 = new Client(this, "192.168.1.83", 24); 
  
 size(1000, 500);
  background(0);
    
}



void draw() {
  background(0);

  
  translate(x/3+700,y/4);
  rotate(radians(dir/6));
   println("robot:" + (float)(dir/6+180));
   println("punct:" + getAngle(y,puncte[0][1],x,puncte[0][0]));
   
   float spre = (getAngle(puncte[0][1],y,puncte[0][0],x) + 90) % 360;
   println("dist:" + dist(x,y,puncte[0][0],puncte[0][1]));
   if (dist(x,y,puncte[0][0],puncte[0][1]) < 30 || esc == true) {
          c1.write("iw 0 4 0\r\n");
         c1.write("iw 0 12 0\r\n");
   } else
   
   //if (abs((float)(dir/6 - spre) >?< 180 ))
   if (spre < (float)(dir/6+180)) {
     c1.write("iw 0 4 110\r\n");
     c1.write("iw 0 12 200\r\n");
   } else {
          c1.write("iw 0 4 200\r\n");
     c1.write("iw 0 12 110\r\n");

   }
   
  rect(-10,-20,20,40);
  ellipse(0,20,20,20);
}


float getAngle(int y12, int y11, int x12,int x11) {
  
  float m1 = (float)(abs(y12 - y11)) / (float)(abs(x12 - x11));  
  float A = atan(m1) * 180 / PI;

  return A;
  
}

void clientEvent(Client someClient) {
  
  //println("xxxxx");
  while (someClient.available() > 35) {
  
     data = someClient.readStringUntil(10); // ...then grab it and print it
    println(data);
    
     data = data.replaceAll("\\n", "");
        data = data.replaceAll("\\r", "");
    if (data.indexOf("X") == 0) {
               
        x = Integer.valueOf(data.substring(1));
      }

      if (data.indexOf("Y") == 0) {
        //println(lines[i].substring(1));
        y = Integer.valueOf(data.substring(1));
      }


      if (data.indexOf("O") == 0) {
        dir = Integer.valueOf(data.substring(1));
      }
  }
}



void keyPressed() {
  println("xxx");
  
  if (key=='q' || key == 'Q') {
    println("ESC!!");
    esc = !esc;
    c1.write("iw 0 4 0\r\n");
    c1.write("iw 0 12 0\r\n");
  }
  
  
   if (key=='w' || key == 'W') {
  c.stop();
  }
  
  
  
  if (key=='a' || key == 'A') {
    println("START!!");
    c1.write("iw 0 4 150\r\n");
  }
  
    if (key=='z' || key == 'Z') {
      println("STOP!!");
    c1.write("iw 0 4 0\r\n");
  }
  
    if (key=='k' || key == 'K') {
    println("START!!");
    c1.write("iw 0 12 150\r\n");
  }
  
    if (key=='m' || key == 'M') {
      println("STOP!!");
    c1.write("iw 0 12 0\r\n");
  }
  
}
