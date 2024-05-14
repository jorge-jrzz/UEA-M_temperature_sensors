// Definición de la constante BETA del sensor NTC
const float BETA = 3950;

// Variables para almacenar el valor analógico y la temperatura en grados Celsius
int analogValue;
float celsius;

void setup()
{
  // Inicialización de la comunicación serial para la salida de datos
  Serial.begin(9600);
}

void loop()
{
  // Lectura del valor analógico del pin A0, donde está conectado el sensor NTC
  analogValue = analogRead(A0);

  // Cálculo de la temperatura en grados Celsius utilizando la ecuación de Steinhart-Hart
  // Esta ecuación se basa en la variación de la resistencia del sensor con la temperatura
  // y la constante BETA que caracteriza el sensor
  celsius = 1 / (log(1 / (1023. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

  // Impresión de la temperatura medida por el monitor serial
  Serial.print("Temperatura: ");
  Serial.print(celsius);
  Serial.println(" C");

  // Retardo para la próxima lectura de temperatura
  delay(500);
}
