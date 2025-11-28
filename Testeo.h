using namespace Caliper;

namespace Testin{
    class TestBase {
    protected:
        int testsPasados = 0;
        int testsFallados = 0;

        void assertEq(bool condicion, const char* nombreTest) { // ve si el testeo fue PASS o FAIL
            if (condicion) {
                Serial.print("[PASS] ");
                Serial.println(nombreTest); 
                testsPasados++;
            } else {
                Serial.print("[FAIL] ");
                Serial.println(nombreTest); 
                testsFallados++;
            }
        }

        void assertFloat(float esperado, float actual, float margen, const char* nombreTest) { 
            bool condicion = abs(esperado - actual) <= margen; // condicion a pasar
            if (condicion) {
                Serial.print("[PASS] ");
                Serial.println(nombreTest);
                testsPasados++;
            } else {
                Serial.print("[FAIL] ");
                Serial.print(nombreTest);
                Serial.print(" - Esperado: ");
                Serial.print(esperado);
                Serial.print(", Actual: ");
                Serial.println(actual);
                testsFallados++;
            }
        }

        void mostrarResultados() {
            Serial.println("\n=== Resultados ===");
            Serial.print("Tests pasados: ");
            Serial.println(testsPasados);
            Serial.print("Tests fallados: ");
            Serial.println(testsFallados);
            Serial.print("Total: ");
            Serial.println(testsPasados + testsFallados);
        }

    public:
        TestBase() {}
    };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // TESTS DE CALIBRADOR
    class CalibradorTest : public TestBase {
    public:
        CalibradorTest() {}

        void calibradoTest() {
            Calibrador c;
            
            // Test 1: Valores dentro del margen de error (5%)
            assertEq(c.calibrado(100.0, 103.0), 
                    "Calibrado con valor dentro del margen positivo");
            
            // Test 2: Valores dentro del margen negativo
            assertEq(c.calibrado(100.0, 97.0), 
                    "Calibrado con valor dentro del margen negativo");
            
            // Test 3: Valor exactamente igual
            assertEq(c.calibrado(100.0, 100.0), 
                    "Calibrado con valor exacto");
            
            // Test 4: Valor fuera del margen positivo
            assertEq(!c.calibrado(100.0, 106.0), 
                    "Valor fuera del margen positivo detectado");
            
            // Test 5: Valor fuera del margen negativo
            assertEq(!c.calibrado(100.0, 94.0), 
                    "Valor fuera del margen negativo detectado");
        }

        void cambiarErrorTest() {
            Calibrador c;
            
            // Test 6: Cambiar a error válido 
            assertEq(c.cambiarError(7.0), 
                    "Cambiar error a valor valido");
            
            // Test 7: Intentar cambiar a error muy grande
            assertEq(!c.cambiarError(15.0), 
                    "Rechazar error mayor al maximo");
            
            // Test 8: Intentar cambiar a error muy pequeño
            assertEq(!c.cambiarError(-1.0), 
                    "Rechazar error negativo");
        }

        void ejecutarTodos() {
            Serial.println("\n=== Ejecutando Tests de Calibrador ===");
            calibradoTest();
            cambiarErrorTest();
            mostrarResultados();
        }
    };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MOCK PARA SENSORES
    class MockAnalogRead {
    public:
        static float valorSimulado;
        
        static void setValor(float valor) {
            valorSimulado = valor;
        }
    };
    
    float MockAnalogRead::valorSimulado = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MOCK SENSOR DE HUMEDAD
    class SensorHumeMock : public SensorPackage::Sensor {
    private:
        float valorHume;
        
    public:
        SensorHumeMock(int pin) : Sensor(pin) {}
        
        float enviarMedicion() override {
            valorHume = MockAnalogRead::valorSimulado;
            float porcHume = (1023 - valorHume) * 100 / 1023;
            return porcHume;
        }
    };
    
    // TESTS DE SENSOR DE HUMEDAD
    class SensorHumeTest : public TestBase {
    public:
        SensorHumeTest() {}
        
        void testMedicionSeco() {
            SensorHumeMock sensor(A0);
            MockAnalogRead::setValor(1023);
            float resultado = sensor.enviarMedicion();
            assertFloat(0.0, resultado, 0.1, "Sensor completamente seco (0%)");
        }
        
        void testMedicionMojado() {
            SensorHumeMock sensor(A0);
            MockAnalogRead::setValor(0);
            float resultado = sensor.enviarMedicion();
            assertFloat(100.0, resultado, 0.1, "Sensor completamente mojado (100%)");
        }
        
        void testMedicionMedia() {
            SensorHumeMock sensor(A0);
            MockAnalogRead::setValor(511.5);
            float resultado = sensor.enviarMedicion();
            assertFloat(50.0, resultado, 1.0, "Sensor con humedad media (50%)");
        }
        
        void testMedicionBaja() {
            SensorHumeMock sensor(A0);
            MockAnalogRead::setValor(767.25);
            float resultado = sensor.enviarMedicion();
            assertFloat(25.0, resultado, 1.0, "Sensor con humedad baja (25%)");
        }
        
        void testMedicionAlta() {
            SensorHumeMock sensor(A0);
            MockAnalogRead::setValor(255.75);
            float resultado = sensor.enviarMedicion();
            assertFloat(75.0, resultado, 1.0, "Sensor con humedad alta (75%)");
        }
        
        void ejecutarTodos() {
            Serial.println("\n=== Ejecutando Tests de SensorHume ===");
            testMedicionSeco();
            testMedicionMojado();
            testMedicionMedia();
            testMedicionBaja();
            testMedicionAlta();
            mostrarResultados();
        }
    };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MOCK SENSOR DE TEMPERATURA
    class SensorTempeMock : public SensorPackage::Sensor {
    private:
        float valorTempe;
        
    public:
        SensorTempeMock(int pin) : Sensor(pin) {}
        
        float enviarMedicion() override {
            valorTempe = MockAnalogRead::valorSimulado;
            float porcTempe = valorTempe * 100; // valor en grados
            return porcTempe;
        }
    };
    
    // TESTS DE SENSOR DE TEMPERATURA
    class SensorTempeTest : public TestBase {
    public:
        SensorTempeTest() {}
        
        void testTemperaturaCero() {
            SensorTempeMock sensor(A1);
            MockAnalogRead::setValor(0);
            float resultado = sensor.enviarMedicion();
            assertFloat(0.0, resultado, 0.1, "Temperatura 0 grados");
        }
        
        void testTemperaturaAmbiente() {
            SensorTempeMock sensor(A1);
            MockAnalogRead::setValor(0.25); // 25 grados
            float resultado = sensor.enviarMedicion();
            assertFloat(25.0, resultado, 0.5, "Temperatura ambiente (25 grados)");
        }
        
        void testTemperaturaAlta() {
            SensorTempeMock sensor(A1);
            MockAnalogRead::setValor(0.50); // 50 grados
            float resultado = sensor.enviarMedicion();
            assertFloat(50.0, resultado, 0.5, "Temperatura alta (50 grados)");
        }
        
        void testTemperaturaMaxima() {
            SensorTempeMock sensor(A1);
            MockAnalogRead::setValor(1.0); // 100 grados
            float resultado = sensor.enviarMedicion();
            assertFloat(100.0, resultado, 0.5, "Temperatura maxima (100 grados)");
        }
        
        void testTemperaturaBaja() {
            SensorTempeMock sensor(A1);
            MockAnalogRead::setValor(0.10); // 10 grados
            float resultado = sensor.enviarMedicion();
            assertFloat(10.0, resultado, 0.5, "Temperatura baja (10 grados)");
        }
        
        void ejecutarTodos() {
            Serial.println("\n=== Ejecutando Tests de SensorTempe ===");
            testTemperaturaCero();
            testTemperaturaAmbiente();
            testTemperaturaAlta();
            testTemperaturaMaxima();
            testTemperaturaBaja();
            mostrarResultados();
        }
    };
}
