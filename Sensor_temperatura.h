#include <Arduino.h>
#include "Sensor.h"

namespace S_Temperatura {

    using namespace SensorPackage;

    // Clase derivada de Sensor, para el sensor de temperatura
    class SensorTempe: public Sensor{
    private:
    float valorTempe;

    public:
    SensorTempe(int pin) : Sensor(pin){} // A1 

    float enviarMedicion() override{
        valorTempe = analogRead(pin);
        float porcTempe = valorTempe*100; // valor en grados
        return porcTempe;
    }
    };

}