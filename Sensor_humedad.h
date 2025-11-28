#include <Arduino.h>
#include "Sensor.h"

namespace S_Humedad {
    const float Max = 1023;
    using namespace SensorPackage;

    // Clase derivada de Sensor
    class SensorHume: public Sensor{
        private:
        float valorHume;

        public:
        SensorHume(int pin) : Sensor(pin){} // A0 

        float enviarMedicion() override{
            valorHume = analogRead(pin);
            float porcHume = (Max-valorHume)*100/Max; // valor en porcentaje
            return porcHume;
        }
    };

}