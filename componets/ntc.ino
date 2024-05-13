byte NTCPin = A0;
#define SERIESRESISTOR 10000
#define NOMINAL_RESISTEACE 10000
#define NOMINAL_TEMPERATURE 25
#define BCOEFICIENT 3950

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    float ADCvalue;
    float Resistance;
    ADCvalue = analogRead(NTCPin);
    Serial.print("Analog: ");
    Serial.print(ADCvalue);
    Serial.print(" = ");
    Resistance = (1023 / ADCvalue) - 1;
    Resistance = SERIESRESISTOR / Resistance;
    Serial.print(Resistance);
    Serial.print("Ohm");
    // Serial.print("Analog: %f", ADCvalue);

    float steinhart;
    steinhart = Resistance / NOMINAL_RESISTEACE;
    steinhart = log(steinhart);
    steinhart /= BCOEFICIENT;
    steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
    steinhart = 1.0 / steinhart;
    steinhart -= 273.15;

    Serial.print(steinhart);
    Serial.println(" oC");
    delay(1000);
}