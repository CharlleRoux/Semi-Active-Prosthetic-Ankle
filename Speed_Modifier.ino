//function to modify speed due to acceleration and speed

int Speed_Modifier(){
  const int acceleration = 20;

  if(abs(throttle - 90) < 5){
    throttle = 90;
  }

  if(start == false){//Startup Routine for when limit switch has not been found
    if(count <= 3000){
      throttle = 90;      
    }

    else if(count > 3000){
      throttle = 80;
    }

    count ++;
    Serial.println(count);
  }

  //if close to step limit decrease motor speed
  if((steps > max_steps - 6) && throttle - acceleration*(steps - (max_steps - 6)) > 95 && step_limit == false && start == true){
    throttle = throttle - acceleration*(steps - (max_steps - 6));
  }

  else if((steps > max_steps - 6) && throttle - acceleration*(steps - (max_steps - 6) < 95) && step_limit == false && start == true){
    throttle = 100;
  }

  //if close to limit switch decrease motor speed
  if((steps < 6 + 1) && throttle + acceleration*(6 - steps) < 85 && min_step == false && start == true){
    throttle = throttle + acceleration*(6 - steps);
    if(steps >= 30){
      throttle = 90;
    }
  }

  else if((steps < 6 + 1) && throttle + acceleration*(6 - steps) > 85 && min_step == false && throttle < 90 && start == true){
    throttle = 80;
    if(steps <= 0){
      throttle = 90;
    }
  }

  return throttle;
}