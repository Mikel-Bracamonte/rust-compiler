struct Persona {
    a: i32,
    b: i32,
}

fn test() -> i32 {
    let test: Persona = Persona {
        a: 2,
        b: 3,
    };
    let test2: Persona = Persona {
        a: 10,
        b: test.b + 10,
    };

    return test.a + test2.b;
}

fn main() {
    let a: i32 = 10;
    let b: i32 = 20;
    println!("{}", test());
    println!("{}", a);
    println!("{}", b);
}