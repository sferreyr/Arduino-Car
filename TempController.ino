/***
       __ __
      (_ |_  _  __ __ _  \/ __
      __)|  (/_ |  | (/_ /  |
      __  _ __ __
       _)/ \ _) _)
      /__\_//__/__
      https://github.com/sferreyr
*/


void controlTemperatura() {

  digitalWrite(laser_pin, HIGH); // Laser ON

  //Tomamos la medida de temperatura del michell.
  Serial.print("Ambiente = ");
  Serial.print(mlx.readAmbientTempC() );
  Serial.print("ºC\tObjeto REAL = ");
  Serial.print(mlx.readObjectTempC() );
  Serial.print("ºC\tObjeto -20GRADOS = ");
  Serial.print(mlx.readObjectTempC() - 20 );
  Serial.println("ºC");
  delay(3000);

  //Enviamos por el puerto serie la hora y fecha.
  Serial.print(hora);
  Serial.print(":");
  Serial.print(minuto);
  Serial.print(":");
  Serial.print(segundo);
  Serial.print(" , ");
  Serial.print(nombreDia[diaDeLaSemana]);
  Serial.print(" ");
  Serial.print(dia);
  Serial.print(" de ");
  Serial.print(nombreMes[mes - 1]);
  Serial.print(" del ");
  Serial.print(anio);
  Serial.println();
  delay(1000);
  digitalWrite(laser_pin, LOW); // Laser OFF

  //Restablecemos el ServoBrazo a la posicion original
 // restablecer_brazoServo();


  //GUARDAMOS temperatura en memoria SD.
  /* HACER CODIGO MEMORIA SD*/
//  GuardarDatos(hora, minuto, segundo, dia, nombreDia[diaDeLaSemana], nombreMes[mes - 1], anio, mlx.readObjectTempC() );

  if (mlx.readObjectTempC() >= 30)
  {
    Serial.println("Temperatura ALTA!");
    //Mensaje en lcd
    lcd.clear();
    lcd.setCursor(0, 0); // Primer renglon
    lcd.print("¡ALERTA!");
    lcd.setCursor(0, 1); // segundo renglon
    lcd.print("Bomba TEMP!");
    SIM800L.println("ATD+PHONE NUMBER;"); //Once the handshake test is successful, it will back to OK
    UpdateSerialSIM();

  }

  proceso_finalizado = true;


}
