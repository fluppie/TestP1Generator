/**
 * Smart DSMR P1 Splitter.
 *
 * This software is licensed under The GNU GPLv3 License.
 *
 * Copyright (c) 2022-2023 Jacco Bezemer <jacco.bezemer@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <HardwareSerial.h>
#include <driver/adc.h>
#include <WiFi.h>
#include <dsmr.h>

#define P1_SERIAL_RX  18  // P1 -> UART1 which is attached to Smartmeter P1 TX pin
#define P1_DR         2   // P1 Data request
#define P2_SERIAL_TX  17  // P2 -> UART2 which, in my case, is attached to HomeWizard P1 RX pin
#define P2_DR         35  // P2 Data request
#define P3_SERIAL_TX  1   // P3 -> UART0 which, in my case, is attached to Alfen Loadbalancer RX pin 
#define P3_DR         34  // P3 Data request
#define P1_LED        14  // lights up when a telegram is received and stored
#define P2_LED        27  // lights up when P2 requests data
#define P3_LED        5  // lights up when P3 requests data

// LED stuf
const int dutyCycle = 950;  // dim the LEDs with this duty cycle
const int PWMFreq = 5000;   // 5 KHz
const int PWMResolution = 10;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWMResolution) - 1);
const int pwmP1led = 0;
const int pwmP2led = 1;
const int pwmP3led = 2;
const int blinkTime = 100;            // must be less than telegram interval!
unsigned long time_now = 0;

//#define LED_FOLLOW_DR               // uncomment this if you want the Data Request Led's follow the Data Request signal instead of the telegram transmit 
//#define USE_MONITOR                 // uncomment this if you want to use one of the UARTs to monitor during development

//void disableWiFi();

P1Reader reader(&Serial1,P1_DR);

void setup() {
  
  //disableWiFi();                        // WiFi is not needed so disable it and draw less power (possibly, maybe this is not needed)
#ifdef USE_MONITOR
  Serial.begin(115200);                 //  UART0, now used for monitoring, later for Alfen Load Balancer
  Serial.println("Smart Splitter for DSMR P1, Copyright 2022-2023, Jacco Bezemer");
#endif
  pinMode(P2_DR,INPUT);
  pinMode(P3_DR,INPUT);

  ledcSetup(pwmP1led, PWMFreq, PWMResolution);
  ledcSetup(pwmP2led, PWMFreq, PWMResolution);
  ledcSetup(pwmP3led, PWMFreq, PWMResolution);
  // Attach the LED PWM Channel to the GPIO Pin
  ledcAttachPin(P1_LED, pwmP1led);
  ledcAttachPin(P2_LED, pwmP2led);
  ledcAttachPin(P3_LED, pwmP3led);
  // Turn LEDs on at max brightness
  ledcWrite(pwmP1led,0);
  ledcWrite(pwmP2led,0);
  ledcWrite(pwmP3led,0);
  delay(2000);
  // turn the LEDs off
  ledcWrite(pwmP1led,MAX_DUTY_CYCLE);
  ledcWrite(pwmP2led,MAX_DUTY_CYCLE);
  ledcWrite(pwmP3led,MAX_DUTY_CYCLE);

  Serial1.begin(115200,SERIAL_8N1,P1_SERIAL_RX,-1,false);  //  Telgram receive port, UART1 attached to DSMR P1 port receiving telegrams at maximum interval, no send pin nessesary
  Serial2.begin(115200,SERIAL_8N1,-1,P2_SERIAL_TX,false);  //  UART2 attached to HomeWizard send telegrams, on request, no receive pin nessesary
#ifndef USE_MONITOR 
  Serial.begin(115200,SERIAL_8N1,-1,P3_SERIAL_TX,false);   //  UART0 attached to Alfen Load Balancer to send telegrams on request, no receive pin nessesary
#endif  

}

void loop() {

  time_now = millis();

#ifdef LED_FOLLOW_DR
  if (digitalRead(P2_DR) == LOW) ledcWrite(pwmP2led,dutyCycle);
  else ledcWrite(pwmP2led,MAX_DUTY_CYCLE);

  if (digitalRead(P3_DR) == LOW) ledcWrite(pwmP3led,dutyCycle);
  else ledcWrite(pwmP3led,MAX_DUTY_CYCLE); 
#endif

//            Source: https://github.com/nielsbasjes/dsmr-tools/blob/master/dsmr-simulator/src/main/java/nl/basjes/dsmr/simulator/P1Simulator.java
//            https://github.com/nielsbasjes/dsmr-tools
//            String record =
//                "/ISk5\\2MT382-1000 FAKE\r\n" +
//                "\r\n" +
//                "1-3:0.2.8(50)\r\n" +                 // DSMR Version
//                "0-0:1.0.0("+nowString+")\r\n" +      // Timestamp Gecodeerd volgens YY/MM/DD/HH/MM/SS     S = zomertijd, W = wintertijd
//                "0-0:96.1.1("+electricEquipmentId+")\r\n" + // Equipment Id
//                "1-0:1.8.1("+ String.format("%09.3f",  800 + (100. * sin1))+"*kWh)\r\n" +
//                "1-0:1.8.2("+ String.format("%09.3f",  800 - (100. * sin2))+"*kWh)\r\n" +
//                "1-0:2.8.1("+ String.format("%09.3f", 1000 + (100. * sin3))+"*kWh)\r\n" +
//                "1-0:2.8.2("+ String.format("%09.3f", 1000 - (100. * sin4))+"*kWh)\r\n" +
//                "0-0:96.14.0(0002)\r\n" +
//                "1-0:1.7.0("+String.format("%05.3f", 1.0 + (0.400 * sin1))+"*kW)\r\n" +
//                "1-0:2.7.0("+String.format("%05.3f", 1.0 - (0.400 * sin2))+"*kW)\r\n" +
//                "0-0:96.7.21(00004)\r\n" +
//                "0-0:96.7.9(00002)\r\n" +
//                "1-0:99.97.0(2)(0-0:96.7.19)(101208152415W)(0000000240*s)(101208151004W)(0000000301*s)\r\n" +
//                "1-0:32.32.0(00002)\r\n" +
//                "1-0:52.32.0(00001)\r\n" +
//                "1-0:72.32.0(00000)\r\n" +
//                "1-0:32.36.0(00000)\r\n" +
//                "1-0:52.36.0(00003)\r\n" +
//                "1-0:72.36.0(00000)\r\n" +
//                "0-0:96.13.0(44534D522073696D756C61746F722063726561746564206279204E69656C73204261736A65732E20536565" +
//                        "2068747470733A2F2F64736D722E6261736A65732E6E6C20666F72206D6F726520696E666F726D6174696F6E2E)\r\n" +
//                "1-0:32.7.0("+String.format("%04.1f", 221. + (3 * sin1))+"*V)\r\n" +
//                "1-0:52.7.0("+String.format("%04.1f", 222. - (3 * sin2))+"*V)\r\n" +
//                "1-0:72.7.0("+String.format("%04.1f", 223. + (6 * sin3))+"*V)\r\n" +
//                "1-0:31.7.0("+String.format("%03.0f",   5. + (2 * sin1))+"*A)\r\n" +
//                "1-0:51.7.0("+String.format("%03.0f",   6. - (2 * sin2))+"*A)\r\n" +
//                "1-0:71.7.0("+String.format("%03.0f",   7. + (3 * sin3))+"*A)\r\n" +
//                "1-0:21.7.0("+String.format("%05.3f", 1.0 + (0.400 * sin1))+"*kW)\r\n" +
//                "1-0:41.7.0("+String.format("%05.3f", 2.0 + (0.400 * sin2))+"*kW)\r\n" +
//                "1-0:61.7.0("+String.format("%05.3f", 3.0 + (0.400 * sin3))+"*kW)\r\n" +
//                "1-0:22.7.0("+String.format("%05.3f", 4.0 + (0.400 * sin1))+"*kW)\r\n" +
//                "1-0:42.7.0("+String.format("%05.3f", 5.0 + (0.400 * sin2))+"*kW)\r\n" +
//                "1-0:62.7.0("+String.format("%05.3f", 6.0 + (0.400 * sin3))+"*kW)\r\n" +
//                "0-1:24.1.0(003)\r\n" +
//                "0-1:96.1.0("+gasEquipmentId+")\r\n" +
//                "0-1:24.2.1(101209112500W)(12785.123*m3)\r\n" +
//                "!FFFF\r\n";
//
//            record =  CheckCRC.fixCrc(record);
//            System.out.print(record);
//            System.err.println("Wrote record for timestamp: " + nowString);

    // send the telegram to P2 (HomeWizard P1 meter)
    if (digitalRead(P2_DR) == LOW) {
    #ifndef LED_FOLLOW_DR
      ledcWrite(pwmP2led,dutyCycle);      // turn on the data request led for P2
    #endif  
      Serial2.println("/ISk5\\2MT382-1003");
      Serial2.println("");
      Serial2.println("0-0:96.1.1(5A424556303035313036383434393132)");
      Serial2.println("1-0:1.8.1(16722.627*kWh)");
      Serial2.println("1-0:1.8.2(19412.739*kWh)");
      Serial2.println("1-0:2.8.1(00859.681*kWh)");
      Serial2.println("1-0:2.8.2(01817.110*kWh)");
      Serial2.println("0-0:96.14.0(0002)");
      Serial2.println("1-0:1.7.0(0000.67*kW)");
      Serial2.println("1-0:2.7.0(0000.00*kW)");
      Serial2.println("0-0:17.0.0(0999.00*kW)");
      Serial2.println("0-0:96.3.10(1)");
      Serial2.println("0-0:96.13.1()");
      Serial2.println("0-0:96.13.0()");
      Serial2.println("0-2:24.1.0(3)");
      Serial2.println("0-2:96.1.0(3238303131303038333036343239303133)");
      Serial2.println("0-2:24.3.0(211123200000)(00)(60)(1)(0-2:24.2.1)(m3)");
      Serial2.println("(13376.292)");
      Serial2.println("0-2:24.4.0(1)");
      Serial2.println("!");
    #ifndef LED_FOLLOW_DR  
      ledcWrite(pwmP2led,MAX_DUTY_CYCLE);
    #endif  
    }

    // send the telegram to P3 (Alfen Load Balancer)
    if (digitalRead(P3_DR) == LOW) {
    //if (counter == 10) {
    #ifndef LED_FOLLOW_DR  
      ledcWrite(pwmP3led,dutyCycle);      // turn on the data request led for P3
    #endif
      Serial.println("/ISk5\\2MT382-1003");
      Serial.println("");
      Serial.println("0-0:96.1.1(5A424556303035313036383434393132)");
      Serial.println("1-0:1.8.1(16722.627*kWh)");
      Serial.println("1-0:1.8.2(19412.739*kWh)");
      Serial.println("1-0:2.8.1(00859.681*kWh)");
      Serial.println("1-0:2.8.2(01817.110*kWh)");
      Serial.println("0-0:96.14.0(0002)");
      Serial.println("1-0:1.7.0(0000.67*kW)");
      Serial.println("1-0:2.7.0(0000.00*kW)");
      Serial.println("0-0:17.0.0(0999.00*kW)");
      Serial.println("0-0:96.3.10(1)");
      Serial.println("0-0:96.13.1()");
      Serial.println("0-0:96.13.0()");
      Serial.println("0-2:24.1.0(3)");
      Serial.println("0-2:96.1.0(3238303131303038333036343239303133)");
      Serial.println("0-2:24.3.0(211123200000)(00)(60)(1)(0-2:24.2.1)(m3)");
      Serial.println("(13376.292)");
      Serial.println("0-2:24.4.0(1)");
      Serial.println("!");
    #ifndef LED_FOLLOW_DR  
      ledcWrite(pwmP3led,MAX_DUTY_CYCLE);
    #endif  
    }

    if (millis() < (time_now + blinkTime)) { 
      while(millis() < time_now + blinkTime){
          //wait approx. [period] ms
      }
    }
    ledcWrite(pwmP1led,MAX_DUTY_CYCLE);   // turn off the telegram stored led so we get notified of a new telegram
//  }

}

void disableWiFi() {
    WiFi.mode(WIFI_OFF); // Switch the WiFi radio off
    adc_power_off(); //deprecated, use adc_power_release(void) instead    
    //adc_power_release();  //causes a reboot, E (10) ADC: adc_power_release called, but s_adc_power_on_cnt == 0
}
