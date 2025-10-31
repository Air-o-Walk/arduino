// -*- mode: c++ -*-

#ifndef MEDIDOR_H_INCLUIDO
#define MEDIDOR_H_INCLUIDO

// ------------------------------------------------------
// ------------------------------------------------------
class Medidor {

  // .....................................................
  // .....................................................
private:

  //Definir pines
  enum PINES{
    gasPin = 5,
    refPin = 28,
    tempPin = 29,
  }; // enum

  //Funcion para normalizar valores
  // .....................................................
  // valor:float --> normalizar() --> float
  // .....................................................
  float normalizar(float valor){
    float respuesta = (valor*3.3)/4096;
    return respuesta;
  } // ()

  //Funcion para calcular la temperatura detectada por el sensor con la ecuacion del datasheet
  // .....................................................
  // valor:float --> calcularTemperatura() --> float
  // .....................................................
  float calcularTemperatura(float valor){
    float temperatura = valor * 53    ;
    /*if (temperatura>20) {
      float correccion=(temperatura-20)*0.3;
      temperatura = temperatura-correccion;
    }*/

    return temperatura;
  } // ()

  //Funcion para usar un promedio de medidas en ves de una sola, asi evitando ruido en la medida
  // .....................................................
  // valor:float --> calcularTemperatura() --> float
  // .....................................................
  float leerPromedio(int pin, int muestras = 200) {
    float suma = 0;
    for (int i = 0; i < muestras; i++) {
      suma += analogRead(pin);
      delay(2); // pequeña pausa opcional
    }
    return suma / muestras;
  }

public:

  // .....................................................
  // constructor
  // .....................................................
  Medidor(  ) {
  } // ()

  // .....................................................
  // .....................................................
  void iniciarMedidor() {
    pinMode(PINES::gasPin, INPUT);
    pinMode(PINES::refPin, INPUT);
    pinMode(PINES::tempPin, INPUT);
    } // ()

  // .....................................................
  // .....................................................

  // .....................................................
  //  medirOzono() --> float
  // .....................................................
  float medirOzono() {
    //Guardar la lectura analogica en una variable
    float vGas = leerPromedio(gasPin);
    float vRef = leerPromedio(refPin);

    //Normalizar la lectura anterior con la llamada a la funcion normalizar
    float vGasN = normalizar(vGas);
    float vRefN = normalizar(vRef);

    //Calculamos vMedida
    float vMedida = vGasN-vRefN;

    Serial.println(vMedida);

    //Calculamos la concentracion
    float concentracion = vMedida/(-41.26*499*0.000001);

    if (concentracion < 0){
      concentracion = concentracion * (-1);
    }

    float calibracion = (concentracion + 0.17505) / 1.14302;
 
    return calibracion;
  } // ()


  float medirTemperatura() {
    //Guardar la lectura analogica en una variable
    float vTemp = analogRead(tempPin);

    //Normalizar la lectura anterior con la llamada a la funcion normalizar
    float vTempN = normalizar(vTemp);

    //Calcular la temperatura detectada por el sensor con la ecuacion del datasheet
    float temperatura = calcularTemperatura(vTempN);

    return temperatura;
  } // ()
	
}; // class

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
#endif
