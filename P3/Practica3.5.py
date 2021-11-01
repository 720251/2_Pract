import serial
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d
import matplotlib.animation as animation

PuertoSerie = serial.Serial('COM4', baudrate=115200, timeout=1.0)
signal = {'X' : [], 'Y' : [], 'Z' : [], 'A' : []}
contador = 0
fig = plt.figure()
ax1 = fig.add_subplot(111,projection='3d')

while True:

    try:
        esp32 = PuertoSerie.readline()
        datosCaracter = ""
        for valor in esp32:
            datosCaracter = datosCaracter + chr(valor)
        prueba = datosCaracter.replace(" ", ";").replace("\n","").replace("\r","").split(";")
        listaTexto =list(prueba)
        #print(listaTexto)
        if contador < 5:
            signal['X'].append(float(listaTexto[1]))
            signal['Y'].append(float(listaTexto[3]))
            signal['Z'].append(float(listaTexto[5]))
            signal['A'].append(float(listaTexto[7]))
            contador = contador + 1
            print(contador)
            if contador == 5:
                meanX = np.mean(signal['X'])
                stdX = np.std(signal['X'])
                meanY = np.mean(signal['Y'])
                stdY = np.std(signal['Y'])
                meanZ = np.mean(signal['Z'])
                stdZ = np.std(signal['Z'])
                meanA = np.mean(signal['A'])
                stdA = np.std(signal['A'])
                ax1.scatter(meanX, meanY, meanZ, c='g', marker='o')
                ax1.scatter(stdX, stdY, stdZ, c='r', marker = 'o')
                plt.show()
        else:
            contador = 0;
            signal['X'].clear()
            signal['Y'].clear()
            signal['Z'].clear()
            signal['A'].clear()
        print(signal)

        
    except KeyboardInterrupt:
        PuertoSerie.close()
        break
    





