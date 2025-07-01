fn main() {
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
    if n <= 1 {
        1
    } else {
        n * factorial(n - 1)
    }
}