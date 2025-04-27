#include  <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "NOME_REDE";
const char* password = "SENHA_REDE";

#define LED_PIN 2
WebSocketsServer webSocket = WebSocketsServer(81);

bool estadoLED = false;

void enviarEstadoParaTodos() {
  if (estadoLED) {
    webSocket.broadcastTXT("LED:on");
  } else {
    webSocket.broadcastTXT("LED:off");
  }
}

void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.print("Cliente ");
	Serial.print(client_num);
	Serial.println("Conectado");
    if (estadoLED) {
      webSocket.sendTXT(client_num, "LED:on");
    } else {
      webSocket.sendTXT(client_num, "LED:off");
    }
  }
  else if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    Serial.println("Recebido: " + msg);

    if (msg == "TOGGLE_LED") {
      estadoLED = !estadoLED;
      digitalWrite(LED_PIN, estadoLED);
      enviarEstadoParaTodos();
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();
}
