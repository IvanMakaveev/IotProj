// Temperature sensor
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 environmentSensor;

// Distance sensor
#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// LCD Display
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

// Color Strip
#include <NeoPixelBus.h>
#define Cred     RgbColor(255, 0, 0)
#define Cpink    RgbColor(255, 0, 128)
#define Clila    RgbColor(255, 0, 255)
#define Cviolet  RgbColor(128, 0, 255)
#define Cblue    RgbColor(0, 0, 255)
const uint16_t PixelCount = 16;
const uint8_t PixelPin = 2;
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

// SERVER SETUP
#include <WiFi.h>
const char* ssid = "IEM";
const char* password = "";
WiFiServer server(80);
String header;

void setup()
{
    Serial.begin(9600); // Console output
    while(!Serial) delay(1); // Serial connection wait 

    Wire.begin(16, 17); // Begin SDA SCL Wires

    environmentSensor.begin(0x76);  // Connect environment sensor

    // Begin LED strip
    strip.Begin();
    strip.Show();

    // Distance sensor
    if (!lox.begin()) {
        Serial.println(F("Failed to boot VL53L0X"));
        while(1);
    }

    lox.startRangeContinuous();

    // LCD Display
    lcd.init();

    lcd.backlight();
    // lcd.setCursor(3,0);
    // lcd.print("AGD 145");
    // lcd.setCursor(5,3);
    // lcd.print("BMK 4PS");

    // BUILD SERVER
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop()
{
    manageClient();
}

bool isDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

int getTemeperature(int index)
{
    int result = 0;
    while (isDigit(header.charAt(index)))
    {
        result *= 10;
        result += header.charAt(index) - '0';
        index++;
    }
    
    return result;
}

String htmlPage = "<!DOCTYPE html><html lang=\"en\"><head>    <meta charset=\"UTF-8\">    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">    <title>Simple Webpage</title>    <style>        body {            font-family: Arial, sans-serif;            margin: 0;            padding: 0;            display: flex;            flex-direction: column;            height: 100vh;        }        header {            background-color: #4CAF50;            color: white;            text-align: center;            padding: 1em;            font-size: 1.5em;        }        .container {            display: flex;            flex: 1;        }        .left-section {            flex: 1;            padding: 20px;            background-color: #f4f4f4;        }        .right-section {            flex: 2;            padding: 20px;            display: flex;            justify-content: center;            align-items: center;            background-color: #ffffff;        }        .block {            background-color: #fff;            padding: 20px;            border: 1px solid #ccc;            border-radius: 8px;            box-shadow: 0 2px 4px rgba(0,0,0,0.1);        }        .block h2 {            margin-top: 0;        }        .block input[type=\"text\"] {            width: calc(100% - 22px);            padding: 10px;            margin-bottom: 10px;            border: 1px solid #ccc;            border-radius: 4px;        }        .block button {            background-color: #4CAF50;            color: white;            padding: 10px 20px;            border: none;            border-radius: 4px;            cursor: pointer;        }        #temp-input {            margin:4%;        }        .block button:hover {            background-color: #45a049;        }    </style></head><body><script>    function changeTemp(id)    {        let val = Number(document.getElementById(\"temp-input\").value);        fetch(\"/setTemp/\"+val).then((response) => console.log(response));    }</script><header>   Smart pot holder</header><div class=\"container\">    <div class=\"left-section\">        <div class=\"block\">            <h2>Set Desired Temperature</h2>            <input id=\"temp-input\" type=\"number\" placeholder=\"Enter desired temperature\" value=\"15\">            <button id=\"btn\" onclick=\"changeTemp(this)\">Submit</button>        </div>    </div>    <div class=\"right-section\">        <div id=\"diagram\">Diagram Area</div>    </div></div></body></html>";

void manageClient()
{
    WiFiClient client = server.available();

    if (client) {                             // If a new client connects,
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected()) {  // loop while the client's connected
            if (client.available()) {             // if there's bytes to read from the client,
                char c = client.read();             // read a byte, then
                header += c;
                if (c == '\n') {                    // if the byte is a newline character
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println("Connection: close");
                        client.println();
                        
                        if (header.indexOf("setTemp/") >= 0) {
                            int index = header.indexOf("setTemp/");
                            index += 8;
                            int temperature = getTemeperature(index);
                            Serial.println(temperature);
                        } 
                        
                        client.println(htmlPage);
                        client.println();
                        break;
                    } else { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                } else if (c != '\r') {  // if you got anything else but a carriage return character,
                currentLine += c;      // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
    }
}
