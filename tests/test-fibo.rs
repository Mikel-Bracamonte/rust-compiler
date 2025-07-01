fn fibo(a: i32) -> i32 {
    if a < 2 {
        return a;
    } else {
        return fibo(a - 2) + fibo(a - 1);
    }
}

fn main() {
    let mut a: i32;
    a = 0;
    while a < 20 {
        println!("{}", fibo(a));
        a = a + 1;
    }
}