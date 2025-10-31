// -*- mode: c++ -*-

// --------------------------------------------------------------
// Jordi Bataller i Mascarell
// --------------------------------------------------------------

#ifndef PUBLICADOR_H_INCLUIDO
#define PUBLICADOR_H_INCLUIDO

// --------------------------------------------------------------
// --------------------------------------------------------------
class Publicador {

  // ............................................................
  // ............................................................
private:

  // Cambie el ibeacon porque al tener characteres alfanuméricos
  // no se creaba bien el beacon e imposibilitaba el envio del ibeacon
  	
  // UUID para iBeacon (16 bytes)
	uint8_t beaconUUID[16] = {
    0x45, 0x50, 0x53, 0x47,
    0x2D, 0x47, 0x54, 0x49,
    0x2D, 0x50, 0x52, 0x4F,
    0x59, 0x2D, 0x33, 0x41
	};

  // ............................................................
  // ............................................................
public:
  EmisoraBLE laEmisora {
	"GTI", //  nombre emisora
	  0x004c, // fabricanteID (Apple)
	  4 // txPower
	  };
  
  const int RSSI = -53; // por poner algo, de momento no lo uso

  // ............................................................
  // ............................................................
public:

  // ............................................................
  // ............................................................
  enum MedicionesID  {
	CO2 = 11,
	TEMPERATURA = 12,
	RUIDO = 13	
  };

  // ............................................................
  // ............................................................
  Publicador( ) {
	// ATENCION: no hacerlo aquí. (*this).laEmisora.encenderEmisora();
	// Pondremos un método para llamarlo desde el setup() más tarde
  } // ()

  // ............................................................
  // ............................................................
  void encenderEmisora() {
	(*this).laEmisora.encenderEmisora();
  } // ()

  // ............................................................
  // ............................................................
  void publicarCO2( int16_t valorCO2, uint8_t contador,
					long tiempoEspera ) {

	//
	// 1. empezamos anuncio
	//
	uint16_t major = (MedicionesID::CO2 << 8) + contador;
	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
											major,
											valorCO2, // minor
											(*this).RSSI // rssi
									);

	
	Globales::elPuerto.escribir( "   publicarCO2(): valor=" );
	Globales::elPuerto.escribir( valorCO2 );
	Globales::elPuerto.escribir( "   contador=" );
	Globales::elPuerto.escribir( contador );
	Globales::elPuerto.escribir( "   todo="  );
	Globales::elPuerto.escribir( major );
	Globales::elPuerto.escribir( "\n" );
	

	//
	// 2. esperamos el tiempo que nos digan
	//
	esperar( tiempoEspera );

	//
	// 3. paramos anuncio
	//
	(*this).laEmisora.detenerAnuncio();
  } // ()

  // ............................................................
  // ............................................................
  void publicarTemperatura( int16_t valorTemperatura,
							uint8_t contador, long tiempoEspera ) {

	uint16_t major = (MedicionesID::TEMPERATURA << 8) + contador;
	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
											major,
											valorTemperatura, // minor
											(*this).RSSI // rssi
									);
	esperar( tiempoEspera );

	Globales::elPuerto.escribir( "   publicarTemperatura(): valor=" );
	Globales::elPuerto.escribir( valorTemperatura );
	Globales::elPuerto.escribir( "   contador=" );
	Globales::elPuerto.escribir( contador );
	Globales::elPuerto.escribir( "   todo="  );
	Globales::elPuerto.escribir( major );
	Globales::elPuerto.escribir( "\n" );

	(*this).laEmisora.detenerAnuncio();
  } // ()



	// ............................................................
  // float valorOzono, float valorTemperatura, --> publicarMedicion() 
  //  uint8_t contador, long tiempoEspera 
  // ............................................................
  void publicarMedicion( float valorOzono, float valorTemperatura , uint8_t contador, long tiempoEspera ) {

		//Convierto el valorOzono y valorTemperatura de float a uint16
		//Multiplico por 100 para poder guardar dos decimales.
		//Ejemplo: valorOzono = 232.51 (float) ---> 23251()
		uint16_t major = ((uint16_t)(valorOzono * 100));
		uint16_t minor = ((uint16_t)(valorTemperatura * 100));

		(*this).laEmisora.setTxPower((int8_t)contador);
		
		(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
												major,
												minor, // 
												contador //(*this).RSSI  // txPower
										);

		Globales::elPuerto.escribir( "   publicarMedicion():" );                
		Globales::elPuerto.escribir( "   Ozono (major)=" );
		Globales::elPuerto.escribir( valorOzono );
		Globales::elPuerto.escribir( "   Temperatura (minor)=" );
		Globales::elPuerto.escribir( valorTemperatura );
		Globales::elPuerto.escribir( "   Contador ="  );
		Globales::elPuerto.escribir( contador );
		Globales::elPuerto.escribir( "\n" );
		//
		// 2. esperamos el tiempo que nos digan
		//
		esperar( tiempoEspera );
		//
		// 3. paramos anuncio
		//
		(*this).laEmisora.detenerAnuncio();
  } // ()
	
}; // class





// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
#endif
