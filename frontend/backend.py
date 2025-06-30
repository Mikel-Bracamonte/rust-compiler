import os
import subprocess
from fastapi import FastAPI, Request
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class CompileRequest(BaseModel):
    code: str
    optimizationLevel: str = 'O2'
    target: str = 'x86_64'

@app.post("/compile")
async def compile_code(req: CompileRequest):
    # Write code to input.rs
    with open("input.rs", "w", encoding="utf-8") as f:
        f.write(req.code)

    # Remove previous output files if exist
    if os.path.exists("output"):
        os.remove("output")
    if os.path.exists("input.s"):
        os.remove("input.s")

    # Run compile.sh
    try:
        proc = subprocess.run(["bash", "compile.sh", "input.rs"], capture_output=True, text=True, timeout=15)
        output = proc.stdout
        errors = proc.stderr
    except Exception as e:
        return {"output": "", "assembly": "", "errors": str(e)}

    # Read assembly if exists
    assembly = ""
    if os.path.exists("input.s"):
        with open("input.s", "r", encoding="utf-8") as f:
            assembly = f.read()

    # If there was a runtime error, append it to errors
    if proc.returncode != 0:
        errors = errors or output or "Error de compilación"
        output = ""

    return {
        "output": output.strip(),
        "assembly": assembly.strip(),
        "errors": errors.strip() if errors else ""
    } 