from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import subprocess, traceback
from pathlib import Path

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"], allow_methods=["*"], allow_headers=["*"],
)

class CompileRequest(BaseModel):
    code: str               # contenido de entrada para el programa (input.rs)
    optimizationLevel: str = "O2"

@app.post("/compile")
async def compile_code(req: CompileRequest):
    try:
        # Directorio raíz de los sources y ejecutable
        cwd = Path(__file__).parent

        # 1) Escribe el archivo de entrada a input.rs
        input_path = cwd / "input.rs"
        input_path.write_text(req.code, encoding='utf-8')

        # 2) Lista de archivos fuente .cpp
        cpp_files = [f for f in cwd.glob('*.cpp')]
        if not cpp_files:
            return {"error": "No se encontraron archivos .cpp en el directorio"}

        exe_file = cwd / "x"
        opt_flag = f"-O{req.optimizationLevel.lstrip('O')}"

        # 3) Compilar todos los .cpp en un solo binario
        #compile_cmd = ["g++", *map(str, cpp_files), opt_flag, "-o", str(exe_file)]
        #proc_compile = subprocess.run(compile_cmd, capture_output=True, text=True)

       # if proc_compile.returncode != 0:
        #    return {"error": proc_compile.stderr or "Error al compilar los .cpp"}

        # 4) Ejecutar el binario con input.rs como argumento
        run_cmd = [str(exe_file), str(input_path)]

        run = subprocess.run(run_cmd, capture_output=True, text=True, timeout=10)
        # EJECUTABLE
        output_text = run.stdout or run.stderr # ejecutable


        if run.returncode != 0:
            return {"error": output_text}

        # 5) Generar ensamblador del ejecutable
        asm_file = cwd / "input.s"
        asm_cmd = ["g++", "-S", str(exe_file), opt_flag, "-o", str(asm_file)]
        proc_asm = subprocess.run(asm_cmd, capture_output=True, text=True)
        if proc_asm.returncode != 0:
            return {"error": proc_asm.stderr or "Error al generar ensamblador"}

        # Leer el ensamblador, reemplazando caracteres inválidos
        asm_text = asm_file.read_text(encoding='utf-8', errors='replace')
        
        
        # input.s
        # gcc input.s -o a
        # ./a
        exe2 = cwd / "a"
        compile2 = subprocess.run(
            ["gcc", str(asm_file), "-o", str(exe2)],
            capture_output=True,
            text=True
        )
        if compile2.returncode != 0:
            return {"error": compile2.stderr or "Error al compilar el ensamblador"}

        # 5.2) Ejecutar el ejecutable generado desde el .s
        run2 = subprocess.run(
            [str(exe2)],
            capture_output=True,
            text=True,
            timeout=10
        )
        assembly_run_output = run2.stdout or run2.stderr

        return {"output": assembly_run_output, "assembly": asm_text} # output_text

    except subprocess.TimeoutExpired:
        return {"error": "La ejecución excedió el tiempo máximo permitido"}
    except Exception as ex:
        traceback.print_exc()
        return {"error": f"Error interno: {ex}"}
