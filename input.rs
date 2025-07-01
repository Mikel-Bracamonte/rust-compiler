struct T1 {
    a : i32,
    b : i32,
}

fn main() {
    let x : T1 = if 1 > 0 {T1 {a : 2, b: 2,}} else {T1 { a: 3, b : 3,}};
    println!("{}", x.a);
}