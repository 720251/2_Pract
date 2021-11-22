import serial
import os

PuertoSerie = serial.Serial('COM4', baudrate=115200, timeout=1.0)
file = open ("./practica3.4.txt","w")

while True:
    try:
        esp32 = PuertoSerie.readline()
        #print(esp32)
        datosCaracter = ""
        for valor in esp32:
            datosCaracter = datosCaracter + chr(valor)
        file.write(datosCaracter.replace(" ", ";")) 
        print(str(datosCaracter))
    except KeyboardInterrupt:
        break
    
PuertoSerie.close()
file.close()
