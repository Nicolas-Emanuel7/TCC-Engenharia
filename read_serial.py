import serial
import csv
import time


# Configuração da porta serial
PORTA = 'COM7'  # aqui coloca a porta serial 
BAUDRATE = 9600


# Conectar ao Arduino
arduino = serial.Serial(PORTA, BAUDRATE, timeout=1)


time.sleep(2)


# Criar arquivo CSV
with open('dados_sensor.csv', 'w', newline='') as arquivo:
    escritor = csv.writer(arquivo)
    # Cabeçalho
    escritor.writerow(['timestamp', 'leitura_sensor'])
    print("Capturando dados... Pressione CTRL+C para parar.")


    try:
        while True:
            linha = arduino.readline().decode('utf-8').strip()
            if linha:
                leitura = int(linha)
                timestamp = time.time()
                escritor.writerow([timestamp, leitura])
                arquivo.flush()
                print("Sensor:", leitura)


    except KeyboardInterrupt:
        print("\nCaptura finalizada!")


    finally:
        arduino.close()