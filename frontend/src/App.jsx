import React, { useState, useRef } from 'react';
import {
  Upload,
  Download,
  Settings,
  FileText,
  Terminal,
  Code,
  Zap,
  AlertCircle,
  CheckCircle,
  Copy,
  RefreshCw,
} from 'lucide-react';
import MonacoEditor from '@monaco-editor/react';
import { compileRustCode } from './api.jsx';

export default function App() {
  const [code, setCode] = useState(`fn main() {
  println!("¡Hola, mundo desde Rust!");
  let mut contador = 0;
  for i in 1..=5 {
    contador += i;
    println!("Iteración {}: contador = {}", i, contador);
  }
  let resultado = factorial(5);
  println!("Factorial de 5 = {}", resultado);
}

fn factorial(n: u32) -> u32 {
  if n <= 1 { 1 } else { n * factorial(n - 1) }
}`);
  const [output, setOutput] = useState('');
  const [assembly, setAssembly] = useState('');
  const [errors, setErrors] = useState('');
  const [view, setView] = useState('executed');
  const [isCompiling, setIsCompiling] = useState(false);
  const [compilationTime, setCompilationTime] = useState(0);
  const [showSettings, setShowSettings] = useState(false);
  const [optimizationLevel, setOptimizationLevel] = useState('O2');
  const fileInputRef = useRef();

  const handleUpload = e => {
    const f = e.target.files?.[0];
    if (!f || !f.name.match(/\.(rs|txt)$/)) {
      alert('Por favor sube un archivo .rs o .txt');
      return;
    }
    const reader = new FileReader();
    reader.onload = () => setCode(reader.result);
    reader.readAsText(f);
  };

  const handleDownload = () => {
    const blob = new Blob([code], { type: 'text/plain' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = 'input.rs';
    a.click();
    URL.revokeObjectURL(url);
  };

  const copyToClipboard = txt => navigator.clipboard.writeText(txt);

  const handleCompile = async () => {
    setIsCompiling(true);
    setErrors('');
    setOutput('');
    setAssembly('');
    const t0 = performance.now();

    let res;
    try {
      res = await compileRustCode({ code, optimizationLevel });
    } catch {
      setErrors('No se pudo conectar con el backend');
      setIsCompiling(false);
      return;
    }

    if (res.error) {
      setErrors(res.error);
    } else {
      setOutput(res.output || '');
      setAssembly(res.assembly || '');
    }

    setCompilationTime(Math.round(performance.now() - t0));
    setIsCompiling(false);
  };

  const getStatusIcon = () => {
    if (errors) return <AlertCircle className="text-red-400" size={20} />;
    if (output) return <CheckCircle className="text-green-400" size={20} />;
    return <Terminal className="text-gray-400" size={20} />;
  };
  const getStatusText = () => {
    if (errors) return 'Error de compilación / ejecución';
    if (output) return `Compilado correctamente (${compilationTime} ms)`;
    return 'Listo para compilar';
  };

  return (
    <div className="min-h-screen bg-gradient-to-br from-gray-900 via-gray-800 to-orange-900">
      {/* HEADER */}
      <header className="bg-gray-900/80 backdrop-blur-sm border-b border-orange-500/20 px-6 py-4">
        <div className="flex items-center justify-between max-w-7xl mx-auto">
          <div className="flex items-center space-x-3">
            <div className="w-10 h-10 bg-gradient-to-r from-orange-500 to-red-500 rounded-lg flex items-center justify-center">
              <Code className="text-white" size={24} />
            </div>
            <div>
              <h1 className="text-2xl font-bold text-white">RustForge</h1>
              <p className="text-orange-300 text-sm">Compilador Online de Rust</p>
            </div>
          </div>
          <div className="flex items-center space-x-2">
            {getStatusIcon()}
            <span className="text-sm text-gray-300">{getStatusText()}</span>
          </div>
        </div>
      </header>

      <div className="max-w-7xl mx-auto p-6">
        {/* TOOLBAR */}
        <div className="bg-gray-800/50 backdrop-blur-sm rounded-lg border border-gray-700 p-4 mb-6">
          <div className="flex flex-wrap items-center justify-between gap-4">
            <div className="flex items-center space-x-3">
              <button
                onClick={() => fileInputRef.current.click()}
                className="flex items-center space-x-2 px-4 py-2 bg-orange-600 hover:bg-orange-700 text-white rounded-lg"
              >
                <Upload size={18} /> <span>Subir .rs</span>
              </button>
              <button
                onClick={handleDownload}
                className="flex items-center space-x-2 px-4 py-2 bg-gray-600 hover:bg-gray-500 text-white rounded-lg"
              >
                <Download size={18} /> <span>Descargar</span>
              </button>
              
            </div>
            <button
              onClick={handleCompile}
              disabled={isCompiling}
              className={`flex items-center space-x-2 px-6 py-3 rounded-lg font-semibold transition transform ${
                isCompiling
                  ? 'bg-gray-600 cursor-not-allowed'
                  : 'bg-gradient-to-r from-green-600 to-green-700 hover:scale-105 shadow-lg'
              } text-white`}
            >
              {isCompiling
                ? <>
                    <RefreshCw size={20} className="animate-spin" />
                    <span>Compilando...</span>
                  </>
                : <>
                    <Zap size={20} />
                    <span>Compilar y Ejecutar</span>
                  </>}
            </button>
          </div>

          {showSettings && (
            <div className="mt-4 p-4 bg-gray-700/50 rounded-lg border border-gray-600">
              <div className="grid grid-cols-1 md:grid-cols-2 gap-4">
                <div>
                  <label className="block text-sm text-gray-300 mb-1">
                    Nivel de Optimización
                  </label>
                  <select
                    value={optimizationLevel}
                    onChange={e => setOptimizationLevel(e.target.value)}
                    className="w-full bg-gray-600 text-white rounded border border-gray-500 px-3 py-2"
                  >
                    <option value="O0">O0 – Sin optimización</option>
                    <option value="O1">O1 – Básica</option>
                    <option value="O2">O2 – Media</option>
                    <option value="O3">O3 – Máxima</option>
                  </select>
                </div>
              </div>
            </div>
          )}

          <input
            type="file"
            accept=".rs,.txt"
            ref={fileInputRef}
            style={{ display: 'none' }}
            onChange={handleUpload}
          />
        </div>

        {/* EDITOR */}
        <div className="bg-gray-800/50 backdrop-blur-sm rounded-lg border border-gray-700 mb-6">
          <div className="bg-gray-700/50 px-4 py-3 border-b border-gray-600 flex items-center space-x-2">
            <FileText size={18} className="text-orange-400" />
            <span className="text-white font-medium">Editor de Código</span>
          </div>
          <MonacoEditor
            height="300px"
            language="rust"
            theme="vs-dark"
            value={code}
            onChange={setCode}
            options={{
              fontSize: 14,
              minimap: { enabled: false },
            }}
          />
        </div>

        {/* OUTPUT / ASM */}
        <div className="space-y-6">
          <div className="bg-gray-800/50 backdrop-blur-sm rounded-lg border border-gray-700 overflow-hidden">
            <div className="bg-gray-700/50 px-4 py-3 border-b border-gray-600 flex items-center justify-between">
              <div className="flex items-center space-x-2">
                <Terminal size={18} className="text-green-400" />
                <span className="text-white font-medium">Salida del Compilador</span>
              </div>
              <div className="flex bg-gray-600 rounded-lg p-1">
                <button
                  onClick={() => setView('executed')}
                  className={`px-3 py-1 text-sm rounded ${
                    view === 'executed'
                      ? 'bg-orange-600 text-white'
                      : 'text-gray-300 hover:text-white'
                  }`}
                >
                  Ejecutado
                </button>
                <button
                  onClick={() => setView('assembly')}
                  className={`px-3 py-1 text-sm rounded ${
                    view === 'assembly'
                      ? 'bg-orange-600 text-white'
                      : 'text-gray-300 hover:text-white'
                  }`}
                >
                  Ensamblador
                </button>
              </div>
            </div>
            <div className="relative">
              <pre className="h-64 overflow-auto bg-gray-900 text-gray-100 font-mono text-xs p-4 whitespace-pre-wrap">
                {view === 'executed' ? output : assembly}
              </pre>
              {(output || assembly) && (
                <button
                  onClick={() => copyToClipboard(view === 'executed' ? output : assembly)}
                  className="absolute top-2 right-2 p-2 bg-gray-700 hover:bg-gray-600 text-gray-300 rounded"
                  title="Copiar"
                >
                  <Copy size={16} />
                </button>
              )}
            </div>
          </div>

          {errors && (
            <div className="bg-red-900/20 backdrop-blur-sm rounded-lg border border-red-500/30 overflow-hidden">
              <div className="bg-red-800/30 px-4 py-3 border-b border-red-500/30 flex items-center space-x-2">
                <AlertCircle size={18} className="text-red-400" />
                <span className="text-white font-medium">Errores</span>
              </div>
              <pre className="h-32 overflow-auto bg-red-950/50 text-red-100 font-mono text-xs p-4 whitespace-pre-wrap">
                {errors}
              </pre>
            </div>
          )}

          {/* Footer */}
        <footer className="mt-12 text-center">
          <div className="bg-gray-800/30 backdrop-blur-sm rounded-lg border border-gray-700 p-6">
            <div className="flex items-center justify-center space-x-8 text-gray-300">
              <div className="text-center">
                <div className="text-orange-400 font-semibold">Desarrollado por:</div>
                <div className="mt-1">Eduardo Aragon • Mikel Bracamonte • Laura Nagamine</div>
              </div>
            </div>
            <div className="mt-4 text-xs text-gray-500">
              RustForge v1.0 - Compilador Online de Rust con soporte para múltiples arquitecturas
            </div>
          </div>
        </footer>
        </div>
      </div>
    </div>
  );
}
