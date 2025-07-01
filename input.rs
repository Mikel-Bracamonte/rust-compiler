
struct T1 {
    a : i32,
    b : i32,
}

fn main() {
    let x : T1 = T1{
        a : 2,
        b : 2,
    };
    println!("{}", x.a);
}