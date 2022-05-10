/**
 * @file       TinySat.c
 * @author     Diego Anestor Coutinho
 * @copyright  Copyright (c) 2022 Inatel Cubesat Design Team
 * @date       May 2022
 * @license    MIT
 *
 * Contact: diego.anestor@gec.inatel.br or diego.coutinho@inatel.br
 */

#include "TinySat.h"

void setup()
{
    // Configures the HC-SR04 sensor
    pinMode(D5, OUTPUT); // trigPin -> Output
    pinMode(D6, INPUT);  // echoPin -> Input

    // Initialyses the serial
    Serial.begin(115200);

    // Conects to the WIFI network
    conecta_wifi();

    // Makes a handshake with the server
    handshake("http://10.1.1.1:100");
}

void loop()
{   
    // Reads the data from the sensor
    double dados = echo();

    // Sends the data to the server
    Serial.println(envia_dados(dados));

    // Prints the data
    Serial.println(dados);

    // Delay to avoid overload
    delay(250);
}
