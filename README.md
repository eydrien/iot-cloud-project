# 🌦️ Proyecto IoT: Control y Monitoreo del Clima con ESP32 + ThingSpeak

Este proyecto implementa un sistema **IoT (Internet of Things)** basado en **ESP32**, que obtiene datos meteorológicos en tiempo real desde la API de **OpenWeatherMap**, los envía a la plataforma **ThingSpeak**, y permite **controlar remotamente el comportamiento del dispositivo** mediante comandos desde la nube.

## 📸 Imagen del Proyecto




---

## 🧠 Objetivo
Desarrollar un sistema embebido capaz de:
- Conectarse a una red WiFi.  
- Consultar información climática (temperatura, humedad y descripción del clima).  
- Enviar los datos a **ThingSpeak** para su visualización.  
- Recibir comandos remotos desde ThingSpeak para cambiar su modo de operación.  
- Mostrar los resultados en una pantalla **OLED (SSD1306)**.

## ⚙️ Componentes Utilizados
| Componente | Descripción |
|-------------|--------------|
| **ESP32 DevKit V1** | Microcontrolador principal con conectividad WiFi integrada |
| **OLED 0.96" (I2C)** | Pantalla para visualizar datos locales |
| **Cables Dupont** | Conexiones I2C (SDA/SCL) |
| **ThingSpeak** | Plataforma IoT para visualización y control |
| **OpenWeatherMap API** | Fuente de datos meteorológicos |

## 🔌 Conexiones (ESP32 ↔ OLED)
| OLED | ESP32 |
|------|-------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

## 🧩 Librerías requeridas
Instálalas desde el **Administrador de Bibliotecas** en Arduino IDE:

- `WiFi.h` (incluida con ESP32)
- `HTTPClient.h` (incluida con ESP32)
- `ArduinoJson` (por Benoit Blanchon)
- `Adafruit GFX Library`
- `Adafruit SSD1306`
- `Wire.h` (I2C)

## ☁️ Integración en ThingSpeak

### 🔹 Canal 1 – Datos del Clima
Recibe los datos enviados por el ESP32.
| Campo | Descripción |
|--------|--------------|
| Field 1 | Temperatura (°C) |
| Field 2 | Humedad (%) |
| Field 3 | Intensidad de señal WiFi (RSSI) |

### 🔹 Canal 2 – Control Remoto
Permite enviar comandos al ESP32.
| Campo | Descripción |
|--------|--------------|
| Field 1 | Comando (1=Clima, 2=Sistema, 3=Rápido) |

## 🕹️ Modos de Operación
| Comando | Descripción |
|----------|--------------|
| `1` | Modo Clima → muestra temperatura, humedad y descripción del clima |
| `2` | Modo Sistema → muestra nombre del WiFi, RSSI y tiempo activo |
| `3` | Modo Rápido → cambia la frecuencia de actualización a cada 10 segundos |

## 🖥️ Panel de Control (ThingSpeak)
Se implementaron **botones de control** en el canal remoto con las siguientes acciones:
- ☁️ Enviar valor **1** → Modo Clima  
- ⚙️ Enviar valor **2** → Modo Sistema  
- ⚡ Enviar valor **3** → Modo Rápido  

Estos botones escriben el valor en el **Field 1** del canal de control, el cual el ESP32 lee y ejecuta.

## 🧱 Estructura del Proyecto
```
iot-weather-project/
├── firmware/
│   ├── main.ino
└── README.md
```

## 📊 Flujo del Sistema
```
OpenWeatherMap API  →  ESP32  →  ThingSpeak (Canal de Datos)
                                   ↑
                                   ↓
                         ThingSpeak (Canal de Control)
                                   ↑
                           Panel con Botones
```

## 🧪 Funcionamiento General
1. El ESP32 se conecta a la red WiFi.
2. Obtiene datos climáticos de **OpenWeatherMap**.
3. Los envía a ThingSpeak (canal de datos).
4. Lee el último comando del canal de control.
5. Cambia su comportamiento y lo refleja en pantalla OLED.
6. Todo se actualiza automáticamente cada cierto intervalo.

## 🧑‍💻 Autor
**Adrián David González Romero**  
Estudiante de Ingeniería de Sistemas y Computación  
📍 Colombia  
Proyecto académico - Computación en la Nube ☁️  

## 🏁 Licencia
Uso educativo y libre con fines de aprendizaje.  
Créditos a las librerías y APIs mencionadas.
