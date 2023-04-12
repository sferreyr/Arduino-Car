/***
 *     __ __                  
 *    (_ |_  _  __ __ _  \/ __
 *    __)|  (/_ |  | (/_ /  | 
 *    __  _ __ __             
 *     _)/ \ _) _)            
 *    /__\_//__/__            
 *    https://github.com/sferreyr
 */


//If we need adjust date in clock module.
 #define ADJUST_DATE
 
//Reloj DS1307
void Reloj() {
//  AdjustDate();
  segundo = HoraFecha.second();
  minuto = HoraFecha.minute();
  hora = HoraFecha.hour();
  dia = HoraFecha.day();
  mes = HoraFecha.month();
  anio = HoraFecha.year();
  diaDeLaSemana = HoraFecha.dayOfTheWeek();
}

#ifdef ADJUST_DATE

void AdjustDate(){
  rtc.begin(); //Inicializamos el RTC
  Serial.println("Estableciendo Hora y fecha...");
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.println("DS1307 actualizado con la hora y fecha que se compilo este programa:");
  Serial.print("Fecha = ");
  Serial.print(__DATE__);
  Serial.print("  Hora = ");
  Serial.println(__TIME__);
}
  
 #endif
