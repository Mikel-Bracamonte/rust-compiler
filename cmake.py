import os
import subprocess

carpeta = "./tests"
script = "./compile.sh"

if not os.path.isdir(carpeta):
    print(f"La carpeta {carpeta} no existe.")
    exit(1)

subprocess.run('g++ *.cpp -o compiler', shell=True, check=True)

for archivo in os.listdir(carpeta):
    ruta_archivo = os.path.join(carpeta, archivo)

    if os.path.isfile(ruta_archivo):
        print(f"Compilando: {ruta_archivo}")
        try:
            subprocess.run(['./compiler', ruta_archivo])
            subprocess.run(['gcc input.s -o output'])
            subprocess.run(['./output'])
        except subprocess.CalledProcessError as e:
            print(f"Error al compilar {ruta_archivo}: {e}")
