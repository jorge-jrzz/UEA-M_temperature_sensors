#include <OneWire.h>
#include <DallasTemperature.h>

// Definición de la constante BETA del sensor NTC
const float BETA = 3950;

// Definición del pin digital al que está conectado el sensor DS18B20
#define ONE_WIRE_BUS 2

// Variables para almacenar el valor analógico y la temperatura en grados Celsius
int analogValue;
float average_temp;

// Definición del pin analógico al que está conectado el potenciómetro
const int analogPinPotenciometer = 1;
// Variable para almacenar el valor leído del potenciómetro
int valPotenciometer;
int interval; // Variable para almacenar el intervalo de tiempo entre mediciones

// Inicialización de la conexión OneWire y del sensor DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Prototipos de funciones
float ntc_sensor();
float ds18b20_sensor();
float average_temperature(float celsius_ntc, float celsius_ds18b20);
int potentiometer_interval();

void setup()
{
    // Inicialización de la comunicación serial para la salida de datos
    Serial.begin(9600);

    // Inicialización del sensor DS18B20
    sensors.begin();
}

void loop()
{
    average_temp = average_temperature(ntc_sensor(), ds18b20_sensor());

    // Impresión de la temperatura medida por el monitor serial
    Serial.print("Temperatura: ");
    Serial.print(average_temp);
    Serial.println(" C");

    interval = potentiometer_interval();
    Serial.print("Intervalo de tiempo entre mediciones: ");
    Serial.print(interval);
    Serial.println("ms");

    // Retardo para la próxima lectura de temperatura
    delay(interval);
}

float ntc_sensor()
{
    float celsius_of_ntc;
    // Lectura del valor analógico del pin A0, donde está conectado el sensor NTC
    analogValue = analogRead(A0);

    // Cálculo de la temperatura en grados Celsius utilizando la ecuación de Steinhart-Hart
    // Esta ecuación se basa en la variación de la resistencia del sensor con la temperatura
    // y la constante BETA que caracteriza el sensor
    celsius_of_ntc = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

    return celsius_of_ntc;
}

float ds18b20_sensor()
{
    // Solicita la temperatura al sensor DS18B20
    sensors.requestTemperatures();

    // Lee la temperatura en grados Celsius desde el sensor DS18B20
    float celsius_of_ds18b20 = sensors.getTempCByIndex(0); // Se asume que solo hay un sensor conectado

    return celsius_of_ds18b20;
}

float average_temperature(float celsius_ntc, float celsius_ds18b20)
{
    // Cálculo del promedio de las temperaturas medidas por el sensor NTC y el sensor DS18B20
    float average_temp = (celsius_ntc + celsius_ds18b20) / 2;

    return average_temp;
}

int potentiometer_interval()
{
    int interval_of_potentiometer;
    // Lectura del valor analógico del potenciómetro
    valPotenciometer = analogRead(analogPinPotenciometer);

    interval_of_potentiometer = map(valPotenciometer, 0, 1023, 500, 5000);

    return interval_of_potentiometer;
}
