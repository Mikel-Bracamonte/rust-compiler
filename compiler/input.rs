struct Test {
    a: i32,
    b: i32,
    c: i32,
}

struct Persona {
    a: i32,
    b: Test,
}

fn main() {
    let var1: Test = Test {
        a: 15,
        b: 25,
        c: 35,
    };
    let var2: Persona = Persona {
        a: 20,
        b: var1,
    };
    println!("{}", var2.b.c);
}