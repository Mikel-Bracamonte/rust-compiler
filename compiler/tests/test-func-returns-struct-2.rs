struct Test1 {
    a: i32,
    b: i32,
}

struct Test2 {
    a: i32,
    b: Test1,
}

fn test(a: i32, b: i32, c: i32, d: i32) -> Test1 {
    return Test2 {a: a, b: Test1 {a: c, b: d,},}.b;
}

fn main() {
    let var1: Test1 = test(1, 2, 3, 4);
    println!("{}", var1.b);
}