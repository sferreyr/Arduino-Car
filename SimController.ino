/***
 *     __ __                  
 *    (_ |_  _  __ __ _  \/ __
 *    __)|  (/_ |  | (/_ /  | 
 *    __  _ __ __             
 *     _)/ \ _) _)            
 *    /__\_//__/__            
 *    https://github.com/sferreyr
 */


 
void UpdateSerialSIM()
{
  //  delay(500);
  while (Serial.available())
  {
    SIM800L.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (SIM800L.available())
  {
    Serial.write(SIM800L.read());//Forward what Software Serial received to Serial Port
  }


}
