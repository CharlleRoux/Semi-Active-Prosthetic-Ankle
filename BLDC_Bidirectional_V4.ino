#include <Servo.h>
#include <Wire.h> 
#include <string.h>

Servo ESC;//define a new servo object
#define MPU6050 0x68

//define I/O Pins
#define ESC_PIN 9//data transmission pin
#define hallpin 2//hall effect sensor on pin 2
#define limitswitch 7//limit switch on pin 7

#define max_steps 30//define ankle range of motion

//Math variables
volatile int steps = 0;
const int window = 10;

float avgErrorX, maxErrorX; 
float GyroX, prev_x = 0, avg_x;
float sum = 0;
float arr[window];
int index = 0;
int count = 0;

float throttle = 90;
float temp = 90;

bool step_limit = false;
bool min_step = true;
bool start = false;

//functions
void IMU_Error();//program to calculate average and maximum error in MPU6050
void Sensor_Read_Filter();//program to measure speed and filter output
int Speed_Modifier();//Program to modify the the speed using constant acceleration
void Step_Limit();//Routine for when step limit has been reached
void Limit_Switch_Trip();//Routine for when limit switch has been tripped

void step_counter(){
  if(temp > 90){
    steps ++;
  }

  else if(temp < 90){
    steps --;
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU6050);
  //Set up power management register of MPU6050
  Wire.write(0x6B);
  Wire.write(0x00);
  //Set sensor resolution
  Wire.write(0x1B);
  Wire.write(0x11);  
  Wire.endTransmission(true);

  delay(20);
  IMU_Error(); //Calculate average and Maximum error

  //array initialization
  for(int i = 0; i < window; i ++){
    arr[i] = 0;    
  }

  // BLDC ESC set up
  ESC.attach(ESC_PIN, 1000, 2000);//Set up ESC communication on pin 9
  attachInterrupt(digitalPinToInterrupt(hallpin), step_counter, FALLING);//set up interrupt to count steps
}

void loop() {
  Sensor_Read_Filter();
  throttle = avg_x/2000.0*45.0 + 90.0;

  if(digitalRead(limitswitch) == LOW){ //&& throttle < 90){//Routine for limit switch
    Limit_Switch_Trip();
    min_step = true;
    start = true;
  }

  else if(steps >= max_steps){//Routine for step limit
    Step_Limit();
    step_limit = true;
  }

  else if(steps > max_steps - 6 && throttle > 90){//Set danger flag when close to step limit
    step_limit = false;
  }

  else if(steps < 6 && digitalRead(limitswitch) == HIGH){//Set danger flag when close to limit switch
    min_step = false;
  } 
  
  throttle = Speed_Modifier();
  temp = throttle;
  
  String str = "Throttle : " + String(temp) + " Steps : " + String(steps);
  Serial.println(str);
  ESC.write(temp);
  
  prev_x = GyroX;
}
