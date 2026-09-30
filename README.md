# 🚀 Control Remoto de Motor DC mediante ESP32, Blynk e Interfaz MOSFET

Este repositorio contiene la implementación completa de un sistema de control de velocidad para motores de corriente continua (12 V) accionado inalámbricamente desde la plataforma **Blynk IoT** mediante un **ESP32** y un circuito discreto de conmutación de potencia con **MOSFET de nivel lógico**.

---

## 📌 Archivos del Repositorio

- `PWM_Blynk.ino`: Código fuente para el **ESP32** (Arduino IDE / C++) que gestiona la conexión a Blynk y la generación de la señal PWM.
- `Codigo_Falstad`: Archivo de texto plano que contiene la red gráfica del circuito para cargar la simulación directamente en **Falstad Circuit Simulator**.

---

## 🛠️ Arquitectura del Sistema

El proyecto divide el **dominio lógico/datos** del **dominio de potencia/energía**:

1. **Interfaz de Usuario:** Control deslizante (*Slider*) en la app/web de Blynk que define la velocidad deseada.
2. **Nube (Blynk Cloud):** Procesa y autentica los mensajes mediante un **Auth Token** permanente.
3. **Gateway Local:** Red Wi-Fi local de 2.4 GHz (e.g., *Hotspot* móvil) que enlaza la nube con el hardware.
4. **Microcontrolador (ESP32):** Cliente IoT que decodifica la orden remota y genera una señal PWM por hardware (3.3 V) en el pin `D4`.
5. **Etapa de Potencia Discreta (Módulo "Caja Negra"):** Conmuta la carga inductiva de 12 V hacia tierra (*Low-Side Switching*) mediante un MOSFET de nivel lógico (ej. IRLZ44N) con diodo de libre circulación (*Flyback*) y protecciones integradas.

---

## 🔌 Lista de Materiales (BOM)

| Componente | Valor / Especificación | Función |
| :--- | :--- | :--- |
| **Microcontrolador** | ESP32 Dev Module | Cliente IoT y generador de señal PWM |
| **MOSFET** | IRLZ44N (o AO3400, FQP30N06L) | Interruptor de potencia de nivel lógico (3.3 V) |
| **Diodo Flyback** | 1N5819 (Schottky) / UF4007 | Protección contra picos de fuerza contraelectromotriz |
| **Resistencia Gate** | 100 Ω (47 Ω - 220 Ω) | Limita la corriente de conmutación del GPIO |
| **Resistencia Pulldown** | 10 kΩ (4.7 kΩ - 47 kΩ) | Mantiene el MOSFET apagado durante el arranque |
| **Capacitor Fuente** | 100 µF / 25 V (Electrolítico) | Filtro de desacoplo y estabilidad de la línea de 12 V |
| **Capacitor Motor** | 100 nF / 50 V (Cerámico 104) | Supresión de ruidos electromagnéticos (EMI) |
| **Fuentes** | 12 V DC (Motor) / 5 V USB-C (ESP32) | Alimentación de potencia y lógica |

> **⚠️ NOTA CRÍTICA:** Las masas (**GND**) de la fuente de 12 V, del módulo de potencia y del ESP32 **deben unificarse en un único punto común** para permitir la conmutación adecuada del transistor.

---

## ⚡ Cómo Cargar la Simulación en Falstad

Para visualizar y analizar el comportamiento dinámico del circuito de potencia antes del montaje físico:

1. Abre el simulador en línea [Falstad Circuit Simulator](https://www.falstad.com/circuit/).
2. Abre el archivo `Codigo_Falstad` de este repositorio y copia todo su contenido de texto.
3. En la barra superior de Falstad, ve a **File > Import From Text...** (Archivo > Importar desde texto...).
4. Pega el texto copiado en la ventana emergente y presiona **OK**.
5. El circuito se cargará automáticamente permitiéndote ajustar la frecuencia y ver las ondas de corriente y voltaje.

---

## 🚀 Instrucciones de Uso y Configuración

### 1. Configuración de Blynk
1. Crea un nuevo **Template** y un **Device** en la consola de Blynk.
2. Agrega un **Datastream Virtual** (ej. `V0`) con rango entero de `0` a `255` (o `0` a `1023`).
3. Agrega un widget tipo **Slider** asociado a ese Datastream.
4. Copia tus credenciales de Blynk (`BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME` y `BLYNK_AUTH_TOKEN`).

### 2. Cargar el Código en el ESP32
1. Abre el archivo `PWM_Blynk.ino` en **Arduino IDE**.
2. Asegúrate de tener instalada la librería oficial de `Blynk`.
3. Edita las constantes al inicio del código con tus datos:
   ```cpp
   #define BLYNK_TEMPLATE_ID "TU_TEMPLATE_ID"
   #define BLYNK_TEMPLATE_NAME "TU_TEMPLATE_NAME"
   #define BLYNK_AUTH_TOKEN "TU_AUTH_TOKEN_PERMANENTE"

   char ssid[] = "NOMBRE_DE_TU_RED_WIFI";
   char pass[] = "CONTRASEÑA_DE_TU_RED";
   
Conecta tu ESP32 por USB y sube el firmware.

### 3. Montaje del Hardware
Ensamble el circuito de potencia siguiendo el esquema de Falstad.

Conecta el pin D4 del ESP32 a la entrada de la resistencia de Gate (100 Ω).

Conecta las tierras comunes (GND) de todo el sistema.

Energiza la fuente de 12 V y conecta el ESP32 por USB.

Desplaza el slider en Blynk para regular la velocidad del motor en tiempo real.

##💡 ¿Por qué un módulo discreto en lugar de uno comercial?
Se prefirió diseñar un circuito propio por motivos didácticos, de eficiencia, costo y seguridad. Muchos módulos comerciales económicos utilizan MOSFETs no optimizados para nivel lógico (3.3 V), provocando que operen en zona lineal, se sobrecalienten y quemen el componente. Un diseño a medida permite la selección precisa de partes, garantiza mayor eficiencia de conmutación y facilita el diagnóstico de fallos, actuando como un bloque confiable o "caja negra" para proyectos futuros.
