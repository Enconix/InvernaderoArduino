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



#include <DHT.h>

//Pin para señal DATA del sensor
#define DHTPIN 2

//Modelo del Sensor
#define DHTTYPE DHT11

//Pin para heater
#define heater 7

//Pin para humidificador
#define humidificador 8

//Pin para ventilador
#define fan 9

//Pin para ventilador humedad
#define fanhum 10

//Temperatura a mantener por el sistema
float temperatura_ref = 25.0;

//Humedad a mantener por el sistema
float humedad_ref = 50.0;

//Variacion de temperatura para correccion
float delta_t = 5.0;

//Ciclos correctivos maximos
int ciclos_ref=10;
//Ciclos de trabajo
int ciclo=0;

DHT dht(DHTPIN, DHTTYPE); //Inicializar sensor

  
void setup() {
  
  
  Serial.begin(9600); //Iniciar comunicación serie

  //Definición salidas 
  
  pinMode(heater, OUTPUT);
  pinMode(humidificador, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(fanhum, OUTPUT);

  digitalWrite(heater,LOW);
}

void loop() {
  // Esperar 6 segundos entre medidas
  delay(6000);
  
  // Lectura de humedad (%)
  float h = dht.readHumidity();
  // Lectura de Temperatura (°C)
  float t = dht.readTemperature();
  
  //Verificación de lectura
  if (isnan(h) || isnan(t) ){
      Serial.println("Error de Lectura, Verificar Sensor o Conexión");
  }

  if (!(isnan(h) || isnan(t))) { //Verificamos sensor
    
      /*Si la temperatura está por debajo del umbral inferior 
       * y no se superó el límite de ciclos correctivos
       */
      if((h <= (temperatura_ref - delta_t)) && ciclo<=ciclos_ref){aumentar_temp();}
      
      
     
  }

}

void aumentar_temp(){
    //Si el heater está apagado, se prende
    
    if (!(digitalRead(heater))){
       digitalWrite(heater,HIGH); //Prender heater
    }
    
    ciclo += ciclo; //Sumamos Ciclo 

    if (ciclo>ciclos_ref){
      digitalWrite(heater,LOW);
      Serial.println("Error de compensación, el actuador no puede elevar temperatura del invernadero");
    }
  }








  
