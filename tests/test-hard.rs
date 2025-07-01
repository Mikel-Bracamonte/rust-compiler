fn func2() {
    for i in 1..5 {
        println!("{}", i * i);
    }
}

fn func(c : mut i32, dx : mut i32) -> i32 {
    println!("{}", c * 2 / 2 % 10);
    print!("{}", - dx * 9);
    dx *= 2;
    dx -= 2;
    dx += 2;
    dx %= 10;
    if(dx > 5){
        func2();
        return (dx + c) + 3;
    }
    let mut y : i32 = 10;
    return y;
}

fn main(){
    let mut a:i32;
    a = 10;
    let mut b:i32 = if a % 2 == 0 { a + 2 } else { a - 2 };
    for i in 3+a..a*a {
        while b >= 0 {
            b -= 1;
        }
    }
    print!("{}", func(a, b));
}