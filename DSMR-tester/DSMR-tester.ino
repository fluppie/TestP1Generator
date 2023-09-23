
#include <SoftwareSerial.h>
// AirM2M CORE ESP32C3

/*Example DSMR telegram*/
//const int ledPin = 13; // D5
#define ledPin                 13  // D5
#define ledPinDataRequest      12  // D4
#define P1_DataRequest         6  // P1 Data request

//float initEnergy = 123456.7;
//float initInjection = 123.4;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPinDataRequest, OUTPUT);
  pinMode(P1_DataRequest,INPUT);
	Serial.begin(115200);
  Serial.setDebugOutput(true);
}

void loop() {
//  initEnergy = initEnergy + 0.01;
//  initInjection = initInjection + 0.01; //random(0, 100)/10000.0;
//  String power = "1-0:1.7.0(" + String(1.3 + random(0, 1000)/1000.0) + "*kW)";
//  String energy = "1-0:1.8.1(" + String(initEnergy) + "*kWh)";
//  String injection = "1-0:2.8.1(" + String(initInjection) + "*kWh)";

if (digitalRead(P1_DataRequest) == LOW) {
  digitalWrite(ledPinDataRequest, HIGH);      // turn on the data request led for P1

  digitalWrite(ledPin, HIGH);
  // DSMR 4.0 is enough for my application
  Serial.println("/XMX5XMXCQA0000008879");
  Serial.println("");
  Serial.println("1-3:0.2.8(40)");
  Serial.println("0-0:1.0.0(000101010000W)");
  Serial.println("0-0:96.1.1(4530303030303030303030303030303030)");
  Serial.println("1-0:1.8.1(000000.000*kWh)");
  Serial.println("1-0:2.8.1(000000.000*kWh)");
  Serial.println("1-0:1.8.2(000000.000*kWh)");
  Serial.println("1-0:2.8.2(000000.000*kWh)");
  Serial.println("0-0:96.14.0(0001)");
  Serial.println("1-0:1.7.0(00.000*kW)");
  Serial.println("1-0:2.7.0(00.000*kW)");
  Serial.println("0-0:17.0.0(000.0*kW)");
  Serial.println("0-0:96.3.10(1)");
  Serial.println("0-0:96.7.21(00000)");
  Serial.println("0-0:96.7.9(00000)");
  Serial.println("1-0:99.97.0(0)(0-0:96.7.19)");
  Serial.println("1-0:32.32.0(00000)");
  Serial.println("1-0:52.32.0(00000)");
  Serial.println("1-0:72.32.0(00000)");
  Serial.println("1-0:32.36.0(00000)");
  Serial.println("1-0:52.36.0(00000)");
  Serial.println("1-0:72.36.0(00000)");
  Serial.println("0-0:96.13.1(XMX_P1CS_V05)");
  Serial.println("0-0:96.13.0()");
  Serial.println("1-0:31.7.0(024*A)");
  Serial.println("1-0:51.7.0(024*A)");
  Serial.println("1-0:71.7.0(024*A)");
  Serial.println("1-0:21.7.0(00.000*kW)");
  Serial.println("1-0:41.7.0(00.000*kW)");
  Serial.println("1-0:61.7.0(00.000*kW)");
  Serial.println("1-0:22.7.0(00.000*kW)");
  Serial.println("1-0:42.7.0(00.000*kW)");
  Serial.println("1-0:62.7.0(00.000*kW)");
  Serial.println("!19D6");
  
//  Serial.println("/FLU5\253769484_A0");
//  Serial.println("");
//  Serial.println("0-0:96.1.4(50217)"); //Belgian P1 version information
//  Serial.println("1-3:0.2.8(50)"); //Generic DSMR version information
//  Serial.println("0-0:1.0.0(101209113020W)"); //Meter timestamp
//  Serial.println("0-0:96.1.1(4B384547303034303436333935353037)"); //Equipment ID
//  //Serial.println(energy); //Consumed energy tariff 1
//  Serial.println("1-0:1.8.2(123456.789*kWh)"); //Consumed energy tariff 2
//  //Serial.println(injection); //Injected energy tariff 1
//  Serial.println("1-0:2.8.2(123456.789*kWh)"); //Injected energy tariff 2
//  Serial.println("0-0:96.14.0(0002)"); //Active tariff period
//  //Serial.println(power); //Consumed active power
//  Serial.println("1-0:2.7.0(00.000*kW)"); //Injected active power
//  Serial.println("1-0:1.4.0((02.351*kW)"); //Current average demand
//  Serial.println("1-0:1.6.0(200509134558S)(02.589*kW)"); //Maximum demand of the running month
//  Serial.println("0-0:98.1.0(3)(1-0:1.6.0)(1-0:1.6.0)(200501000000S)(200423192538S)(03.695*kW)(200401000000S)(200305122139S)(05.980*kW)(200301000000S)(200210035421W)(04.318*kW)"); //Maximum demand of the last 13 months
//  Serial.println("0-0:96.13.0(303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F303132333435363738393A3B3C3D3E3F)"); //Text message
//  Serial.println("1-0:32.7.0(220.1*V)"); //Instantaneous voltage L1
//  Serial.println("1-0:52.7.0(220.2*V)"); //Instantaneous voltage L2
//  Serial.println("1-0:72.7.0(220.3*V)"); //Instantaneous voltage L3
//  Serial.println("1-0:31.7.0(001*A)"); //Instantaneous current L1
//  Serial.println("1-0:51.7.0(002*A)"); //Instantaneous current L2
//  Serial.println("1-0:71.7.0(003*A)"); //Instantaneous current L3
//  Serial.println("1-0:21.7.0(01.111*kW)"); //Instantaneous active power consumption L1
//  Serial.println("1-0:41.7.0(02.222*kW)"); //Instantaneous active power consumption L2
//  Serial.println("1-0:61.7.0(03.333*kW)"); //Instantaneous active power consumption L3
//  Serial.println("1-0:22.7.0(04.444*kW)"); //Instantaneous active power injection L1
//  Serial.println("1-0:42.7.0(05.555*kW)"); //Instantaneous active power injection L2
//  Serial.println("1-0:62.7.0(06.666*kW)"); //Instantaneous active power injection L3
//  Serial.println("0-1:24.1.0(003)"); //Mbus channel 1 device type (003 = gas meter)
//  Serial.println("0-1:96.1.0(3232323241424344313233343536373839)"); //Mbus channel 1 device ID
//  Serial.println("0-1:96.1.1(37464C4F32313139303333373333)"); //Mbus channel 1 device ID according DIN 43863-5
//  //Serial.println("0-1:24.2.1(101209112500W)(12785.123*m3)"); //Last 5-minute water meter value
//  Serial.println("0-1:24.2.3(200512134558S)(00112.384*m3"); //Last 5-minute non-temperature compensated gas meter value
//  Serial.println("0-2:24.1.0(007)"); //Mbus channel 1 device type (007 = water meter)
//  Serial.println("0-1:96.1.0(3232323241424344313233343536373839)"); //Mbus channel 1 device ID
//  Serial.println("0-1:96.1.1(37464C4F32313139303333373333)"); //Mbus channel 2 device ID according DIN 43863-5
//  Serial.println("0-2:24.2.1(200512134558S)(00112.384*m3"); //Last 5-minute water meter value
//  Serial.println("!XXX");

  digitalWrite(ledPin, LOW);
  digitalWrite(ledPinDataRequest, LOW);      // turn off the data request led for P1
  }
  delay(100);
}
