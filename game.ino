#define led_clock 11
#define led_read 12
#define led_data 13
#define led_output 4

#define col_1 10
#define col_2 9
#define col_3 8
#define col_4 7

#define encod_0 6
#define encod_1 5

// button columns
int cols[4] = {10,9,8,7};
int k = 0;

// leds
byte bytes_leds[16] = {142, 141, 139, 135,
                        78, 77, 75, 71,
                        46, 45, 43, 39,
                        30, 29, 27, 23};
int randNum;

// array of randoms

#define MAX 10
byte combinacion[MAX];
int combs = 0;
bool start_play = false;

//interrupts

volatile byte button = 0;
volatile bool q0; 
volatile bool q1;  
volatile int turns = 0; 
volatile bool continuar = true;
volatile bool winning = false; 


//losing

byte a,b;


// INIT
void setup(){
  
  pinMode(led_clock,OUTPUT);
  pinMode(led_read,OUTPUT);
  pinMode(led_data,OUTPUT);
  pinMode(led_output,OUTPUT); digitalWrite(led_output, HIGH);
  
  pinMode(col_1,OUTPUT);
  pinMode(col_2,OUTPUT);
  pinMode(col_3,OUTPUT);
  pinMode(col_4,OUTPUT);
  
  //pinMode(encod_0,INPUT);
  //pinMode(encod_1,INPUT);

  attachInterrupt(0, buttonToLed, RISING);

  Serial.begin(9600);

  randomSeed(analogRead(0));
  }


void loop(){
  if(continuar){
   if(!start_play){
    combinacion_init();
    }
    else{ 
      digitalWrite(cols[k],HIGH);
      //delay(50); // usefull?
      digitalWrite(cols[k],LOW);
      k++;
      if( k == 4){k = 0;}
      }
    }else{
      if(!winning){losing();} 
      else{won();} 
      }
  }

// INTERRUPT
void buttonToLed(){
    q0 = digitalRead(encod_0);
    q1 = digitalRead(encod_1);
    button = 15 & ~(1 << (3 - k)); 
    if(!q0 && !q1) {button = button + 128;} 
    if(q0 && !q1) {button = button + 64;}
    if(!q0 && q1) {button = button + 32;}
    if(q0 && q1) {button = button + 16;}
    /*Serial.println("\n");
    Serial.println(button);
    Serial.println(combinacion[turns]);     */
    if( button == combinacion[turns] ) {
      turns++;
      }else{
        continuar = false; Serial.println("\nYOU LOSE");Serial.println("\n");randomSeed(millis()); combs=0; turns=0;}
      
    if(turns == combs){
        start_play = false; turns = 0;
        }
    if(turns == MAX){
      continuar = false; winning = true;
      }

    while(digitalRead(2)){} // hmed ur a genious
 }


// combiations
// randomly fills the array of leds to light up
void combinacion_init(){
  
  pinMode(encod_0,OUTPUT);
  pinMode(encod_1,OUTPUT);
  
  Serial.println(combs + 1);
  
  randNum = random(0,16);
  combinacion[combs] = bytes_leds[randNum];
  
  for(int j = 0; j <= combs; j++){
      shiftOut(led_data, led_clock, LSBFIRST, combinacion[j]);
      delay(250);
      digitalWrite(led_read, HIGH);
      digitalWrite(led_output, LOW);
      delay(250);
      digitalWrite(led_output, HIGH);
      digitalWrite(led_read, LOW);
    }
  combs++;  
  start_play = true;
  
  pinMode(encod_0,INPUT);
  pinMode(encod_1,INPUT);
  }


// losing

void losing(){
  digitalWrite(led_output, LOW);
  
  pinMode(encod_0,OUTPUT);
  pinMode(encod_1,OUTPUT);
  
  for (a=0;a<4;a++){
    for (b=0;b<4;b++){
      digitalWrite(led_read, LOW);
      shiftOut(led_data, led_clock, LSBFIRST, (1 << (a+4)) | (15 & ~(1 << b)));
      digitalWrite(led_read, HIGH);
      delay(200);
    }
   }

   digitalWrite(led_output, HIGH);
   digitalWrite(led_read, LOW);
   
   continuar = true; start_play = false;
  }

//winning

void won(){
  while(true){Serial.println("you win");}}
