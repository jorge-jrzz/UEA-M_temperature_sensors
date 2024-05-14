// Definición del pin analógico al que está conectado el potenciómetro
int analogPin = 0;

// Variable para almacenar el valor leído del potenciómetro
int val = 0;

void setup()
{
    // Inicialización de la comunicación serial a 9600 baudios para la salida de datos
    Serial.begin(9600);
}

void loop()
{
    // Lectura del valor analógico del potenciómetro
    val = analogRead(analogPin);

    // Mostrar el valor leído por el monitor serial
    Serial.println(val);
}
