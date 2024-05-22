#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Definición del pin digital al que está conectado el sensor DS18B20
#define ONE_WIRE_BUS 2

// Definición de la constante BETA del sensor NTC
const float BETA = 3950;

// Variables para almacenar el valor analógico y la temperatura en grados Celsius
int analogValue;
float average_temp;

// Definición del pin analógico al que está conectado el potenciómetro
const int analogPinPotenciometer = A1;
// Variable para almacenar el valor leído del potenciómetro
int valPotenciometer;
// Variable para almacenar el intervalo de tiempo entre mediciones
int interval;

// Inicialización de la conexión OneWire y del sensor DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Define los pines para cada segmento (a hasta g) del primer display (unidades)
const int segmentUnitPins[7] = {3, 4, 5, 6, 7, 8, 9}; // Pines para los segmentos a-g
const byte unitsNumbers[10] = {
    B11111100, // 0
    B01100000, // 1
    B11011010, // 2
    B11110010, // 3
    B01100110, // 4
    B10110110, // 5
    B10111110, // 6
    B11100000, // 7
    B11111110, // 8
    B11110110  // 9
};

// Dirección I2C del módulo HW-171
const int hw171Address = 0x20;
// Define los pines para cada segmento (a hasta g) del segundo display (decenas)
byte tensNumbers[10] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 9
};

// ------------------------------------------------------------

// Prototipos de funciones
float ntc_sensor();
float ds18b20_sensor();
float average_temperature(float celsius_ntc, float celsius_ds18b20);
int potentiometer_interval();
void unitDisplayNumber(int num);
void tensDisplayNumber(int num);
int getUnits(float numero);
int getTens(float numero);

void setup() {
    // Inicialización de la comunicación serial para la salida de datos
    Serial.begin(9600);

    // Inicialización del sensor DS18B20
    sensors.begin();

    // Establece los pines de los segmentos como salidas
    for (int i = 0; i < 7; i++) {
        pinMode(segmentUnitPins[i], OUTPUT);
    }

    // Inicia la comunicación I2C
    Wire.begin();
}

void loop() {

    // Lectura del intervalo de tiempo entre mediciones
    interval = potentiometer_interval();
    Serial.print("Intervalo de tiempo entre mediciones: ");
    Serial.print(interval);
    Serial.println("ms");

    // Lectura de la temperatura de los sensores NTC y DS18B20
    float ntc_temp = ntc_sensor();
    float ds18b20_temp = ds18b20_sensor();
    average_temp = average_temperature(ntc_temp, ds18b20_temp);

    // Impresión de la temperatura medida por el monitor serial
    Serial.print("Temperatura: ");
    Serial.print(average_temp);
    Serial.println(" °C");

    // Muestra las unidades de la temperatura en el display de siete segmentos
    unitDisplayNumber(getUnits(average_temp));
    // Muestra las decenas de la temperatura en el display de siete segmentos utilizando el HW-171
    tensDisplayNumber(getTens(average_temp));

    delay(interval);
}

float ntc_sensor() {

    float celsius_of_ntc;
    // Lectura del valor analógico del pin A0, donde está conectado el sensor NTC
    analogValue = analogRead(A0);

    // Cálculo de la temperatura en grados Celsius utilizando la ecuación de Steinhart-Hart
    // Esta ecuación se basa en la variación de la resistencia del sensor con la temperatura
    // y la constante BETA que caracteriza el sensor
    celsius_of_ntc = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

    return celsius_of_ntc;
}

float ds18b20_sensor() {

    // Solicita la temperatura al sensor DS18B20
    sensors.requestTemperatures();

    // Lee la temperatura en grados Celsius desde el sensor DS18B20
    float celsius_of_ds18b20 = sensors.getTempCByIndex(0); // Se asume que solo hay un sensor conectado

    return celsius_of_ds18b20;
}

float average_temperature(float celsius_ntc, float celsius_ds18b20) {

    // Cálculo del promedio de las temperaturas medidas por el sensor NTC y el sensor DS18B20
    float average_temp = (celsius_ntc + celsius_ds18b20) / 2;

    return average_temp;
}

int potentiometer_interval() {

    int interval_of_potentiometer;

    // Lectura del valor analógico del potenciómetro
    valPotenciometer = analogRead(analogPinPotenciometer);

    // Mapeo del valor del potenciómetro al intervalo de tiempo entre mediciones
    // El valor del potenciómetro varía de 0 a 1023, y el intervalo de tiempo se mapea de 500 a 5000 ms
    interval_of_potentiometer = map(valPotenciometer, 0, 1023, 500, 5000);

    return interval_of_potentiometer;
}

// Función para mostrar un dígito en el display de siete segmentos
void unitDisplayNumber(int num) {

    // Apaga todos los segmentos
    for (int i = 0; i < 8; i++) {
        digitalWrite(segmentUnitPins[i], HIGH); // Alto (apagado)
    }

    // Activa los segmentos basados en el número a mostrar
    for (int i = 0; i < 8; i++) {
        // Verifica si el bit en la posición i del número está activo
        if (bitRead(unitsNumbers[num], i) == LOW) {
            // Activa el segmento correspondiente
            digitalWrite(segmentUnitPins[7 - i], LOW);
        }
    }
}

// Función para mostrar un dígito en el display de siete segmentos utilizando el HW-171
void tensDisplayNumber(int num) {

    Wire.beginTransmission(hw171Address); // Inicia la transmisión hacia la dirección del módulo
    Wire.write(tensNumbers[num]);         // Escribe el patrón para el número al módulo HW-171
    Wire.endTransmission();               // Finaliza la transmisión
}

int getUnits(float numero) {
    int entero = numero;        // Convierte el número decimal a un entero
    int unidades = entero % 10; // Obtiene el módulo 10 para obtener las unidades
    return unidades;            // Retorna las unidades como un entero
}

int getTens(float numero) {
    int entero = numero;              // Convierte el número decimal a un entero
    int decenas = (entero / 10) % 10; // Obtiene el cociente de la división entera entre 10 y luego el módulo 10 para obtener las decenas
    return decenas;                   // Retorna las decenas como un entero
}
