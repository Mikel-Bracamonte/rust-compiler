fn while_0() {
    println!("{}", 1);
    let mut a: i32 = 0;
    while a < 10 {
        a += 1;
        println!("{}", 2);
    }
    return;
}

fn main() {
    while_0();
}