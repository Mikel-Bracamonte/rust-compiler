fn main() {
    let mut x : i32;
    x = 2;
    println!("{}", x);
    print!("{}", if true {10} else {20});
    println!("{}", if !(false) {10} else {20});

    print!("{}", -5);
    println!("{}", 8/3);

    let mut y : i32;
    y = 10;
    println!("{}", y+3);
}