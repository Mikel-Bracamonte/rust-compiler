export async function compileRustCode({ code, optimizationLevel }) {
  const res = await fetch('http://localhost:8000/compile', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ code, optimizationLevel }),
  });
  if (!res.ok) {
    const text = await res.text();
    throw new Error(text || res.statusText);
  }
  return res.json(); // { output: string, assembly: string, error?: string }
}
