import numpy as np 
import sys
import os
import csv
import time
import matplotlib.pyplot as plt 
from PIL import Image
import random

archivo = csv.reader(open('C:/Users/...','r'))
emociones = {0:"angry",1:"disgust",2:"fear",3:"happy",4:"sad",5:"surprise",6:"neutral"}
tipos = ["train","validation"]
for i in range(2):
    tipo_1 = tipos[i]
    os.mkdir("C:/Users/../data/{}".format(tipo_1))
i = len(tipos)
while i != 0:
    for item in range(7):
        emocion = emociones[item]
        os.mkdir(('C:/Users/.../data/'+str(tipos[i-1]) + '/{}').format(emocion))
    i -= 1
with open('C:/Users/.../labels.csv','r') as archivo:
    lector = csv.reader(archivo)
    for i,row in enumerate(lector):
        if i > 0:
            label,numeros,tipo = row
            integral = int(label)
            lista = numeros.split()
            matriz = np.reshape(lista,(48,48))
            matriz_2 = matriz.astype(np.uint8)
            imagen = Image.fromarray(matriz_2)
            if tipo == 'Training':
                imagen.save('C:/Users/.../train/'+ emociones[integral] +'/Imagen_' + str(i) +'_'+ emociones[integral] + '.jpg')
            else:
                imagen.save('C:/Users/.../validation/'+ emociones[integral] +'/Imagen_' + str(i) + '_'+ emociones[integral] + '.jpg')
            print("Numero: ",i,"   Tipo: ",tipo)
