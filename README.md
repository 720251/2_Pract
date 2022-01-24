# 2_Pract
----------PRACTICA 1: ESP32 COMO PLATAFORMA DE PROTOTIPADO RÁPIDO DE SENSORES----------

1.1) El primer objetivo de esta práctica es aprender a utilizar los timer hardware para generar una interrupción, además de leer el ADC y mostrarlo por pantalla.
Los timer permiten temporizar las diferentes funciones sin afectar a la ejecución del resto del programa, como si ocurre con los delay.

Para poder utilizarlos es necesario configurar una serie de variables como son interruptCounter, de tipo volatile porque se comparte entre la rutina de interrupción y el bucle principal y totalInterruptCounter, contador que cuenta las interrupciones que se van generando. También se requiere de un punterto timer del tipo hw_timer_t para configurar el temporizador y de un multiplexor que permite mantener la sincronización entre el bucle principal y la rutina de interrupción.

Se deben inicializar el temporizador, indicando la frecuencia de trabajo y como realizar el contador, además de vincularlo a una función de manejo y establecer el número de ticks del procesador. Para este caso, como se requiere realizar una interrupción cada 10 segundos, se debe configurar en 10000000.

Para leer el valor del ADC basta con utilizar la función analogRead(pinADC) y, siempre que se ejecute una interrupción, se leerá el valor.

1.2) En este subapartado, el objetivo es aprender a manejar la UART y los diferentes periféricos que tenemos. De esta forma, al escribir en la consola actADC dará como resultado la lectura del ADC actual, si se escribe ADC(x) siendo x un número del 1 al 9, mandará el valor del ADC cada x segundos (si x es 0, dejará de mandar datos), y si se escribe PWM(x) siendo x un número del 0 al 9, comandará el duty cycle, modificando la intensidad de iluminación de un led.

Para conseguir esto es necesario utilizar las diferentes funciones de los String y del Serial, además de utilizar ledcAttachPin y las funciones pertinentes para manejar el PWM.

1.3) Finalmente, el último apartado de la práctica consiste en el manejo de un sensor inercial, concretamente un MPU-9250. Utiliza el protocolo I2C para enviar la información al microcontrolador. Por ello, para facilitar su manejo, existen librerías como <MPU9250_asukiaaa.h> que permiten obtener los valores de aceleración siempre que se inicialice la función beginAccel() y se actualice el valor de aceleración con la función accelUpdate() cada vez que se requiera muestrear. Para visualizar los datos también existen funciones propias de la librería.

De esta forma, utilizando las interrupciones se muestrea cada 100ms, se activa un led cada 200ms y se manda la información por la UART cada 1000ms.

----------PRACTICA 2: DISEÑO BASADO EN SISTEMA OPERATIVO DE TIEMPO REAL ----------

Está práctica se centra en aprender el uso y conceptos asociados a un sistema operativo en tiempo real (RTOS) y diseñar un firmware basado en el mismo.
Trabajamos con una librería llamada ESP32_FreeRtos.  De esta forma, adaptaremos el sistema desarrollado la práctica anterior en el que se utiliza un sensor inercial, sustituyendo las interrupciones y contadores con tareas que tienen orden de prioridad. Es muy importante designar el orden de prioridad de tareas para su correcta ejecución.

Para crear las tareas se utiliza el comando xTaskCreatePinnedToCore, donde se designa el puntero a la función, el nombre de la tarea, el tamaño de pila de las tareas, un puntero que se utiliza como parámetro para la tarea que se está creando, la prioridad de la tarea, un indentificador mediante el cual se puede hacer referencia a la tarea creada (util para colas, semáforos y eventos) y el núcleo donde correrá la tarea. Para temporizar las acciones se utiliza vTaskDelay.

----------PRACTICA 3: COMUNICACION, GESTION Y REPRESENTACIÓN DE DATOS DE SENSORES CON PYTHON ----------

El objetivo fundamental es familiarizarse con Python como lenguaje de programación, aprender a gestionar datos procedentes de comunicaciones serie, trabajar con estos datos en formato .txt para almacenarlos en ficheros, y operar con los datos y representarlos gráficamente.
Para ello, partiremos de la práctica anterior donde se utiliza un sensor inercial que muestrea la aceleración y manda los datos vía UART cada 100 ms.

3.1) Se crea un algoritmo capaz de acceder al puerto serie, mostrando por pantalla los datos en tiempo real y almacenandolos eun un fichero .txt
Para leer lo que llega del puerto serie se utiliza la función PuertoSerie.readline(), posteriormente se elimina la información no deseada, como tabulaciones y retornos de carro, pasa separar cada elemento con un ';' y escribirlo tanto en la consola como en el archivo .txt.

3.2) Una vez conseguida la información a través del puerto serie, se debe acumular durante 5 segundos, calculando el promedio y la desviación estándar para representarlos en tiempo real.
En este caso, no solo es necesario importar la librería serie, sino que también se debe importar la librería numpy para manejarse con números y realizar cálculos estadísticos, la librería time para manejar los timers, la librería plt para realizar gráficos y la librería drawnow para realizar actualizaciones en tiempo real.

Igual que en el apartado anterior, se debe configurar el puerto y leerlo, limpiando la información no deseada, en este caso, solo queremos guardar los parámetros, que meteremos dentro de una variable de tipo diccionario denominada signal. Crearemos dos variables más de este tipo para la media y la desviación estándar.

Como solo queremos tener los parámetros y no sus nombres, que ya vienen definidos en la variable diccionario, pasaremos únicamente los parámetros en formato float, realizando un append para que se vayan acumulando en ese transcurso de tiempo.

Finalmente, para realizar el sumatorio de la media y la desviación estándar de cada parámetro se utiliza la función append, realizando la media o la desviación estándar del array que se requiere, metiéndolo en la variable diccionario correspondiente.
Para visualizar gráficamente los resultados se plotean los parámetros con su media y su desviación estandar.

----------PRACTICA 4: COMUNICACIONES WIFI DESDE MICROCONTROLADOR ----------

4.1) SOCKET TCP

En este apartado se va a implementar un programa para aprender a manejarse con el protocolo TCP. Para poder utilizar este protocolo vía WiFi, es necesario, además de incluir la librería WiFi, asignar su nombre de usuario y contraseña. Otros dos parámetros a tener en cuenta para el manejo de protocolo TCP son el servidor IP que deseamos visitar, es decir, la IP del servidor que pondremos en la aplicación SocketTest, y el puerto del servidor.

Para obtener la fecha y la hora se va a utilizar el servidor NTP, se trata de un protocolo de internet para sincronizar los relojes a través de una red. Como estamos en Europa, se especifica la dirección del servidor NTP como "europe.pool.ntp.org", se ajusta la compensación UTC y, como estamos en UTC+1 es 3600 y al ser horario de invierno, la compensación de luz será cero.

Se realiza un algoritmo que comprueba si el cliente, que somos nosotros, está conectado a la dirección IP y al puerto del servidor. Cuando el cliente tiene datos que leer, utilizamos la función readStringUntil para visualizar lo que el cliente recibe. Si recibimos un start, llamamos a una función para imprimir en este caso, la fecha y la hora y retornarsela al servidor. En caso de recibir un estop, se deja de escribir en la consola y de mandar la fecha y la hora. Para poder generar un servidor se utiliza la aplicación SocketTest.

4.2) SERVIDOR WEB

En este apartado, se va a implementar un programa para aprender a manejar el protocolo HTTP. El objetivo que se persigue es conseguir mostrar la hora en tiempo real en un servidor WEB y que, al pulsar un botón, la hora se resetee a cero y empiece a contar desde ese momento.
Al igual que en el apartado anterior, se debe incluir la librería WiFi y asignar su nombre de usuario y contraseña. También se debe sincronizar la fecha y la hora mediante el servidor NTP y, además se deben incluir las librerías “ESPAsyncWebServer.h” y “SPIFFS.h” definiendo el objeto servidor de tipo AsyncWebServer en el puerto 80.
La librería ESPAsyncWebServer, permite configurar las rutas donde el servidor escuchará las solicitudes HTTP entrantes y ejecutará funciones cuando se reciba una solicitud en esa ruta. Para eso se utiliza el método “on” en el objeto del servidor.  

Para poder configurar las rutas es necesario crear los archivos index.html y sytle.css que contienen un título, un botón y un marcador de posición, y el tamaño de fuente, estilo, color del botón y la alineación de la página, respectivamente, y subirlos pulsando Herramientas/ESP32 Sketch Data Upload. Cuando el servidor recibe una solicitud en la URL raíz, envía el archivo index.html configurado al cliente. Además, el cliente realizará también una solicitud para el archivo CSS.

4.3) JSON A SERVIDOR FTP

El siguiente apartado de la práctica consiste en implementar un programa para aprender a manejar el servidor TCP. El objetivo que se persigue es conseguir generar un fichero json que se suba al servidor cada 10 segundos y contenga una temperatura aleatoria y la marca temporal.

Para este apartado es necesario incluir las librerías “arduinojson.h” y “esp32_ftpClient”, además de las utilizadas anteriormente.

Como en los apartados anteriores, será necesario definir el usuario y la contraseña del WiFi y, para este caso, incluir la información necesaria para poder conectarse al servidor FTP de la universidad.

También se crean variables para el json, definiendo el documento que se guarda en la memoria estática, con una capacidad de memoria definida en función del tamaño del array y del tamaño del objeto.
Las variables almacenadas en los objetos json deben ser serializadas para poder mandarse como cadenas de caracteres. Una vez serializadas ya es posible mandarlas a través de conexión con servidor FTP, seleccionando el directorio en el que se quiere guardar el archivo, el tipo de archivo, su nombre y los datos a incluir. Posteriormente se cierra el archivo y la conexión.


4.4) MQTT

El último apartado de la práctica consiste en utilizar uno de los ejemplos de Adafruit donde se utiliza mqtt para realizar un contador y poder visualizarlo en la web de adafruitIO, en un feed asignado de Adafruit.
En este caso, en vez de configurar los parámetros en el código principal de Arduino, se configuran dentro de la librería config.h. Estos parámetros son el usuario y la contraseña del WiFi que se va a utilizar, el usuario y la key de Adafruit IO y otro conjunto de parámetros que nos permiten trabajar con esta aplicación.

Es necesario señalar que, para este caso, se ha utilizado un ejemplo propio de adafruitIO pero existe la posibilidad de usar brokers públicos y suscribirse a acciones o publicar datos en ese broker para que otro dispositivo lo reciba.

----------PRACTICA 5: COMUNICACIONES BLE Y BLUETOOTH ----------

En esta sesión de prácticas se pretende conseguir familiarizarse con las comunicaciones BLE de forma que sea posible leer mensajes de BLE emitidos por un sensor y reportarlos en formato json y emitir mensajes de advertising vía BLE.

5.1) ESCANEO DE BEACONS Y REPORTO EN FORMATO JSON

En primer lugar, es necesario descargarse la aplicación Beacon Simulator en el móvil para, como su nombre indica, simular uno, de forma que nuestro esp32 sea capaz de detectarlo y decodificar su información en formato json.

Para este caso, el servidor será el móvil puesto que anuncia su existencia, puede ser controlado por otros dispositivos y contiene la información que se desea leer, mientras que el cliente será el esp32, capaz de escanear dispositivos cercanos y, cuando encuentra el servidor que está buscando, lea la información entrante.

Como esta práctica consiste en el manejo de BLE, se incluyen sus librerías. Además, cabe destacar que la información recibida desde el beacon se encuentra en formato hexadecimal, por lo que es necesario convertirla a formato decimal utilizando la variable ENDIAN_CHANGE_U16

Como en prácticas anteriores, también se crean variables y funciones para el json. En este caso, la función almacenaDatos contiene los valores de major, minor, UUID y TxPower. La función escribeFichero tendrá el mismo cometido que en apartados anteriores, serializar en formato json.

Siguiendo uno de los ejemplos de Arduino, se crea la clase MyAdvertisedDeviceCallbacks donde se define la función onResult a la que se le pasa el argumento advertisedDevice de tipo BLEAdvertisedDevice. En esta función se imprime por pantalla la información de publicidad del dispositivo.

Cuando los datos publicitarios contienen algún tipo de información (manufacturerData), el tamaño es igual a 25, e l primer byte del elemento 4C y el segundo byte es 0, tenemos nuestro BEACON ENCONTRADO, diferenciándose del resto de dispositivos escaneados. En este momento será cuando utilicemos las funciones de json para almacenar los datos y escribir el fichero en formato json.

5.2) ADVERTISING iBEACON

Este apartado tiene como objetivo generar un anuncio con el esp32 y detectarlo con el móvil utilizando la aplicación nRFconnect. Para este caso, el móvil es el cliente puesto que escanea el dispositivo y el esp32 es el servidor puesto que se anuncia y contiene la información.

Se sigue uno de los ejemplos de BLE, definiendo el UUID (Identificador único universal) de forma que acabe con el nip de la universidad, 720251.
En la función setBeacon, se configuran las diferentes variables que se desea enviar, la ID como 4C, el UUID como el nuestro propio, major y el minor.
Tras cargar el código, observamos en el puerto serie como se comienza a anunciar el Beacon y, utilizando la aplicación nRFconnect observamos el dispositivo anunciado.

----------PRACTICA 5: COMUNICACIONES BLE Y BLUETOOTH ----------

Para la realización de esta práctica se han adquirido dos módulos sx1278 que trabajan en la banda de 433 MHz.

El objetivo es familiarizarse con este tipo de protocolo con la finalidad de establecer comunicaciones ping-ping entre dos dispositivos, además de enviar datos utilizando el protocolo LoraWAN a un servidor TTN. Para ello, se ha seguido la wiki del siguiente enlace: https://bricolabs.cc/wiki/guias/lora_ttn

6.1 PING-PONG LORA

Para este primer apartado, se conectarán dos módulos LoRa a dos módulos esp32, de forma que los pines quedarán distribuidos de la siguiente forma:

SX1278_SCK    18
SX1278_MISO   19
SX1278_MOSI   23
SX1278_CS     5
SX1278_RST    14
SX1278_DI0    26
LORA_BAND   433E6

Se crean dos funciones, que son llamadas en el bucle principal, la primera de ellas envía el mensaje siempre que el intervalo fijado en la configuración sea menor a la diferencia entre el millis actual y el último millis utilizado para el envío de los datos. La segunda de ellas recibe la información del otro dispositivo, comprueba si la longitud es la adecuada y si está recibiendo la información del dispositivo correcto, mostrando los parámetros necesarios por pantalla.

Si observamos con detenimiento la primera función, sendMessage, observamos como se inicia el paquete de LoRa, escribiendo la dirección de destino, la dirección local, contador de mensaje, la longitud del mensaje saliente y el mensaje saliente. Además, esto se muestra por pantalla.

En la segunda función, leemos los bytes del paquete y comprobamos si recibimos información de algún paquete por medio de LoRa. Además, chequeamos la longitud del paquete y si el destinatario es el correcto. En caso de ser correcto, se escriben una serie de parámetros para comprobar el envío.
Para comprobar el correcto funcionamiento de la práctica, se han utilizado dos esp32 con dos módulos LoRa, cada uno configurado de una forma distinta.

6.2 ENVÍO DATOS A TTN

Como los módulos LoRa adquiridos trabajan en la banda de 433MHz es necesario construir, en primer lugar, un Gateway.

Para ello se van a utilizar varias guías, entre las que podemos destacar https://learn.sparkfun.com/tutorials/esp32-lora-1-ch-gateway-lorawan-and-the-things-network/all y https://www.hackster.io/Arn/single-channel-ttn-lora-gateway-and-nodes-with-esp32-sx1276-709612. 

Antes de cargar el código de ejemplo, se deben realizar algunas modificaciones como son: banda de frecuencia de trabajo, configuración de los pines de SPI, configuración WiFi y configuración de los pines del modem de lora.

Si nos conectamos a la IP que nos indica (192.168.0.20) podremos visualizar el portal web ESP Gateway config. Desde esta página es posible controlar los mensajes que llegan, las frecuencias y los factores de propagación. Además, es posible cambiar la configuración de la puerta de enlace, ajustar el canal y el factor de dispersión, entre otras posibilidades.

Una vez tenemos nuestra puerta de enlace, debemos generar un código capaz de comunicarse con este Gateway para que envíe la información a la nube. Al trabajar con 433MHz no se ha conseguido poner en marcha el código para comunicarse con el gateway generado.
