#include "Calibrar_sensor.h"
#include "Sensor.h"
#include "Testeo.h"
#include "Sensor_humedad.h"
#include "Sensor_temperatura.h"

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ; // Esperar a que se conecte el puerto serial
    }
    
    delay(1000); 
    
    Testin::CalibradorTest testCal;
    testCal.ejecutarTodos(); // hace el testeo de la clase calibrador

    Testin::SensorHumeTest testShume;
    testShume.ejecutarTodos(); // hace el testeo de la clase sensorHume

    Testin::SensorTempeTest testStempe;
    testStempe.ejecutarTodos(); // hace el testeo de la clase sensorTEmpe
    
    Serial.println("\n=== Tests Completados ===");
}

void loop() {
  // put your main code here, to run repeatedly:

}
