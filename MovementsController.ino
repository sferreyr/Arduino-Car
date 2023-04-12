/***
 *     __ __                  
 *    (_ |_  _  __ __ _  \/ __
 *    __)|  (/_ |  | (/_ /  | 
 *    __  _ __ __             
 *     _)/ \ _) _)            
 *    /__\_//__/__            
 *    https://github.com/sferreyr
 */


 /*
  _FRENTE CHASIS___
   |1L        |3R
   |          |
   |          |
   |2L______ |4RB
*/

void avanzar() {
  motor1L.setSpeed(250);
  motor2LB.setSpeed(250);
  motor3R.setSpeed(250);
  motor4RB.setSpeed(250);

  motor1L.run(FORWARD);//
  motor2LB.run(FORWARD); 
  motor3R.run(BACKWARD); // ESTE MOTOR SE ENCUENTRA CON GIRO INVERSO
  motor4RB.run(FORWARD);//

  //Mensaje en lcd
  lcd.clear();
  lcd.setCursor(0, 0); // Primer renglon
  lcd.print("Avanzando");
  lcd.setCursor(0, 1); // segundo renglon
  lcd.print("   :)");
}

void retroceder() {
  motor1L.setSpeed(250);
  motor2LB.setSpeed(250);
  motor3R.setSpeed(250);
  motor4RB.setSpeed(250);

  motor1L.run(BACKWARD);//
  motor2LB.run(FORWARD); // ESTE MOTOR SE ENCUENTRA CON GIRO INVERSO
  motor3R.run(BACKWARD); //
  motor4RB.run(BACKWARD);//

  

  //Mensaje en lcd
  lcd.clear();
  lcd.setCursor(0, 0); // Primer renglon
  lcd.print("Volviendo..");
  lcd.setCursor(0, 1); // segundo renglon
  lcd.print("   :| ");
}

void girar_izquierda() {
  motor1L.setSpeed(255);
  motor2LB.setSpeed(255);
  motor3R.setSpeed(255);
  motor4RB.setSpeed(255);

  //Hacemos GIRO
  motor1L.run(BACKWARD);//
  motor2LB.run(BACKWARD); 
  motor3R.run(BACKWARD); // ESTE MOTOR SE ENCUENTRA CON GIRO INVERSO
  motor4RB.run(FORWARD);//
  
}

void girar_derecha() {
  motor1L.setSpeed(255);
  motor2LB.setSpeed(255);
  motor3R.setSpeed(255);
  motor4RB.setSpeed(255);
  //Hacemos GIRO
  motor1L.run(FORWARD);//
  motor2LB.run(FORWARD); 
  motor3R.run(FORWARD); // ESTE MOTOR SE ENCUENTRA CON GIRO INVERSO
  motor4RB.run(BACKWARD);//
  
}

void detener_motores() {
  motor1L.run(RELEASE);
  motor2LB.run(RELEASE);
  motor3R.run(RELEASE);
  motor4RB.run(RELEASE);
  avanzando = false;
}
