struct S1 {
    x: i32
}

fn s1(x: i32) -> S1 {
    return S1 {x: x};
}

fn s2(s1: S1) -> S1 {
    println!("{}", s1.x);
    s1.x *= 2;
    return S1 {x: s1.x + 1};
}

fn main() {
    let mut var1: S1 = s2(s2(s2(s1(2))));
    println!("{}", var1.x);
}