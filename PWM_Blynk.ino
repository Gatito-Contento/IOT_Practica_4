#define BLYNK_TEMPLATE_ID "TMPL2fpwjiwrM"
#define BLYNK_TEMPLATE_NAME "PWM motor dc"
#define BLYNK_AUTH_TOKEN "Ebcqz7XOjc7KfWwzBep0chQivvOsMYwd"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <esp_wifi.h> // Necesario para desactivar el ahorro de energía Wi-Fi

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Entradafacil";
char pass[] = "qsefthil1342";

// Pines de salida PWM
const int ledPin1 = 2;  // LED interno / Primer salida PWM
const int ledPin2 = 4;  // Segundo pin de salida PWM (puedes cambiar el 4 por el pin que gustes)

// Configuración PWM
const int freq = 5000;      // 5 kHz
const int resolution = 8;   // 8 bits (0 a 255)

BLYNK_CONNECTED() {
  Serial.println("¡Conectado a Blynk! Sincronizando datos...");
  Blynk.syncVirtual(V0);
}

BLYNK_WRITE(V0) {
  int valorSlider = param.asInt(); // Lee el valor del slider (0 a 255)
  
  Serial.print("-> PWM enviado a ambos pines: ");
  Serial.println(valorSlider);

  // Escribimos la misma señal PWM en los dos pines simultáneamente
  ledcWrite(ledPin1, valorSlider);
  ledcWrite(ledPin2, valorSlider);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\nIniciando ESP32...");

  // Conexión a Blynk
  Blynk.begin(auth, ssid, pass);

  // Desactivar el ahorro de energía Wi-Fi (mantiene baja latencia)
  WiFi.setSleep(false);
  

  // Configuración de las dos salidas PWM (ESP32 Core v3.x)
  ledcAttach(ledPin1, freq, resolution);
  ledcAttach(ledPin2, freq, resolution);

  // Ambos arrancan en 0 (apagados)
  ledcWrite(ledPin1, 0);
  ledcWrite(ledPin2, 0);
}

void loop() {
  Blynk.run();
}