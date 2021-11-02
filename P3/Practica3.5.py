# -*- coding: utf-8 -*-
"""
Created on Tue Nov  2 19:14:32 2021

@author: Alba
"""

import serial
import numpy as np
import time;
import matplotlib.pyplot as plt
from drawnow import *
plt.style.use('ggplot')

#0.- Definimos las figuras que vamos a mostrar
def Figuras():
    plt.subplot(2,2,1)
    plt.ylim(-2,2)
    plt.title('X', fontsize=12, fontweight='bold', color='k')
    plt.ylabel('Valor [g]', fontsize=12, fontweight='bold', color='k')
    plt.xlabel('Tiempo [s]', fontsize=12, fontweight='bold', color='k')
    plt.plot(tiempo, mean['X'], label = 'media')
    plt.plot(tiempo, std['X'], label = 'desv. est.')
    plt.legend()
    
    plt.subplot(2,2,2)
    plt.ylim(-2,2)
    plt.title('Y', fontsize=12, fontweight='bold', color='k')
    plt.ylabel('Valor [g]', fontsize=12, fontweight='bold', color='k')
    plt.xlabel('Tiempo [s]', fontsize=12, fontweight='bold', color='k')
    plt.plot(tiempo, mean['Y'], label = 'media')
    plt.plot(tiempo, std['Y'], label = 'desv. est.')
    plt.legend()
    
    plt.subplot(2,2,3)
    plt.ylim(-2,2)
    plt.title('Z', fontsize=12, fontweight='bold', color='k')
    plt.ylabel('Valor [g]', fontsize=12, fontweight='bold', color='k')
    plt.xlabel('Tiempo [s]', fontsize=12, fontweight='bold', color='k')
    plt.plot(tiempo, mean['Z'], label = 'media')
    plt.plot(tiempo, std['Z'], label = 'desv. est.')
    plt.legend()
    
    plt.subplot(2,2,4)
    plt.ylim(-2,2)
    plt.title('A', fontsize=12, fontweight='bold', color='k')
    plt.ylabel('Valor [g]', fontsize=12, fontweight='bold', color='k')
    plt.xlabel('Tiempo [s]', fontsize=12, fontweight='bold', color='k')
    plt.plot(tiempo, mean['A'], label = 'media')
    plt.plot(tiempo, std['A'], label = 'desv. est.')
    plt.legend()




# 1.- DEFINIMOS EL PUERTO 
PuertoSerie = serial.Serial('COM4', baudrate=115200, timeout=1.0)

#2.- LEEMOS DEL PUERTO SERIE LIMPIANDO LA INFORMACIÓN NO DESEADA
# Y CREAMOS UN TIMER PARA QUE ACUMULE LOS DATOS CADA 5 SEGUNDOS

signal = {'X' : [], 'Y' : [], 'Z' : [], 'A' : []}
mean = {'X' : [], 'Y' : [], 'Z' : [], 'A' : []}
std = {'X' : [], 'Y' : [], 'Z' : [], 'A' : []}


tiempo = []
tiempoCero = time.time()
tiempoTomadeDatos = 5.0 

while True:
    tiempoInicio = time.time()
    tiempoActual = tiempoInicio
    while(tiempoTomadeDatos >= (tiempoActual-tiempoInicio)):
        esp32 = PuertoSerie.readline()
        datosCaracter = ""
        for valor in esp32:
            datosCaracter = datosCaracter + chr(valor)
        prueba = datosCaracter.replace(" ", ";").replace("\n","").replace("\r","").split(";")
        listaTexto =list(prueba)
        signal['X'].append(float(listaTexto[1]))
        signal['Y'].append(float(listaTexto[3]))
        signal['Z'].append(float(listaTexto[5]))
        signal['A'].append(float(listaTexto[7]))
        tiempoActual = time.time()
    npX = np.array(signal['X'])
    npY = np.array(signal['Y'])
    npZ = np.array(signal['Z'])
    npA = np.array(signal['A'])
    mean['X'].append(npX.mean())
    mean['Y'].append(npY.mean())
    mean['Z'].append(npZ.mean())
    mean['A'].append(npA.mean())
    std['X'].append(npX.std())
    std['Y'].append(npY.std())
    std['Z'].append(npZ.std())
    std['A'].append(npA.std())
    
    tiempo.append(tiempoActual - tiempoCero)
    
    drawnow(Figuras)

esp32.close()