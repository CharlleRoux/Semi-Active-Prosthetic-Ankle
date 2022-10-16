// Routine For when limit switch is activated

void Limit_Switch_Trip(){
  while(digitalRead(limitswitch) == LOW && throttle <= 90){
    ESC.write(90);
    Sensor_Read_Filter();
    throttle = avg_x/250.0*90.0 + 90.0;

    String str = "Limit Reached, Throttle : " + String(throttle);
    Serial.println(str);
  }

  steps = 0;
}