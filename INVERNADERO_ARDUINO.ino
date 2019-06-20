/* Invernadero Automático Arduino
 *  
 *  Autor: Martín Encina  (Enconix)
 *  Fecha 11/06/2019
 *  Ultima edición: 
 *  Descripción: Es un sistema de control automático para invernaderos pequeños, con Arduino.
 *  
 *  Maneja una sensor DHT11 (Se puede actualizar a un más preciso) y un 4 reles
 *  para las 4 acciones de control, aumentar temperatura (heater), aumentar humedad 
 *  (humidificador), reducir temperatura (ventilador), reducir humedad (ventilador_humedad)
 *  
 */



#include <dht_nonblocking.h>

#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
//#define DHT_SENSOR_TYPE DHT_TYPE_22

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );

//Pin para heater
#define heater 7
//Pin para ventilador
     
#define fan 9

//Temperatura a mantener por el sistema
float t_ref = 25.0;

//Humedad a mantener por el sistema
float h_ref = 50.0;

//Variacion de temperatura para correccion
float delta_t = 2.0;
  

//Ciclos correctivos maximos
int ciclos_ref=10;
//Ciclos de trabajo
int ciclo=0;

float t;
float h;
boolean flag_H = LOW;
void setup( )
{
  Serial.begin( 9600);
  pinMode(heater, OUTPUT);
  digitalWrite(heater,LOW);

  pinMode(fan, OUTPUT);
  digitalWrite(fan,LOW);
}
void loop( )
{
  
 if(dht_sensor.measure(&t, &h)){
    Serial.print( "T = " );
    Serial.print( t);
    Serial.print( " deg. C, H = " );
    Serial.print( h);
    Serial.println( "%" );

    if (t <= (t_ref - delta_t)){aumentar_temp();}  //Si está por debajo de 23
    else if (t<= (t_ref + delta_t - 1.0) && digitalRead(heater)){aumentar_temp();} //si está por debajo de 27 y calentando, seguir.
    else {digitalWrite(heater,LOW); ciclo=0;}  //Si heater==off (ciclo de enfriamiento) o temperatura alta, apagar heater y ciclo
   
}
}
void aumentar_temp() {
 
  Serial.print("Heating, intento:  ");
  Serial.println(ciclo);
  if ((!(digitalRead(heater))) && (ciclo <= ciclos_ref)){
       digitalWrite(heater,HIGH); //Prender heater
      }
    delay(6000);   //Calentar 6 segundos
    ciclo = ++ciclo;
   if (ciclo>ciclos_ref){
      digitalWrite(heater,LOW);
      Serial.println("Error de compensación, el actuador no puede elevar temperatura del invernadero");
      ciclo = ciclos_ref + 1; //Evitamos que se encienda el heater.
    }
  }

  
