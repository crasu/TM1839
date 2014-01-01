#include "Adafruit_NeoPixel.h"
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xCD };
IPAddress ip(192, 168, 100, 177);

EthernetServer server(80);

#define RED    0xFF0000
#define BLUE   0x0000FE
#define GREEN  0x00FF00
#define YELLOW 0xFFFF00
#define PINK   0xFF00FE
#define WHITE  0xFFFFFE
#define BLACK  0x000000

#define PIN 7 // Datapin
#define NOLEDS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NOLEDS, PIN, NEO_GRB + NEO_KHZ800);

void processGetRequest(String line)
{ 
  Serial.println("Processing get request");

  if (line.indexOf("/BLACK") >= 0) {  
    Serial.println("Got black command");
    colorWipe(BLACK, 1);
  }

  int index = line.indexOf("/SET");
  if (index  >= 0) {  
    //Strindg data = line.substring(index + 5, line.length() - 9);
    //Serial.println("Got set command with data: '" + data + "'");
    
    processLedData(line, index + 5);
    delay(5);
    strip.show();
  }  
}

void processLedData(String line, int dataindex)
{
  int datalength = line.length() - dataindex - 9;
  for(int i = 0; i <= datalength - 8; i+=8) {
    char buffer[10];

    String entry = line.substring(dataindex + i, dataindex + i + 8);

    entry.toCharArray(buffer, 10);
    unsigned long value = 0L;
    sscanf(buffer, "%lx", &value);
    
    uint16_t pos = value >> 24;
    uint8_t red =  value >> 16;
    uint8_t green = value >> 8;
    uint8_t blue = value;
    
    //TODO make debug output nicer
    Serial.println("Got data entry with values:");
    Serial.println(pos);
    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);
    strip.setPixelColor(pos, red, green, blue);
  }
}

void setup()
{
  Serial.begin(9600); 
  delay(1000);

  startEthernet();

  strip.begin();

  colorWipe(BLACK, 4);

  while(1) {
    handleRequests(); 
    //colorMove();
    //delay(5);
    //Serial.println("loop");
  }
}

void startEthernet()
{
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());  
}

void handleRequests() 
{
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    while (client.connected()) {
      if (client.available()) {
        String line = readHttpLine(client);

        if (line.indexOf("GET /") >= 0) {
          processGetRequest(line);  
        }

        if (line.equals("")) {
          sendHttpResponse(client);
          break;
        }
      }
    }

    delay(1);
    client.stop();
    Serial.println("client disonnected");
  }
}

String readHttpLine(EthernetClient client)
{
  String line = client.readStringUntil('\n');     
  line.replace("\r", "");
  Serial.println("httpline'" + line + "'");  

  return line;
}

void sendHttpResponse(EthernetClient client)
{
  Serial.println("Sending response now");
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connnection: close");
  client.println();
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("OK");
  client.println("</html>");
}  

void colorWipe(uint32_t color, int wait)
{
  for (int i=0; i < NOLEDS; i++) {
    strip.setPixelColor(i, color);
    if(wait > 0) {
      delay(1);
      strip.show();
      delay(wait);
    }
  }
}

void colorMove()
{
  for (int i=0; i <= NOLEDS; i++) {
    strip.setPixelColor(i > 0 ? i - 1 : 0, BLACK);
    strip.setPixelColor(i, WHITE);
    strip.show();
    delay(100);
  }
}

void loop()
{
}



