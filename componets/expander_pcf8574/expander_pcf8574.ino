#include <Wire.h>

// Dirección I2C del módulo HW-171
const int hw171Address = 0x20;

// Definición de los patrones de segmentos para cada dígito del 0 al 9
byte numbers[10] = {
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

void setup()
{
    Wire.begin(); // Inicia la comunicación I2C
}

void loop()
{
    // Cuenta del 0 al 9
    for (int i = 0; i < 10; i++)
    {
        displayNumber(i); // Llama a la función para mostrar el número
        delay(500);       // Muestra cada número durante 500 milisegundos
    }
}

// Función para mostrar un dígito en el display de siete segmentos utilizando el HW-171
void displayNumber(int num)
{
    Wire.beginTransmission(hw171Address); // Inicia la transmisión hacia la dirección del módulo
    Wire.write(numbers[num]);             // Escribe el patrón para el número al módulo HW-171
    Wire.endTransmission();               // Finaliza la transmisión
}
