struct Persona {
    a: i32,
    b: i32,
    c: i32,
    d: i32,
    e: i32,
    f: i32
}

fn main() {
    println!("{}", Persona {
        a: 1,
        b: 2,
        c: 3,
        d: 4,
        e: 5,
        f: 6
    }.b);
}