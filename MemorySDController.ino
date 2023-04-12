/***
 *     __ __                  
 *    (_ |_  _  __ __ _  \/ __
 *    __)|  (/_ |  | (/_ /  | 
 *    __  _ __ __             
 *     _)/ \ _) _)            
 *    /__\_//__/__            
 *    https://github.com/sferreyr
 */


void GuardarDatos(int hora, int minuto, int segundos, int dia, String nombreDia, String nombreMes, int Anio, float TemperaturaMichell) {

  // Memoria SD
  //Memoria SD
  pinMode(53, OUTPUT);
  Serial.print("Iniciando SD ...");
  if (!SD.begin(53)) {
    Serial.println("No se pudo inicializar");
    return;
  }
  Serial.println("inicializacion exitosa");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("datos.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Escribiendo datos Obtenidos");
    myFile.println( "" + nombreDia + " " + dia + " de " + nombreMes + " " + Anio + " " + "HORA:" + hora + " " + "Con:" + " " + minuto + " " + "Minutos y" + " " + segundos + " " + "Segundos "
                    + "\t"
                    + "Bomba " + ActualBomba + " " + "Temperatura Michell/Rodamientos: " + TemperaturaMichell);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening datos.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("datos.txt");
  if (myFile) {
    Serial.println("datos.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening datos.txt");
  }


}
