//Calculate Error for MPU6050

void IMU_Error(){
  float err_x = 0;
  int n = 1000;
  float sum = 0;
  
  for(int i = 0; i < n; i ++){
    Wire.beginTransmission(MPU6050);
    Wire.write(0x43);//Acces Gyroscope Registers
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050, 2, true);

    //Request Gyroscop data
    GyroX = (Wire.read() << 8 | Wire.read()) / 16.4; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
    
    //String str = "X : " + String(GyroX) + " Y : " + String(GyroY) + " Z : " + String(GyroZ);
    //Serial.println(str);
    
    if(abs(GyroX) > abs(err_x)){
      err_x = GyroX; 
      //Serial.println(err_x);     
    }    

    //delay(20);
    sum += GyroX;
  } 
  
  avgErrorX = sum/n;  
  maxErrorX = err_x;
}
