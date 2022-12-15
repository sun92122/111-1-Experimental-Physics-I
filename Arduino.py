import matplotlib.pyplot as plt
import numpy as np
import serial
import openpyxl

DEBUG = False

workbook = openpyxl.Workbook()
color = {'r': 'FF0000', 'g': '00FF00', 'b': '0000FF', 'c': '00FFFF',
         'm': 'FF00FF', 'y': 'FFFF00', 'k': '000000', 'w': 'FFFFFF'}

s = serial.Serial("COM5", 9600)
while True:
    c = input('color = ')
    #           #F00 #0F0 #00F #0FF #F0F #FF0 #000 #FFF
    if c not in ['r', 'g', 'b', 'c', 'm', 'y', 'k', 'w']:
        break
    try:
        sheet = workbook[color[c]]
    except:
        sheet = workbook.create_sheet(color[c])
        sheet.sheet_properties.tabColor = color[c]
    title = ['V (V)', 'I (mA)']
    sheet.append(title)

    V = np.array([])
    I = np.array([])

    s.close()
    s.open()
    t = 0

    while True:
        output = s.readline().decode().strip(' \n\b\r')
        if output:
            try:
                temp_V, temp_I = map(float, output.split())
                V = np.append(V, [[temp_V]])
                I = np.append(I, [[temp_I]])
                sheet.append([temp_V, temp_I])
            except:
                print(output, 'not data')
                if "start" in output:
                    t += 1
                if t > 3:
                    break

    plt.xlim(left=1.55, right=2.8)
    plt.ylim(bottom=-0.1, top=10.)
    plt.scatter(V, I, s=5, color=c, edgecolors='none')
plt.savefig('./fig.png', dpi=600)
plt.show()
workbook.save('./data.xlsx')
