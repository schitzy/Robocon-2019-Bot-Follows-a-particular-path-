#define P 1
#define D 90   //d>p
#define M 50
#define B 40
#define wtsp 0
#define lrs B
#define dt 1000
#define lop 10
const byte rx = 19;
const byte tx = 18;
const byte uen = 31;
const byte jp = 33;
const byte rx2 = 17;
const byte tx2 = 16;
const byte uen2 = 37;
const byte jp2 = 35;
const byte d1 = 13; //br
const byte d2 = 11; //bl
const byte d3 = 9;  //fl
const byte d4 = 7;  //fr
const byte p1 = 12;
const byte p2 = 10;
const byte p3 = 8;
const byte p4 = 6;
byte x = 0; 
byte y = 0; 
#define dw(a,b) digitalWrite(a,b)
#define aw(a,b) analogWrite(a,b)
unsigned int junctionCount = 0;
    
void Ff() 
  {dw(d1,1);
  dw(d2,0);
  dw(d3,1);
  dw(d4,0);}

void wait()
 {dw(d1,0);
  dw(d2,1);
  dw(d3,1);
  dw(d4,0);
  aw(p1,wtsp);
  aw(p2,wtsp);
  aw(p3,wtsp);
  aw(p4,wtsp);
    delay(5000);}

void ll()
 {dw(d1,1);
  dw(d2,1);
  dw(d3,0);
  dw(d4,0);
  aw(p1,lrs);
  aw(p2,lrs);
  aw(p3,lrs);
  aw(p4,lrs);
  }

void rr() 
 {dw(d1,0);
  dw(d2,0);
  dw(d3,1);
  dw(d4,1);
  aw(p1,lrs);
  aw(p2,lrs);
  aw(p3,lrs);
  aw(p4,lrs);
  }

void clearJunction() 
  {char address = 0x01;
  char command = 'X';
  char data = 0x00;
  char checksum=address+command+data;
  Serial1.write(address);
  Serial1.write(command);
  Serial1.write(data);
  Serial1.write(checksum);}
    
void setup()
  {pinMode(uen,OUTPUT); 
  pinMode(jp,INPUT); 
  pinMode(uen2,OUTPUT); 
  pinMode(jp2,INPUT); 
  for(byte i=6;i<=13;i++){
  pinMode(i,OUTPUT);}
  dw(uen,1);
  dw(p1,0);
  dw(p2,0);
  dw(p3,0);
  dw(p4,0);
  dw(d1,0);
  dw(d2,0);
  dw(d3,0);
  dw(d4,0);
  Serial1.begin(9600);
  Serial.begin(19200);
  clearJunction();}
int e;
int le=0;
int getJunction() 
  {char address = 0x01;
  char command = 'X';
  char data = 0x01;
  char checksum=address+command+data;
  Serial1.write(address);
  Serial1.write(command);
  Serial1.write(data);
  Serial1.write(checksum);
  while(Serial1.available() <= 0);
  return (int(Serial1.read()));}
    
void tkright(){
  while(x==255){
  dw(uen,0);
  while(Serial1.available() <= 0);
  x = Serial1.read(); 
  dw(uen,1);
  rr();
  if (x>=0&&x<=70) 
  break; 
  delay(lop);}}

void tkleft(){
  while(x==255){
  dw(uen,0);
  while(Serial1.available() <= 0);
  x = Serial1.read(); 
  dw(uen,1);
  ll();
  if (x>=0&&x<=70) 
  break;
  delay(lop);}}
    
void forward() { 
  dw(uen,0);
  while(Serial1.available() <= 0);
  x = Serial1.read(); 
  dw(uen,1);
  while(x>=0&&x<=254){
  dw(uen,0);
  while(Serial1.available() <= 0);
  x = Serial1.read(); 
  dw(uen,1);    
  if(digitalRead(jp)) {
  while(digitalRead(jp))
  {Ff();
  aw(p1,B);
  aw(p2,B);    
  aw(p3,B);
  aw(p4,B);}
  junctionCount = getJunction();}
 
  if(x<=35)
  e = 35 - x;
  else e = x - 35;
  int S = B+(P*e)+D*(e-le);
  le=e;
  if (S>M)
  S=M;
  if (S<0)
  S=0;
  if(x<=30&&x>=0)
  {Ff();
  aw(p1,S);
  aw(p2,2*B-S);    
  aw(p3,2*B-S);
  aw(p4,S);}
  else if(x<=39&&x>=31)
  {Ff();
  aw(p1,B);
  aw(p2,B);    
  aw(p3,B);
  aw(p4,B);}     
  else if(x<=70&&x>=40)   
  {Ff();
  aw(p1,2*B-S);
  aw(p2,S);    
  aw(p3,S);
  aw(p4,2*B-S);}
  Serial.print(x);
  delay(lop);}}

void loop() {
    forward();
    delay(dt);
    tkright();
    forward();
    delay(dt);
    tkleft();
    forward();
    delay(dt);
    tkright();
    forward();
    delay(dt);
    tkleft();
    forward();
    delay(dt);
    wait();
}
