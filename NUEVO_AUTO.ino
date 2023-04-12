/***
       __ __
      (_ |_  _  __ __ _  \/ __
      __)|  (/_ |  | (/_ /  |
      __  _ __ __
       _)/ \ _) _)
      /__\_//__/__
      https://github.com/sferreyr
*/


#include <BufferedInput.h>
#include <BufferedOutput.h>
#include <loopTimer.h>
#include <millisDelay.h>
#include <SafeString.h>
#include <SafeStringNameSpaceEnd.h>
#include <SafeStringNameSpaceStart.h>
#include <SafeStringReader.h>
#include <SafeStringStream.h>
#include <SerialComs.h>
#include <AFMotor.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
#include <LiquidCrystal_I2C.h>
#include <EasyBuzzer.h>
#include <Servo.h>
#include <Wire.h>
#include "RTClib.h" // Reloj DS1307
#include <SD.h> // memoria SD
#include <SoftwareSerial.h> // Utilizado para el SIM800L

File myFile;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

/*Circuit TCRT5000 1
  // 5v           --->   VCC    VERDE OSCURO
  // Grnd         --->   Grnd   NEGRO
  // A0           --->   A12     BLANCO ->Sale del sensor -> BLANCO A ARDUINO
  // D0           --->   22     AZUL  ->Sale del sensor -> VERDE A ARDUINO
*/
//SENSOR DE DETECCION DE LINEA 1 (CENTRAL)
const int pinIRd = 22;
const int pinIRa = A12;
const int pinLED = 13; //LED de deteccion
int IRvalueA = 0;
int IRvalueD = 0;


/*Circuit TCRT5000 2
  // 5v           --->   VCC    VERDE
  // Grnd         --->   Grnd   AZUL
  // A0           --->   A13     BLANCO  ->Sale del sensor ->  BLANCO A ARDUINO
  // D0           --->   26     NARANJA ->Sale del sensor -> AMARILLO A ARDUINO
*/
//SENSOR DE DETECCION DE LINEA 2 IZQUIERDO
const int pinIRd_iz = 26;
const int pinIRa_iz = A9;
const int pinLED_iz = 13; //LED de deteccion
int IRvalueA_iz = 0;
int IRvalueD_iz = 0;


/*Circuit TCRT5000 3
  // 5v           --->   VCC    ROJO
  // Grnd         --->   Grnd   AZUL
  // A0           --->   A15     AMARILLO  ->Sale del sensor ->  AMARILLO A ARDUINO
  // D0           --->   24     BLANCO  ->Sale del sensor -> GRIS A ARDUINO
*/
//SENSOR DE DETECCION DE LINEA 3 DERECHO
const int pinIRd_der = 24;
const int pinIRa_der = A8; //  AMARILLO -- VERDE (ARDUINO)
const int pinLED_der = 13; //LED de deteccion
int IRvalueA_der = 0;
int IRvalueD_der = 0;
//Cuando el LED DE LOS SENSORES ENCIENDE , SIGNIFICA QUE DETECTO. (LOW)


/*Circuit TCRT5000 4
  // 5v           --->   VCC    ROJO
  // Grnd         --->   Grnd  NEGRO
  // A0           --->   A14     BLANCO
  // D8           --->   22     NARANJA
*/
//SENSOR DE DETECCION DE LINEA 4 (DETECTOR DE BOMBA)
const int pinIRd_Ladoderecho_centro = 42;
const int pinIRa_Ladoderecho_centro = A15;
const int pinLED_Ladoderecho_centro = 13; //LED de deteccion
int IRvalueA_Ladoderecho_centro = 0;
int IRvalueD_Ladoderecho_centro = 0;


// DC motor on M1
/*
   Dibujo de chasis
  ___MOTOR FRENTE___
   |1L        |3R
   |          |
   |          |
   |2L______ |4RB

*/
AF_DCMotor motor1L(2);
AF_DCMotor motor2LB(1);
AF_DCMotor motor3R(3);
AF_DCMotor motor4RB(4);

//Sensor Ultrasonido
const int EchoPin = 28;    //D5 (AMARILLO)  *
const int TriggerPin = 29; //D6 (BLANCO CREMA) * SE ENCUENTRAN INVERTIDOS PARA QUE FUNCIONEN
boolean obstaculo = false;

//BUZZER
const byte pinBuzzer = 24;


//PANTALLA DIGITAL LCM1602 V1
//EN ARDUINO MEGA SE UTILIZAN PINES SDA = 20 Y SCL = 21
//SDA  BLANCO -> A ARDUINO -> BLANCO
//SCL  ROJO Y BLANCO -> A ARDUINO -> BLANCO CLARO
const String Text1_LCD = "Inicializando. . .";
//Crear el objeto lcd  dirección  0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27, 16, 2); //


//Servo motor Brazo para elevar
//el sensor de temperatura hacia el michell.
Servo servo_brazo;  // create servo object to control a servo
int servo_pos = 0;    // variable to store the servo position
int servo_pin = 31; //Digital 24



//LASER
const int laser_pin = 48;

//Reloj DS1307
RTC_DS1307 rtc;
char nombreDia[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
char nombreMes[12][12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo",  "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
int segundo, minuto, hora, dia, mes, diaDeLaSemana;
long anio; //variable año
DateTime HoraFecha;

//Variables de Bombas y ubicacion
//BombaNro Define la bomba actual, al comenzar siempre es 0. El auto empieza desde porton tanque, hacia calle dorrego.
int ActualBomba = 0; //Bomba actual
const int MaxBombas = 4; // Bombas maximas actuales a controlar.
bool isBase = true; //Esta en base?
bool circuito_completo = false; // Cuando finaliza el control del total de bombas.

bool avanzando = false; // El vehiculo se encuentra avanzando?


//SIM 800L, Componente para las llamadas.
/*  ARDUINO MEGA     SIM800L
     10              TX   NARANJA
     11              RX BLANCO
*/
int RxPin_Blanco = 11;
int TxPin_Naranja = 10;
SoftwareSerial SIM800L(TxPin_Naranja, RxPin_Blanco); //SIM800L Tx & Rx is connected to Arduino #3 & #2



void setup() {

  // Mensajes en consola
  Serial.begin(9600); // set up Serial library at 9600 bps
  Serial.println("Consola!");

  //USO UNICO PARA SIM800L, para poder darle ordenes
  //Begin serial communication with Arduino and SIM800L
  SIM800L.begin(9600);
  SIM800L.println("AT"); //Once the handshake test is successful, it will back to OK

  //Iniciamos Reloj interno DS1307
  rtc.begin(); //Inicializamos el RTC


  //TCRT5000 SENSOR 2 (IZQUIERDA)
  pinMode(pinIRd_iz, INPUT);
  pinMode(pinIRa_iz, INPUT);
  pinMode(pinLED_iz, OUTPUT);

  //TCRT5000 SENSOR 1 (CENTRAL)
  pinMode(pinIRd, INPUT);
  pinMode(pinIRa, INPUT);
  pinMode(pinLED, OUTPUT);

  //TCRT5000 SENSOR 3 (DERECHA)
  pinMode(pinIRd_der, INPUT);
  pinMode(pinIRa_der, INPUT);
  pinMode(pinLED_der, OUTPUT);

  //TCRT5000 SENSOR 4  CENTRO (DERECHA)
  pinMode(pinIRd_Ladoderecho_centro, INPUT);
  pinMode(pinIRa_Ladoderecho_centro, INPUT);
  pinMode(pinLED_Ladoderecho_centro, OUTPUT);

  //Prueba de motores
  // turn on motors
  motor1L.setSpeed(220);
  motor2LB.setSpeed(220);
  motor3R.setSpeed(220);
  motor4RB.setSpeed(220);
  motor1L.run(RELEASE);
  motor2LB.run(RELEASE);
  motor3R.run(RELEASE);
  motor4RB.run(RELEASE);

  //Sensor Ultrasonido
  pinMode(TriggerPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  //BUZZER
  //  analogWrite(pinBuzzer, salida);
  // Configuración del pin
  EasyBuzzer.setPin(pinBuzzer);

  //PANTALLA LCD LMC1602
  // Inicializar el LCD
  lcd.init();
  //Encender la luz de fondo.
  lcd.backlight();
  lcd.print(Text1_LCD); // "Inicializando. . ."

  //Servo Brazo
  //TEST SERVO
  servo_brazo.attach(servo_pin);  // attaches the servo on pin 24 to the servo object
  servo_brazo.write(90);  // set servo to mid-point
  servo_brazo.detach();  // attaches the servo on pin 24 to the servo object

  //Laser del sensor de temperatura
  pinMode(laser_pin, OUTPUT); // Positivo para Laser


  //SENSOR TEMPERATURA
  // añadido para que funciones el sensor de temperatura JLSM
 /* pinMode(A13, OUTPUT); //SCL NARANJA
  pinMode(A14, OUTPUT); //SDA AMARILLO
  digitalWrite(A13, LOW);
  digitalWrite(A14, LOW);
  delay(100);*/
  //UTILIZA LOS PINES SDA20 y 21 EN CONJUUNTO CON LA PANTALLA LCD
  //SDA  SENSR-> BLANCO -> A ARDUINO -> BLANCO
  //SCL  SENSOR->NARANJA -> A ARDUINO -> BLANCO CLARO
  ///////////////////////////////////////////////////////JLSM
  mlx.begin();

}



void loop() {
    
  //SIM 800L
  UpdateSerialSIM();

  //Reloj interno DS1307
  Reloj();

  //Sensor de Linea 2 (IZQUIERDA)
  IRvalueA_iz = analogRead(pinIRa_iz);
  IRvalueD_iz = digitalRead(pinIRd_iz);

  //Sensor de Linea 1 (CENTRAL)
  IRvalueA = analogRead(pinIRa);
  IRvalueD = digitalRead(pinIRd);

  //Sensor de Linea 3 (DERECHA)
  IRvalueA_der = analogRead(pinIRa_der);
  IRvalueD_der  = digitalRead(pinIRd_der);

  //Sensor de Linea 4  CENTRO(DERECHA)
  IRvalueA_Ladoderecho_centro = analogRead(pinIRa_Ladoderecho_centro);
  IRvalueD_Ladoderecho_centro  = digitalRead(pinIRd_Ladoderecho_centro);


  //Sensor de bomba.
  if ( IRvalueD_Ladoderecho_centro == LOW &&avanzando == true) {
    Serial.print("SENSOR DE BOMBAS DETECTANDO!! \n");
     controlTemperatura();

     if ( ActualBomba <= MaxBombas){
     //Sumamos a la bomba actual
     ActualBomba = ActualBomba +1;
    
     }
     else if (ActualBomba == MaxBombas){
       Serial.print("Circuito Completo, las 4 bombas sensadas. \n");
       //VOLVER A LA INVERSA, RESTANDOLE AL ACTUAL BOMBA NUEVAMENTE CADA SENSOR.
      }

  }


  /*    PISO DE LA SALA DE MAQUINA ES INDETECTABLE, LOS 3 SENSORES SON SIEMPRE LOW.
  */
  //Si todos los sensores no detectan nada.
  if (IRvalueD == HIGH && IRvalueD_iz == HIGH  && IRvalueD_der == HIGH ) {
    // delay(5000);
    detener_motores();
    //Mensaje en lcd
    lcd.clear();
    lcd.setCursor(0, 0); // Primer renglon
    lcd.print("    Stand By    ");
    lcd.setCursor(0, 1); // segundo renglon
    lcd.print("ZzZzZzZzZzZzZzZz");
    lcd.clear();
    //   Serial.print("NO SE DETECTA NADA \n");
  }

  //Si el sensor central detecta la linea
  if (IRvalueD == LOW && IRvalueD_iz == HIGH  && IRvalueD_der == HIGH ) {
    Serial.print("SENSOR CENTRAL DETECTO \n");
    Serial.print("AUTO AVANZA LINEA RECTA \n");
      avanzar();
      avanzando = true;
   
  }

  //Si el Sensor izquierdo detecta la linea.
  if (IRvalueD == HIGH && IRvalueD_iz == LOW && IRvalueD_der == HIGH ) {
    Serial.print("SENSOR IZQUIERDO DETECTO \n");
     girar_izquierda();
  }

  //Si el Sensor derecho detecta la linea.
  if (IRvalueD == HIGH && IRvalueD_iz == HIGH  && IRvalueD_der == LOW ) {
    Serial.print("SENSOR DERECHO DETECTO \n");
      girar_derecha();
  }

  //Si los sensores TODOS DETECTAN.
  if (IRvalueD == LOW && IRvalueD_iz == LOW  && IRvalueD_der == LOW ) {
    Serial.print("Todos los sensores detectaron \n");
    detener_motores();
    avanzar();

  }






}



//Ping Usado en Sensor de UltraSonido
/*int ping(int TriggerPin, int EchoPin)
  {
  long duration, distanceCm;
  digitalWrite(TriggerPin, LOW); //para generar un pulso limpio ponemos a LOW 4us
  delayMicroseconds(4);
  digitalWrite(TriggerPin, HIGH); //generamos Trigger (disparo) de 10us
  delayMicroseconds(10);
  digitalWrite(TriggerPin, LOW);
  duration = pulseIn(EchoPin, HIGH); //medimos el tiempo entre pulsos, en microsegundos

  distanceCm = duration * 10 / 292 / 2; //convertimos a distancia, en cm
  return distanceCm;
  }
*/
// void CheckObject(int valor_medido) {
// if (valor_medido  <= 20 /*CMS*/) {
/*  //   Serial.println("Objecto Muy cerca, se detienen los motores.");
  // Serial.println("Distancia del objeto: " + valor_medido );
  obstaculo = true;
  //Mensaje en lcd
  lcd.clear();
  lcd.setCursor(0, 0); // Primer renglon
  lcd.print("Obstaculo");
  lcd.setCursor(0, 1); // segundo renglon
  lcd.print("Detectado");


  }
  else {
  obstaculo = false;


  }

  }*/
