struct Pair {
    a: i32,
    b: i32,
}

fn make_nested(n: i32) -> Pair {
    println!("{}", n);
    if n <= 0 {
        return Pair { a: 0, b: 0, };
    } else {
        let inner: Pair = make_nested(n - 1);
        println!("{}", inner.a);
        println!("{}", inner.b);
        return Pair {
            a: n + inner.a,
            b: n * inner.b + 1,
        };
    }
}

fn main() {
    let result: Pair = make_nested(1);
    println!("{}", result.a);
    println!("{}", result.b);
}