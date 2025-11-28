#ifndef SENSOR_H
#define SENSOR_H

namespace SensorPackage {
    // Clase padre
    class Sensor{
    protected:
        int pin; //para el pin en el que se conecte el sensor

    public:
        Sensor(int pinElegido) : pin(pinElegido){}

        // metodo abstracto que deben cumplir las clases hijas
        virtual float enviarMedicion() = 0;
    };

}

#endif
