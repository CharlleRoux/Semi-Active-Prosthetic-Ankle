// Read sensor value and filter output

void Sensor_Read_Filter(){
  Wire.beginTransmission(MPU6050);
  Wire.write(0x43);//Acces Gyroscope Registers
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050, 2, true);

  GyroX = ((Wire.read() << 8 | Wire.read()) / 16.4) - avgErrorX;//X-axis gyroscope sensor value is read from the MPU6050

  if(abs(GyroX) < 1.5*abs(maxErrorX)){
    GyroX = 0;
  }

  else if(abs(GyroX - prev_x) < abs(avgErrorX)){
    GyroX = prev_x;
  }

  //Implement Moving Average Filter
  index ++;
  if(index > window - 1){
    index = 0;
  }
  arr[index] = GyroX;
  //sum = sum + arr[index];
  sum = 0;
  for(int i = 0; i < window; i ++){
    sum = sum + arr[i];
  }  
  
  avg_x = sum/window;
}