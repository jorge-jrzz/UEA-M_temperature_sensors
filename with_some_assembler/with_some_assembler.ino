#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

const float BETA = 3950;

int analogValue;
float average_temp;

const int analogPinPotenciometer = A1;

int valPotenciometer;
int interval;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int segmentUnitPins[7] = {3, 4, 5, 6, 7, 8, 9};

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

const int hw171Address = 0x20;

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

float ntc_sensor();
float ds18b20_sensor();
float average_temperature(float celsius_ntc, float celsius_ds18b20);
int potentiometer_interval();
void unitDisplayNumber(int num);
void tensDisplayNumber(int num);
int getUnits(float numero);
int getTens(float numero);

void setup()
{
    Serial.begin(9600);
    sensors.begin();
    for (int i = 0; i < 7; i++)
    {
        pinMode(segmentUnitPins[i], OUTPUT);
    }

    Wire.begin();
}

void loop()
{
    interval = potentiometer_interval();
    Serial.print("Intervalo de tiempo entre mediciones: ");
    Serial.print(interval);
    Serial.println("ms");

    Serial.print("Temperatura del NTC: ");
    Serial.println(ntc_sensor());
    Serial.print("Temperatura delDALLAS: ");
    Serial.println(ds18b20_sensor());

    average_temp = average_temperature(ntc_sensor(), ds18b20_sensor());

    Serial.print("Temperatura: ");
    Serial.print(average_temp);
    Serial.println(" C");

    unitDisplayNumber(getUnits(average_temp));
    tensDisplayNumber(getTens(average_temp));

    delay(interval);
}

float ntc_sensor()
{
    float celsius_of_ntc;
    analogValue = analogRead(A0);
    celsius_of_ntc = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

    return celsius_of_ntc;
}

float ds18b20_sensor()
{
    sensors.requestTemperatures();

    float celsius_of_ds18b20 = sensors.getTempCByIndex(0);

    return celsius_of_ds18b20;
}

// float average_temperature(float celsius_ntc, float celsius_ds18b20)
// {
//     float average_temp = (celsius_ntc + celsius_ds18b20) / 2;

//     return average_temp;
// }

float average_temperature(float celsius_ntc, float celsius_ds18b20)
{
    // Convertir los valores float a enteros escalados
    int16_t celsius_ntc_int = celsius_ntc * 100;
    int16_t celsius_ds18b20_int = celsius_ds18b20 * 100;
    int16_t average_temp_int;

    asm volatile(
        "movw r24, %1\n\t"        // Cargar celsius_ntc_int en r24:r25
        "movw r26, %2\n\t"        // Cargar celsius_ds18b20_int en r26:r27
        "add r24, r26\n\t"        // Sumar los bytes menos significativos
        "adc r25, r27\n\t"        // Sumar los bytes más significativos con acarreo
        "lsr r25\n\t"             // Desplazar un bit a la derecha r25
        "ror r24\n\t"             // Rotar un bit a la derecha r24
        "movw %0, r24\n\t"        // Guardar el resultado en average_temp_int
        : "=r" (average_temp_int)
        : "r" (celsius_ntc_int), "r" (celsius_ds18b20_int)
        : "r24", "r25", "r26", "r27"
    );

    // Convertir el resultado de vuelta a float
    return average_temp_int / 100.0;
}

int potentiometer_interval()
{
    int interval_of_potentiometer;
    valPotenciometer = analogRead(analogPinPotenciometer);
    interval_of_potentiometer = map(valPotenciometer, 0, 1023, 500, 5000);

    return interval_of_potentiometer;
}

void unitDisplayNumber(int num)
{
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(segmentUnitPins[i], HIGH);
    }

    for (int i = 0; i < 8; i++)
    {
        if (bitRead(unitsNumbers[num], i) == LOW)
        {
            digitalWrite(segmentUnitPins[7 - i], LOW);
        }
    }
}

void tensDisplayNumber(int num)
{
    Wire.beginTransmission(hw171Address);
    Wire.write(tensNumbers[num]);
    Wire.endTransmission();
}

// int getUnits(float numero)
// {
//     int entero = numero;
//     int unidades = entero % 10;
//     return unidades;
// }

int getUnits(float numero)
{
    int entero = (int)numero;
    int unidades;

    asm volatile(
        "ldi r18, 10\n\t"      // Load immediate value 10 into register r18
        "movw r24, %1\n\t"     // Move the value of entero into r24:r25
        "1: subi r24, 10\n\t"  // Subtract 10 from r24
        "brcs 2f\n\t"          // If carry set, branch to label 2
        "rjmp 1b\n\t"          // Jump back to label 1
        "2: subi r24, -10\n\t" // Add 10 to r24 (undo last subtraction)
        "mov %0, r24\n\t"      // Move the final result (units) to the output variable
        : "=r"(unidades)       // Output operands
        : "r"(entero)          // Input operands
        : "r18", "r24", "r25"  // Clobbers
    );

    return unidades;
}

// int getTens(float numero)
// {
//     int entero = numero;
//     int decenas = (entero / 10) % 10;
//     return decenas;
// }

int getTens(float numero)
{
    int entero = (int)numero;
    int decenas;

    asm volatile(
        // Dividir por 10
        "ldi r18, 10\n\t"  // Load immediate value 10 into register r18
        "clr r25\n\t"      // Clear register r25 (high byte of quotient)
        "movw r24, %1\n\t" // Move the value of entero into r24:r25
        "divloop:\n\t"
        "cp r24, r18\n\t"  // Compare r24 with 10
        "brcs enddiv\n\t"  // If carry set, branch to enddiv
        "subi r24, 10\n\t" // Subtract 10 from r24
        "inc r25\n\t"      // Increment the quotient
        "rjmp divloop\n\t" // Repeat the loop
        "enddiv:\n\t"
        // Obtener el dígito de las decenas
        "mov r24, r25\n\t"    // Move the quotient to r24
        "andi r24, 0x0F\n\t"  // Mask the lower nibble to get the remainder
        "mov %0, r24\n\t"     // Move the final result (tens) to the output variable
        : "=r"(decenas)       // Output operands
        : "r"(entero)         // Input operands
        : "r18", "r24", "r25" // Clobbers
    );

    return decenas;
}
