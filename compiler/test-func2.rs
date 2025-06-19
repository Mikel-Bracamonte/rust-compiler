fn doble(x: i32) -> i32 {
    return x * 2;
}

fn ejecutar_callback(f: fn(i32) -> i32, val: i32) -> i32 {
    return f(val);
}

fn main() {
    let resultado: i32 = ejecutar_callback(doble, 5);
    println!("{}", resultado);
}
