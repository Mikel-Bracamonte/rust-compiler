fn main() {
    let a: i32 = 4;
    let mut b: i32 = 2;
    b = 3;
    let c: i32 = a + b * (a - b);
    println!("{}", c);
}