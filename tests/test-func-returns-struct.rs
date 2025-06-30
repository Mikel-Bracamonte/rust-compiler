struct Test1 {
    a: i32,
    b: i32,
}

fn test(a: i32, b: i32) -> Test1 {
    return Test1 {a: a, b: b,};
}

fn main() {
    println!("{}", test(1, 2).b);
}