/***
 *     __ __                  
 *    (_ |_  _  __ __ _  \/ __
 *    __)|  (/_ |  | (/_ /  | 
 *    __  _ __ __             
 *     _)/ \ _) _)            
 *    /__\_//__/__            
 *    https://github.com/sferreyr
 */


 
boolean servo_posicionado = false;
boolean proceso_finalizado = false;

void posicionarServo() {
  servo_brazo.attach(servo_pin);  // attaches the servo on pin 24 to the servo object
  servo_brazo.write(90);  // set servo to mid-point
  servo_pos = 90;
  for (servo_pos = 90; servo_pos <= 175; servo_pos++) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    //   Serial.println(servo_pos);
    servo_brazo.write(servo_pos);              // tell servo to go to position in variable 'pos'
    delay(25); //40                       // waits 15ms for the servo to reach the position

    if (servo_pos == 175) {
      servo_posicionado = true;
      Serial.println("SERVO LLEGO A 175");
      controlTemperatura();
      break;

    }
  }



}
void restablecer_brazoServo() {
  servo_pos = 175;
  for (servo_pos = 175; servo_pos >= 90; servo_pos--) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    //  Serial.println(servo_pos);
    servo_brazo.write(servo_pos);              // tell servo to go to position in variable 'pos'
    delay(25); //40                       // waits 15ms for the servo to reach the position

    if (servo_pos == 90) {
      servo_posicionado = false;
      Serial.println("SERVO LLEGO A 90");
      servo_brazo.detach();

      break;

    }
  }
}
