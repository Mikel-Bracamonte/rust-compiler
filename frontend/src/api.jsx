export async function compileRustCode({ code, optimizationLevel, target }) {
  const response = await fetch('http://localhost:8000/compile', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({ code, optimizationLevel, target })
  });
  if (!response.ok) {
    throw new Error('Error al comunicarse con el backend');
  }
  return await response.json();
}
