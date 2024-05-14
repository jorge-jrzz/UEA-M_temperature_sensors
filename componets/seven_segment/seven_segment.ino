// Define los pines para cada segmento (a hasta g) y el pin de cátodo común
const int segmentPins[8] = {2, 3, 4, 5, 6, 7, 8}; // Pines para los segmentos a-g
const int commonCathodePin = 9; // Pin para el cátodo común

// Define los números a mostrar en el display de siete segmentos
const byte numbers[11] = {
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

void setup() {
  // Establece los pines de los segmentos como salidas
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  // Establece el pin de cátodo común como salida
  pinMode(commonCathodePin, OUTPUT);
}

void loop() {
  // Cuenta del 0 al 9
  for (int i = 0; i < 10; i++) 
  {
    displayNumber(i); // Llama a la función para mostrar el número
    delay(1000); // Muestra cada número durante 1 segundo
  }
}

// Función para mostrar un dígito en el display de siete segmentos
void displayNumber(int num) {
  // Apaga todos los segmentos
  for (int i = 0; i < 8; i++) {
    digitalWrite(segmentPins[i], HIGH); // Alto (apagado)
  }

  // Activa los segmentos basados en el número a mostrar
  for (int i = 0; i < 8; i++) {
    if (bitRead(numbers[num], i) == LOW) { // Comprueba cada bit del número
      digitalWrite(segmentPins[7-i], LOW); // Activa el segmento si el bit es bajo
    }
  }
}
