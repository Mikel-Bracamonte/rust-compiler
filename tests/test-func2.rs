fn cuadrado(x: i32) -> i32 {
    return x * x;
}

fn doblecuadrado(y: i32) -> i32 {
    return cuadrado(y) * 2;
}

fn main() {
    let resultado: i32 = doblecuadrado(3);
    println!("{}", resultado);
}
