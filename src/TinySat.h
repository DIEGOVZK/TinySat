/**
 * @file       TinySat.c
 * @author     Diego Anestor Coutinho
 * @copyright  Copyright (c) 2022 Inatel Cubesat Design Team
 * @date       May 2022
 * @license    MIT
 *
 * Contact: diego.anestor@gec.inatel.br or diego.coutinho@inatel.br
 */


/* ---------------------------------- SIMPLE UI ---------------------------------- */

void blink_FAST()
{
    digitalWrite(BUILTIN_LED, LOW);
    delay(100);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(100);
}

void blink_SLOW()
{
    digitalWrite(BUILTIN_LED, LOW);
    delay(100);
    digitalWrite(BUILTIN_LED, HIGH);
}

void blink_()
{
    digitalWrite(BUILTIN_LED, LOW);
    delay(25);
    digitalWrite(BUILTIN_LED, HIGH);
}


/* ---------------------------------- NETWORKING ---------------------------------- */

// Dependencies:
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>       
#include <WiFiClient.h>

// Maximum number of attempts to connect to the WIFI network
const int maxTry = 60;

// Name of the domain to connect to
String serverName = "http://10.1.1.1:100";

// Information about the WIFI router
const char *password = "password";
const char *ssid = "ssid";

// Defines the HTTP client object
WiFiClient client;
HTTPClient http;

// Initializes the WIFI network
bool conecta_wifi()
{
    // Turns off the WIFI network
    WiFi.disconnect();
    delay(100);

    // Turns on the WIFI network
    pinMode(BUILTIN_LED, OUTPUT);
    WiFi.begin(ssid, password);

    // Tries to connect to the WIFI network
    unsigned char tentativas = 0;
    while (tentativas <= maxTry)
    {
        // When the connection is established
        // Jumps to the next "done" statement
        if (WiFi.status() == WL_CONNECTED)
            goto done;

        delay(1000);
        tentativas++;
        Serial.print("`");
    }

    // If the connection is not established
    while (1)
    {
        blink_FAST();
    }

    done:
    Serial.print("\nGot IP: ");
    Serial.println(WiFi.localIP());
    return 0;
}

// Connects to the server and makes a handshake
bool handshake(String ip = serverName)
{
    // Defines the server IP
    String serverName = String(ip);

    // Configures the HTTP client
    http.begin(client, serverName);

    // Sends a GET request to the server
    int httpCode = http.GET();
    Serial.print("[HTTP] GET >>\n");

    // If the is an error or the server did not respond in time 
    if ((httpCode != HTTP_CODE_OK) || (String("200") != http.getString()))
    {
        // Stops and reuturns an error
        blink_SLOW();
        return 1;
    }

    // Ends the connection
    http.end();
    blink_();
    return 0;
}

// Sends the data to the server
bool envia_dados(float data = -1)
{
    // Configures the HTTP client
    http.begin(client, serverName);

    // Prepares the package header
    http.addHeader("Content-Type", "application/json");

    // Sends a POST request to the server with the data
    int httpCode = http.POST(String(data, 3));
    Serial.print("[HTTP] POST >> \n");

    // If the is an error or the server did not respond in time 
    if ((httpCode != HTTP_CODE_OK) || (String("200") != http.getString()))
    {
        // Stops and reuturns an error
        blink_SLOW();
        return 1;
    }

    // Ends the connection
    http.end();
    blink_();
    return 0;
}


/* ---------------------------------- SENSORS ---------------------------------- */

// Speed of sound in cm/uS
#define SOUND_VELOCITY 0.034300

// Reads the data from the sensor and returns it
double echo(char trigPin = D5,  char echoPin = D6)
{
    // Auxiliary variable
    double duration = 0;

    for (int i = 0; i < 25; i++)
    {
        // Sets the trigger pin to LOW
        digitalWrite(trigPin, LOW);
        delayMicroseconds(5);

        // Send a 10us pulse to the trigger pin
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // Reads the response time in the echo pin
        duration += pulseIn(echoPin, HIGH) / 25.0;
    }

    // Calculates the distance
    return (duration * SOUND_VELOCITY / 2);
}
