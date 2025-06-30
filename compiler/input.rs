struct Test {
    a: i32,
    b: i32,
}

struct Persona {
    a: i32,
    b: Test,
}

fn main() {
    let a: i32 = 10;
    let b: i32 = 20;
    println!("{}", test());
    println!("{}", a);
    println!("{}", b);
}