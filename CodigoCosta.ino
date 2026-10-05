#include <DHT.h>

#define DHTPIN 2          // Pin digital donde está conectado el sensor DHT
#define DHTTYPE DHT11     // Modelo de sensor DHT11

DHT dht(DHTPIN, DHTTYPE);

const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 3;
const int IN4 = 11;
int velocidad = 150;
char comando = '.';

// Variables para el temporizador no bloqueante
unsigned long tiempoAnterior = 0;
const long intervalo = 2000; // Leer sensor cada 2000 ms (2 segundos)

void setup() {
  // Declarar las salidas para los motores
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);//Iniciar la comunicación serial
  dht.begin();//Iniciar el sensor de humedad y temperatura
}

void loop() {
  // 1. Control de Motores (Solo lee si hay algo en el buffer)
  if (Serial.available() > 0) {
    comando = Serial.read();
    if (comando == 'F') { // Adelante
      analogWrite(IN1, 0);
      analogWrite(IN2, velocidad);
      analogWrite(IN3, 0);
      analogWrite(IN4, velocidad);    }
    else if (comando == 'B') { // Atrás
      analogWrite(IN1, velocidad);
      analogWrite(IN2, 0);
      analogWrite(IN3, velocidad);
      analogWrite(IN4, 0);    }
    else if (comando == 'L') { // Izquierda
      analogWrite(IN1, velocidad);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, velocidad);    }
    else if (comando == 'R') { // Derecha
      analogWrite(IN1, 0);
      analogWrite(IN2, velocidad);
      analogWrite(IN3, velocidad);
      analogWrite(IN4, 0);
    }
    else if (comando == 'S') { // Stop
      analogWrite(IN1, 0);
      analogWrite(IN2, 0);
      analogWrite(IN3, 0);
      analogWrite(IN4, 0);
    } 
  }
  // 2. Lectura y envío de sensores (No bloqueante)
  unsigned long tiempoActual = millis();
  
  if (tiempoActual - tiempoAnterior >= intervalo) {
    tiempoAnterior = tiempoActual;

    float h = dht.readHumidity();
    float t = dht.readTemperature(); // En Celsius

    // Verificar si la lectura falló
    if (!isnan(h) && !isnan(t)) {
      // Formato de envío: Temperatura,Humedad (separado por coma)
      Serial.print(t);
      Serial.print(",");
      Serial.println(h); // println añade el \n final vital para App Inventor
    }
  }
}
