/* HAL 96000x Astate nullzone 75
 RC Tracked Vehicle with Robotic arm 
 Controller
 
 Stuart Oldfield
 
 arm maybe working , calibration needs sorting
 basic track control working
 sorted pwm pins 
 
 need powermap for motors as they run at different speeds
 added motor bias to final drive
  
 Using Arduino Pro Mini 
 
 // Stu's Pins
 D0
 D1
 D2 - rc1
 D3 - EN Track 0
 D4 - rc2
 D5 - EN track 1
 D6 - EN Arm 0
 D7 - rc 3
 D8 - RC Chan 4 left stick
 D9 - EN Arm 1
 D10 - EN arm 2
 D11 - EN arm 3
 D12 - Dir Track 0
 D13 - Dir Track 1
 A0 - D Arm 0
 A1 - D Arm 2
 A3 - D Arm 3
 A3 - D Arm 3
 A4 - Gyro
 A5 - Gyro
 A6 - Free (analog in only)
 A7 - Free       "
 
 */
// Channel Pin Array

#define ARMED 1
#define MMINPWM 150 // minimum pwm value before motor engaged
#define RC_CHANS 5 // (actual +1)
#define RC_TRACK_STEER 0 // incoming RC drives tracks = 1 normal steer = 0


unsigned long time;  // time since start in millis
unsigned long last_time;  // time since last interval
unsigned long report_time= 2000;  // time between reports

int state=0; // initial state to init 

int rc_pin[] = {0,2,4,7,8,0,0};
int dir_pin[] = {0,12,13,A0,A2,A1,A3};
int pwm_pin[] = {0,3,5,6,9,10,11};

int track0bias=0;
int track1bias=0;
// Channel Value Array
int rc_val[RC_CHANS];
// 1 = Speed
// 2 = Direction
// 3 = arm x
// 4 = arm y
int velocity; // Forward/Back speed
int turn; // Turning Factor
int vel_T0; // Velocity Track 0
int vel_T1; // Velocity Track 1
// set calibration for servo inputs
int rc_max[]={1700,1900,1900,1700,1700,1700};
int rc_min[]={1200,1100,1100,1200,1200,1200};


void setpins(){
	// setup pins
	// set channels to input
  for(int i=1;i<7;i++)
  {
    if(i<=RC_CHANS){
      pinMode(rc_pin[i],INPUT); // set pin to input
    }
    pinMode(pwm_pin[i],OUTPUT); // pwm pins are outputs
    analogWrite(pwm_pin[i],0);  // set pin to zero
	pinMode(dir_pin[i],OUTPUT); // direction pins to output
	digitalWrite(dir_pin[i],1); // set direction to forward
  }
}

void setup() {
	state=2;  // state 2 = setup
	setpins();

	Serial.begin(9600); 
	Serial.println("Hello");  
}


void readrc(){
        int t;
	for(int i=1;i<=RC_CHANS;i++){
		t=pulseIn(rc_pin[i],HIGH,25000); // Read a pulse from channel	
        rc_val[i]=t;
        // adjust calibration
        if(t>=rc_max[i])  {
			rc_max[i]=t;
			if(state==2) state=3;
		}
        if(t>=10){
          if(t<=rc_min[i])  rc_min[i]=t;
		}
    }
}

void movearm(){
  
  // left stick is arm
  // calculate arm x&y positions
  
int armx = map(rc_val[3], rc_min[3],rc_max[3], 1, 6); // map x to 5 values
// velocity = constrain(velocity, -255, 255); //only pass values whose absolutes are valid pwm values

int army = map(rc_val[4], rc_min[4],rc_max[4], 1, 6); // map y to 5 values
  int m =(army*10)+armx;
// zero any current arm movement
for(int i=3;i<=7;i++) digitalWrite(pwm_pin[i],0);
// Serial.println("Arm Vector = :"+ String(m));
// pin[3]=arm0
// pin[4]=arm1
// pin[5]=arm2
// pin[6]=arm3
  if(ARMED ==1){
      
    switch(m){
       case 11:
    // bottom left = A3- pin 6
      digitalWrite(dir_pin[6],0);
      analogWrite(pwm_pin[6],128);
          
      break;
      
      case 13: // bottom middle = A1- pin 4
    
      digitalWrite(dir_pin[4],0);
      analogWrite(pwm_pin[4],128);
      break;
      
      case 15:  // bottom left A2- pin 5
      digitalWrite(dir_pin[5],0);
      analogWrite(pwm_pin[5],128);    
    
      break;
      case 31: // left - A0- pin 3
      digitalWrite(dir_pin[3],0);
      analogWrite(pwm_pin[3],128);
      break;
      
      case 35: // right A0+  pin 3
      digitalWrite(dir_pin[3],1);
      analogWrite(pwm_pin[3],128);
      break;
      
      case 51: // A2+ pin 5
      digitalWrite(dir_pin[5],1);
      analogWrite(pwm_pin[5],128);
      break;
      
      case 53: // A1+ pin 4
      digitalWrite(dir_pin[4],1);
      analogWrite(pwm_pin[4],128);
      break;
      
      case 55: // top right A3+ pin 6
      digitalWrite(dir_pin[6],1);
      analogWrite(pwm_pin[6],128);
      break;
    
     default:
     
     break;
       
    }
  }
}
void loop() {
  time=millis(); // get current time
  readrc();  // get the RC values
  moveme();   // move the rover
 // lightcheck();
  movearm();
  report();  // print the diags
  // delay(100);
}

void report(){
 // check to see if we're due yet
 if(time-last_time>report_time){
	 last_time=time;
 
	  Serial.print("State:"+String(state)+" Armed="+String(ARMED)+" ");
	  if(rc_val[1]>=1000){Serial.println("Left Switch: Engaged");}
	  if(rc_val[1]<=1000){Serial.println("Left Switch: Disengaged");}

		Serial.println("Right Stick X:" + String(rc_val[1]));
		Serial.println("Right Stick Y:" + String(rc_val[2]));
		Serial.println("**********************");
		Serial.println("T0 Velocity:" + String(vel_T0));
		Serial.println("T1 Velocity:" + String(vel_T1));
		Serial.println("**********************");

		Serial.println("RC Chans:");
		for(int i=1;i<RC_CHANS;i++){
			Serial.println(" C:"+ String( i) + " V: " + rc_val[i] );
		}
		Serial.println();
/*
		for(int i=1;i<7;i++){
		Serial.print("Motor:"+ String( i) + " Dir Pin: " + dir_pin[i] );
			Serial.println(" PWM Pin:"+ String( i) + " Value: " + pwm_pin[i] );
			
		}
*/	
	for(int i=1; i<RC_CHANS ; i++){
				Serial.print("RC :"+ String( i) + " Max: " + String(rc_max[i] ));
			  Serial.println(" Min: " + String(rc_min[i] ));
			}
		Serial.println();

		// delay(200);
	}

}

void lightcheck(){
	int i=0;
	int l=0;
	for(l=0;l<4;l++){
	
		for(i=1;i<7;i++){  
			// set dir to zero
			digitalWrite(dir_pin[i],1);
			// set enables to pwm128
			digitalWrite(pwm_pin[i],128);
		}
	
		delay(150);

		for(i=1;i<7;i++){
	
			// set dir to 1
			digitalWrite(dir_pin[i],0);
		}
		delay(150);
		for(i=1;i<7;i++){
		digitalWrite(pwm_pin[i],0);
		}
		
	}
}

void moveme()
{

		// tell the routine where the tracks are
		int dir_a=dir_pin[1]; 
		int dir_b=dir_pin[2];
		int pwm_a=pwm_pin[1];
		int pwm_b=pwm_pin[2];
		int v; // used to store absolute velocity
		
		// get the track velocities
		if(RC_TRACK_STEER==1){
		vel_T0 = map(rc_val[1], rc_min[1],rc_max[1], -255, 255); // map to +- pwm
		vel_T0 = constrain(vel_T0, -255, 255); // trim to valid pwm values

		vel_T1 = map(rc_val[2], rc_min[2],rc_max[2], -255, 255); // map to +- pwm
		vel_T1 = constrain(vel_T1, -255, 255); // trim to valid pwm values
		}

		if(RC_TRACK_STEER==0){
			turn=map(rc_val[1],rc_min[1],rc_max[1],-255,255);
			velocity=map(rc_val[2],rc_min[2],rc_max[2],-255,255);
			// add the turn and the bias to the motor speeds
			vel_T0=constrain(velocity+turn,-255,255);
			vel_T1=constrain(velocity-turn,-255,255);
			
		}
		
		
	  // check to see if we're armed
	  if((ARMED==1)&&(state>=3)){
		  
		  
		  /* Work out if motors are running forward or backward*/
		  if(vel_T0>0){
			  digitalWrite(dir_a, 1);
			} 
		  else {
			  digitalWrite(dir_a, 0);
			}
			  
			if(vel_T1>0){
				digitalWrite(dir_b, 1);
			}
			else {
				digitalWrite(dir_b, 0); 
			};
		}
	  
		v=constrain(abs(vel_T0)+track0bias);
		// introduce a null zone
		if(v>=MMINPWM){
			analogWrite(pwm_a, v);
		}
		else
		{
			analogWrite(pwm_a,0);
		}
			
		v=constrain(abs(vel_T1)+track1bias,0,255);
		if(v>=MMINPWM){
		  analogWrite(pwm_b, v);
		}
		else
		{
		   analogWrite(pwm_b,0);
		}
			  
	}	

