#include <Arduino.h>
#include "Sensor.h"

namespace Caliper {
    class Calibrador{
    private:
        float error = 0.05;
        float errorMax = 0.1;
        float errorMin = 0.0;

    public:
        Calibrador(){}
        bool calibrado(float valorReal, float valorMedido){
            if(error*(-1)*valorReal <= (valorReal-valorMedido) && (valorReal-valorMedido)<=error*valorReal){
                return true;
            }
            return false;
        }
        bool cambiarError(float newError){
            newError = newError/100;
            if(errorMin<newError && newError<errorMax){
                error = newError;
                return true;
            }
            return false;
        }
    };
}
