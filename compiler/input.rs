fn add(a: i32, b: i32, c: bool) -> i32 {
    if c {
        return a + b;
    } else {
        return a;
    }
}

fn main() {
    let x: i32 = 2;
    let y: i32 = 10;
    let z: bool = true;
    for a in x..add(x, y, z) {
        if !z {
            continue;
        }
        while(true) {
            break;
        }
    }
    println!("{}", add(add(x, -1*-y, !z), -y, !z));
}