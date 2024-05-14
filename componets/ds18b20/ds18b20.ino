#include <OneWire.h>
#include <DallasTemperature.h>

// Definición del pin digital al que está conectado el sensor DS18B20
#define ONE_WIRE_BUS 2

// Inicialización de la conexión OneWire y del sensor DallasTemperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
    // Inicialización del monitor serial a 9600 baudios
    Serial.begin(9600);

    // Inicialización del sensor DS18B20
    sensors.begin();
}

void loop()
{
    // Solicita la temperatura al sensor DS18B20
    sensors.requestTemperatures();

    // Lee la temperatura en grados Celsius desde el sensor DS18B20
    float temperatura = sensors.getTempCByIndex(0); // Se asume que solo hay un sensor conectado

    // Muestra la temperatura por el monitor serial
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println("°C");

    // Espera 1 segundo antes de la próxima lectura
    delay(1000);
}
