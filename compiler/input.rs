struct Persona {
    nombre: String,
    edad: u32,
}

fn sumar(a: i32, b: i32) -> i32 {
    a + b
}

fn main() {
    let persona = Persona {
        nombre: String::from("laura"),
        edad: 19,
    };
    println!("Nombre: {}", persona.nombre);
    println!("Edad: {}", persona.edad);
}