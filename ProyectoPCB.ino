// ***********************************************************************
// Universida del Valle de Guatemala
// Simulación de Circuitos y Fabricación de PCB
// Proyecto Final - Código
// Daniela Navas
// ***********************************************************************

// ***********************************************************************
// Librerias 
// ***********************************************************************
#include <LiquidCrystal_I2C.h> 
#include <Servo.h> //Libreria para Servo
// ***********************************************************************
// Definición de Pines 
// ***********************************************************************
const int buttonL = 4; //Boton Izquierda
const int buttonR = 5; //Boton Derecha
const int buttonS = 6; //Boton Seleccionar

const int buzzer = A3; //Bocina para acompañar la alerta de la Led con un sonido

// ***********************************************************************
// Variables Globales 
// ***********************************************************************
bool estadoL = HIGH;
bool estadoR = HIGH;
bool estadoS = HIGH;

//Variables para funcionamiento de sonido de alerta
unsigned long previousTimeB = 0;
unsigned long currentTimeB = 0;

//Variables para el temporizador de Pastilla 1
unsigned long timerDuration_P1 = 24UL * 60UL * 60UL; // 24 horas en segundos
unsigned long L_timerDuration_P1 = 24UL * 60UL * 60UL;

//Variables para el temporizador de Pastilla 2
unsigned long timerDuration_P2 = 24UL * 60UL * 60UL; // 24 horas en segundos
unsigned long L_timerDuration_P2 = 24UL * 60UL * 60UL * 1000UL;

//Variables para el temporizador de Pastilla 3
unsigned long timerDuration_P3 = 24UL * 60UL * 60UL * 1000UL; // 24 horas en segundos
unsigned long L_timerDuration_P3 = 24UL * 60UL * 60UL * 1000UL;

// Funcionamiento de Registro de Tiempo
unsigned long previousMillisTimer = 0;  // Variable para almacenar el tiempo anterior
const unsigned long intervalTimer = 1000; // Intervalo de 1 segundo en milisegundos

// ***********************************************************************
// Servo Motor
// ***********************************************************************
Servo servoP;  //Definir nombre de un Servo

// ***********************************************************************
// Maquina de Estados
// ***********************************************************************
//Definir Estados
typedef enum estados {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13,
                      s14, s15, s16, s17, s18, s19, s20, s21, s22, s23, s24, s25,
                      s26, s27, s28, s29, s30, s31, s32, s33, s34, s35, s36, s37,
                      s38, s39, s40, s41, s42, s43, s44, s45, s46} estados;
estados estadoActual = s0;
estados estadoFuturo = s0;

const long interval = 200;  //Intervalo de parpadeo
unsigned long previousMillis = 0; 

// ***********************************************************************
// Pantalla LCD
// ***********************************************************************
LiquidCrystal_I2C lcd(0x27,16,2); // si no te sale con esta direccion  puedes usar (0x3f,16,2) || (0x27,16,2)  ||(0x20,16,2) 

// ***********************************************************************
// Configuración 
// ***********************************************************************
void setup() {
  // Puerto Serial
  Serial.begin(9600);
  // Definir Botones
  pinMode(buttonL, INPUT_PULLUP); //Input Boton de Izquierda
  pinMode(buttonR, INPUT_PULLUP); //Input Boton de Derecha
  pinMode(buttonS, INPUT_PULLUP); //Input Boton de Seleccionar
  // LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  // Buzzer
  pinMode(buzzer, OUTPUT); //Output Sonido de Alerta
  // Servo
  servoP.attach(3);  //Conectar servo al pin 3
}

// ***********************************************************************
// Loop Principal 
// ***********************************************************************
void loop() {
  Serial.print("Timer P1: ");
  Serial.println(timerDuration_P1);
  Serial.print("Timer P2: ");
  Serial.println(timerDuration_P2);
  Serial.print("Timer P3: ");
  Serial.println(timerDuration_P3);

  // Lectura de botones
  estadoL = digitalRead(buttonL);
  estadoR = digitalRead(buttonR);
  estadoS = digitalRead(buttonS);

  // Alarmas | Timer
  unsigned long currentMillisTimer = millis(); // Obtiene el tiempo actual
  if (currentMillisTimer - previousMillisTimer >= intervalTimer) { // Comprueba si ha pasado el intervalo de 1 segundo
    previousMillisTimer = currentMillisTimer; // Guarda el tiempo actual como el tiempo anterior
    timerDuration_P1--; //Disminuir el Tiempo 1 Segundo
    timerDuration_P2--;
    timerDuration_P3--;
  }

  if (timerDuration_P1 == 0){
    timerDuration_P1 = L_timerDuration_P1;
    estadoActual = s44;
  }

  if (timerDuration_P2 == 0){
    timerDuration_P2 = L_timerDuration_P2;
    estadoActual = s45;
  }

  if (timerDuration_P3 == 0){
    timerDuration_P3 = L_timerDuration_P3;
    estadoActual = s46;
  }

  switch (estadoActual){
    case s0: // Reposo | Menú Principal (Sin Seleccionar)
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s1; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s2; 
      break;
    
    case s1: // Menú Principal | Manual Seleccionado
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s1;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s2; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s3;
      break;

    case s2: // Menú Principal | Alarmas Seleccionado
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s2;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s0; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s1; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s10;
      break;

    case s3: // Pastilla 1 | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s3;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s4; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s6; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s9;
      break;

    case s4: // Pastilla 2 | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s4;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s5; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s3; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s8;
      break;

    case s5: // Pastilla 3 | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s5;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s6; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s4; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s7;
      break;
    
    case s6: // Regresar | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s6;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s3; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s5; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0;
      break;
    
    case s7: // Abrir | Pastilla 3
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s7;
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0; 
      break;
    
    case s8: // Abrir | Pastilla 2
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s8;
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0; 
      break;

    case s9: // Abrir | Pastilla 1
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s9;
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0; 
      break;
    
    case s10: // Alarma 1 | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s10;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s11; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s13; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s14;
      break;
    
    case s11: // Alarma 2 | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s11;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s12; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s10; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s15;
      break;

    case s12: // Alarma 3 | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s12;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s13; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s16; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s9;
      break;
    
    case s13: // Regresar | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s13;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s10; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s12; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0;
      break;

    case s14: // Alarma 1 | Abierto
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s14;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s17; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s21; 
      break;

    case s15: // Alarma 2 | Abierto
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s15;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s22; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s26; 
      break;

    case s16: // Alarma 3 | Abierto
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s16;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s27; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s31; 
      break;

    case s17: // Alarma 1 | 6 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s17;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s18; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s14; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s32;
      break;

    case s18: // Alarma 1 | 9 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s18;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s19; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s17; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s33;
      break;

    case s19: // Alarma 1 | 12 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s19;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s20; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s18; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s34;
      break;

    case s20: // Alarma 1 | 24 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s20;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s21; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s19; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s35;
      break;

    case s21: // Alarma 1 | Regresar | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s21;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s17; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s20; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0;
      break;

    case s22: // Alarma 2 | 6 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s22;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s23; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s15; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s36;
      break;

    case s23: // Alarma 2 | 9 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s23;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s24; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s22; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s37;
      break;

    case s24: // Alarma 2 | 12 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s24;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s25; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s23; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s38;
      break;

    case s25: // Alarma 2 | 24 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s25;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s26; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s24; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s39;
      break;

    case s26: // Alarma 2 | Regresar | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s26;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s15; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s25; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0;
      break;

    case s27: // Alarma 3 | 6 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s27;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s28; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s16; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s40;
      break;

    case s28: // Alarma 3 | 9 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s28;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s29; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s27; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s41;
      break;

    case s29: // Alarma 3 | 12 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s29;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s30; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s28; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s42;
      break;

    case s30: // Alarma 3 | 24 Horas | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s30;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s31; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s29; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s43;
      break;

    case s31: // Alarma 3 | Regresar | Seleccionada
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s31;
      if(estadoL == HIGH && estadoR == LOW && estadoS == HIGH)
        estadoFuturo = s16; 
      if(estadoL == LOW && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s30; 
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0;
      break;

    case s32: // Alarma 1 | 6 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s33: // Alarma 1 | 9 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s34: // Alarma 1 | 12 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s35: // Alarma 1 | 24 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s36: // Alarma 2 | 6 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s37: // Alarma 2 | 9 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s38: // Alarma 2 | 12 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s39: // Alarma 2 | 24 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s40: // Alarma 3 | 6 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s41: // Alarma 3 | 9 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s42: // Alarma 3 | 12 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s43: // Alarma 3 | 24 Horas
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s0;
      break;

    case s44: // Pastilla 1 | Timer
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s44;
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0;
      break;

    case s45: // Pastilla 2 | Timer
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s45;
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0;
      break;

    case s46: // Pastilla 3 | Timer
      if(estadoL == HIGH && estadoR == HIGH && estadoS == HIGH)
        estadoFuturo = s46;
      if(estadoL == HIGH && estadoR == HIGH && estadoS == LOW)
        estadoFuturo = s0;
      break;

  }

  //Trancisiones de Estados
  switch (estadoActual){
    case s0: // Reposo | Menú Principal (Sin Seleccionar)
      lcd.setCursor(0, 0); 
      lcd.print("1.Manual        ");
      lcd.setCursor(0, 1); 
      lcd.print("2.Alarma        ");
      // Servo
      servoP.write(0); //Mover Servomotor a 0°
      break;

    case s1: // Menú Principal | Manual Seleccionado
      lcd.setCursor(0, 0); 
      lcd.print("1.Manual<       ");
      lcd.setCursor(0, 1); 
      lcd.print("2.Alarma        ");
      break;

    case s2: // Menú Principal | Alarmas Seleccionado
      lcd.setCursor(0, 0); 
      lcd.print("1.Manual        ");
      lcd.setCursor(0, 1); 
      lcd.print("2.Alarma<       ");
      break;

    case s3: // Pastilla 1 | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Pastilla        ");
      lcd.setCursor(0, 1); 
      lcd.print(" 1<  2   3   R  ");
      break;

    case s4: // Pastilla 2 | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Pastilla        ");
      lcd.setCursor(0, 1); 
      lcd.print(" 1   2<  3   R  ");
      break;
    
    case s5: // Pastilla 3 | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Pastilla        ");
      lcd.setCursor(0, 1); 
      lcd.print(" 1   2   3<  R  ");
      break;

    case s6: // Regresar | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Pastilla        ");
      lcd.setCursor(0, 1); 
      lcd.print(" 1   2   3   R< ");
      break;

    case s7: // Pastilla 3 | Abierta
      // Actualizar LCD
      lcd.setCursor(0, 0); 
      lcd.print("Pastilla 3      ");
      lcd.setCursor(0, 1); 
      lcd.print("        Cerrar< ");
      // Buzzer
      currentTimeB = millis(); 
      if (currentTimeB - previousTimeB >= 1000) { 
        previousTimeB = currentTimeB; 
        tone(buzzer, 1500); 
      } else{ 
        noTone(buzzer); 
      }
      previousTimeB = currentTimeB; 
      // Servo
      servoP.write(180); //Mover Servomotor a 180°
      break;

    case s8: // Pastilla 2 | Abierta
      // Actualizar LCD
      lcd.setCursor(0, 0); 
      lcd.print("Pastilla 2      ");
      lcd.setCursor(0, 1); 
      lcd.print("        Cerrar< ");
      // Buzzer
      currentTimeB = millis(); 
      if (currentTimeB - previousTimeB >= 1000) { 
        previousTimeB = currentTimeB; 
        tone(buzzer, 1500); 
      } else{ 
        noTone(buzzer); 
      }
      previousTimeB = currentTimeB; 
      // Servo
      servoP.write(90); //Mover Servomotor a 120°
      break;

    case s9: // Pastilla 1 | Abierta
      // Actualizar LCD
      lcd.setCursor(0, 0); 
      lcd.print("Pastilla 1      ");
      lcd.setCursor(0, 1); 
      lcd.print("        Cerrar< ");
      // Buzzer
      currentTimeB = millis(); 
      if (currentTimeB - previousTimeB >= 1000) { 
        previousTimeB = currentTimeB; 
        tone(buzzer, 1500); 
      } else{ 
        noTone(buzzer); 
      }
      previousTimeB = currentTimeB; 
      // Servo
      servoP.write(0); //Mover Servomotor a 60°
      break;
    
    case s10: // Alarma 1 | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma          ");
      lcd.setCursor(0, 1); 
      lcd.print(" 1<  2   3   R  ");
      break;
    
    case s11: // Alarma 2 | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma          ");
      lcd.setCursor(0, 1); 
      lcd.print(" 1   2<  3   R  ");
      break;

    case s13: // Regresar | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma          ");
      lcd.setCursor(0, 1); 
      lcd.print(" 1   2   3   R< ");
      break;

    case s14: // Alarma 1 | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 1        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12  24  R ");
      break;

    case s15: // Alarma 2 | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 2        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12  24  R ");
      break;

    case s16: // Alarma 3 | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 3        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12  24  R ");
      break;

    case s17: // Alarma 1 | 6 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 1        ");
      lcd.setCursor(0, 1); 
      lcd.print("6< 9  12  24  R ");
      break;

    case s18: // Alarma 1 | 9 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 1        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9< 12  24  R ");
      break;

    case s19: // Alarma 1 | 12 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 1        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12< 24  R ");
      break;

    case s20: // Alarma 1 | 24 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 1        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12  24< R ");
      break;

    case s21: // Alarma 1 | Regresar | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 1        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12  24  R<");
      break;

    case s22: // Alarma 2 | 6 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 2        ");
      lcd.setCursor(0, 1); 
      lcd.print("6< 9  12  24  R ");
      break;

    case s23: // Alarma 2 | 9 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 2        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9< 12  24  R ");
      break;

    case s24: // Alarma 2 | 12 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 2        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12< 24  R ");
      break;

    case s25: // Alarma 2 | 24 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 2        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12  24< R ");
      break;

    case s26: // Alarma 2 | Regresar | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 2        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12  24  R<");
      break;

    case s27: // Alarma 3 | 6 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 3        ");
      lcd.setCursor(0, 1); 
      lcd.print("6< 9  12  24  R ");
      break;

    case s28: // Alarma 3 | 9 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 3        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9< 12  24  R ");
      break;

    case s29: // Alarma 3 | 12 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 3        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12< 24  R ");
      break;

    case s30: // Alarma 3 | 24 Horas | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 3        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12  24< R ");
      break;

    case s31: // Alarma 3 | Regresar | Seleccionada
      lcd.setCursor(0, 0); 
      lcd.print("Alarma 3        ");
      lcd.setCursor(0, 1); 
      lcd.print("6  9  12  24  R<");
      break;

    case s32: // Alarma 1 | 6 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 1   ");
      lcd.setCursor(0, 1); 
      lcd.print("   En 6 Horas   ");
      timerDuration_P1 = 6UL * 60UL * 60UL; // 6 Horas en Segundos
      L_timerDuration_P1 = 6UL * 60UL * 60UL; // 6 Horas en Segundos
      break;

    case s33: // Alarma 1 | 9 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 1   ");
      lcd.setCursor(0, 1); 
      lcd.print("   En 9 Horas   ");
      timerDuration_P1 = 9UL * 60UL * 60UL; // 9 Horas en Segundos
      L_timerDuration_P1 = 9UL * 60UL * 60UL; // 9 Horas en Segundos
      break;

    case s34: // Alarma 1 | 12 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 1   ");
      lcd.setCursor(0, 1); 
      lcd.print("  En 12 Horas   ");
      delay(2000);
      timerDuration_P1 = 12UL * 60UL * 60UL; // 12 Horas en Segundos
      L_timerDuration_P1 = 12UL * 60UL * 60UL; // 12 Horas en Segundos
      break;

    case s35: // Alarma 1 | 24 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 1   ");
      lcd.setCursor(0, 1); 
      lcd.print("  En 24 Horas   ");
      delay(2000);
      timerDuration_P1 = 24UL * 60UL * 60UL; // 24 Horas en Segundos
      L_timerDuration_P1 = 24UL * 60UL * 60UL; // 24 Horas en Segundos // 24 Horas en Ms
      break;

    case s36: // Alarma 2 | 6 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 2   ");
      lcd.setCursor(0, 1); 
      lcd.print("   En 6 Horas   ");
      delay(2000);
      timerDuration_P2 = 6UL * 60UL * 60UL; // 6 Horas en Segundos
      L_timerDuration_P2 = 6UL * 60UL * 60UL; // 6 Horas en Segundos
      break;

    case s37: // Alarma 2 | 9 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 2   ");
      lcd.setCursor(0, 1); 
      lcd.print("   En 9 Horas   ");
      delay(2000);
      timerDuration_P2 = 9UL * 60UL * 60UL; // 9 Horas en Segundos
      L_timerDuration_P2 = 9UL * 60UL * 60UL; // 9 Horas en Segundos
      break;

    case s38: // Alarma 2 | 12 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 2   ");
      lcd.setCursor(0, 1); 
      lcd.print("  En 12 Horas   ");
      delay(2000);
      timerDuration_P2 = 12UL * 60UL * 60UL; // 12 Horas en Segundos
      L_timerDuration_P2 = 12UL * 60UL * 60UL; // 12 Horas en Segundos
      break;

    case s39: // Alarma 2 | 24 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 2   ");
      lcd.setCursor(0, 1); 
      lcd.print("  En 24 Horas   ");
      delay(2000);
      timerDuration_P2 = 24UL * 60UL * 60UL; // 24 Horas en Segundos
      L_timerDuration_P2 = 24UL * 60UL * 60UL; // 24 Horas en Segundos
      break;

    case s40: // Alarma 3 | 6 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 3   ");
      lcd.setCursor(0, 1); 
      lcd.print("   En 6 Horas   ");
      delay(2000);
      timerDuration_P3 = 6UL * 60UL * 60UL; // 6 Horas en Segundos
      L_timerDuration_P3 = 6UL * 60UL * 60UL; // 6 Horas en Segundos
      break;

    case s41: // Alarma 3 | 9 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 3   ");
      lcd.setCursor(0, 1); 
      lcd.print("   En 9 Horas   ");
      delay(2000);
      timerDuration_P3 = 9UL * 60UL * 60UL; // 9 Horas en Segundos
      L_timerDuration_P3 = 9UL * 60UL * 60UL; // 9 Horas en Segundos
      break;

    case s42: // Alarma 3 | 12 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 3   ");
      lcd.setCursor(0, 1); 
      lcd.print("  En 12 Horas   ");
      delay(2000);
      timerDuration_P3 = 12UL * 60UL * 60UL; // 12 Horas en Segundos
      L_timerDuration_P3 = 12UL * 60UL * 60UL; // 12 Horas en Segundos
      break;

    case s43: // Alarma 3 | 24 Horas | Marcada
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 3   ");
      lcd.setCursor(0, 1); 
      lcd.print("  En 24 Horas   ");
      timerDuration_P3 = 24UL * 60UL * 60UL; // 24 Horas en Segundos
      L_timerDuration_P3 = 24UL * 60UL * 60UL; // 24 Horas en Segundos
      break;

    case s44: // Pastilla 1 Abierto con Alarma
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 1   ");
      lcd.setCursor(0, 1); 
      lcd.print("      Regresar <");
      // Buzzer
      currentTimeB = millis(); 
      if (currentTimeB - previousTimeB >= 5000) { 
        previousTimeB = currentTimeB; 
        tone(buzzer, 1500); 
      } else{ 
        noTone(buzzer); 
      }
      previousTimeB = currentTimeB; 
      servoP.write(0); //Mover Servomotor a 0°
      break;
    
    case s45: // Pastilla 2 Abierto con Alarma
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 2   ");
      lcd.setCursor(0, 1); 
      lcd.print("      Regresar <");
      /// Buzzer
      currentTimeB = millis(); 
      if (currentTimeB - previousTimeB >= 5000) { 
        previousTimeB = currentTimeB; 
        tone(buzzer, 1500); 
      } else{ 
        noTone(buzzer); 
      }
      previousTimeB = currentTimeB;
      servoP.write(90); //Mover Servomotor a 90°
      break;

    case s46: // Pastilla 3 Abierto con Alarma
      lcd.setCursor(0, 0); 
      lcd.print("   Pastilla 3   ");
      lcd.setCursor(0, 1); 
      lcd.print("      Regresar <");
      // Buzzer
      currentTimeB = millis(); 
      if (currentTimeB - previousTimeB >= 5000) { 
        previousTimeB = currentTimeB; 
        tone(buzzer, 1500); 
      } else{ 
        noTone(buzzer); 
      }
      previousTimeB = currentTimeB;
      servoP.write(180); //Mover Servomotor a 180°
      break;

  } 

  //Actualización de Estados
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    estadoActual = estadoFuturo;
    previousMillis = currentMillis;
  }
  delay(1);

}
