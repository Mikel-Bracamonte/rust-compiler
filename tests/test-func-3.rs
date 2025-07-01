fn function(a: i32, b: i32) -> i32 {
    let mut c: i32;
    a = a + 2;
    b = b * 2;
    c = a + b;

    return c;
}

fn main() {
    let mut a: i32;
    a = 2;
    let mut b: i32;
    b = 4;
    println!("{}", function(a, b));
}