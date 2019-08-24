#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "CarroFPV";
const char* password = "science_7425";

//------------------------------------------------------------ setupWiFi
void setupWiFi()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("\n &IP Address: ");
  Serial.println(WiFi.softAPIP());
}

ESP8266WebServer server(80);

/*
#define PIN_PWMA D1 //motor derecho
#define PIN_DIRA D3
#define PIN_PWMB D2 //motor izquierdo
#define PIN_DIRB D4
*/

int PWMA=5;//Right side
int PWMB=4;//Left side
int DA=0;//Right reverse
int DB=2;//Left reverse

void handleRoot() {
  server.send(200, "text/plain", "For Car control go to '192.168.4.1/car'");
 }

void handleCar() {
 String message = "";
 int BtnValue = 0;
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i)=="a")
    {
      String s = server.arg(i);
      BtnValue = s.toInt();
    }
    Serial.println(server.argName(i) + ": " + server.arg(i) + "\n");
  }

  switch (BtnValue) {

   case 1: // Izquierda
    digitalWrite(PWMA, HIGH);
    digitalWrite(DA, LOW);
    digitalWrite(PWMB, LOW);
    digitalWrite(DB, LOW);
    break;
   case 2: // De frente
      digitalWrite(PWMA, HIGH);
      digitalWrite(DA, LOW);
      digitalWrite(PWMB, HIGH);
      digitalWrite(DB, LOW);
     break;
  case  3:// Derecha
     digitalWrite(PWMA, LOW);
     digitalWrite(DA, LOW);
     digitalWrite(PWMB, HIGH);
     digitalWrite(DB, LOW);
     break;
  case  4:// Izquierda Cam
     //motorSpeed(900,LOW,HIGH,900,HIGH,LOW);
     break;
  case 5: // Stop
   digitalWrite(PWMA, LOW);
   digitalWrite(DA, LOW);
   digitalWrite(PWMB, LOW);
   digitalWrite(DB, LOW);
   break;
  case  6://  Derecha Cam
     //motorSpeed(900,HIGH,LOW,900,LOW,HIGH);
    break;
  case 7://Izquierda atrás
    digitalWrite(PWMA, HIGH);
    digitalWrite(DA, HIGH);
    digitalWrite(PWMB, LOW);
    digitalWrite(DB, LOW);
    break;
  case 8://Atrás
    digitalWrite(PWMA, HIGH);
    digitalWrite(DA, HIGH);
    digitalWrite(PWMB, HIGH);
    digitalWrite(DB, HIGH);
    break;
  case 9://Derecha atrás
    digitalWrite(PWMA, LOW);
    digitalWrite(DA, LOW);
    digitalWrite(PWMB, HIGH);
    digitalWrite(DB, HIGH);
    default:
    break;
  }

  message += "<html> <head> <title>Gungor yalcin</title><head>";
  message += "<body><center><h3>Wifi CarroFPV NodeMCU  Web Server</center></h1>";
  message += "<table> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=1\"><button style=\"width:320;height:500;font-size:500px;\" class=\"button\">\\</button></a></p> ";
  message += "<td><p><a href=\"/car?a=2\"><button style=\"width:320;height:500;font-size:500px;\" class=\"button\">^</button></a></p> ";
  message += "<td><p><a href=\"/car?a=3\"><button style=\"width:320;height:500;font-size:500px;\" class=\"button\">/</button></a></p> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=4\"><button style=\"width:320;height:500;font-size:500px;\" class=\"button\"> < </button></a></p> ";
  message += "<td><p><a href=\"/car?a=5\"><button style=\"width:320;height:500;font-size:300px;\" class=\"button\">O</button></a></p> ";
  message += "<td><p><a href=\"/car?a=6\"><button style=\"width:320;height:500;font-size:500px;\" class=\"button\"> > </button></a></p> ";
  message += "<tr>";
  message += "<td><p><a href=\"/car?a=7\"><button style=\"width:320;height:500;font-size:500px;\" class=\"button\">/</button></a></p> ";
  message += "<td><p><a href=\"/car?a=8\"><button style=\"width:320;height:500;font-size:500px;\" class=\"button\">v</button></a></p> ";
  message += "<td><p><a href=\"/car?a=9\"><button style=\"width:320;height:500;font-size:500px;\" class=\"button\">\\</button></a></p> ";
  message += "</table> ";
  message += "</body></html>";
  server.send(200, "text/html", message);
 }

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {

pinMode(PWMA, OUTPUT);
pinMode(PWMB, OUTPUT); 
pinMode(DA, OUTPUT);
pinMode(DB, OUTPUT);

  Serial.begin(115200);

  setupWiFi();

  server.on("/", handleRoot);
  server.on("/car", handleCar);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  }

  void loop() {
    server.handleClient();
  }
