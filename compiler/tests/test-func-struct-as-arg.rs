struct Test1 {
    a: i32,
    b: i32,
}

struct Test2 {
    a: i32,
    b: Test1,
}

fn test(t2: Test2) {
    t2.b.a %= 3;
}

fn main() {
    let var1: Test1 = Test1 {
        a: 50,
        b: 25,
    };
    let var2: Test2 = Test2 {
        a: 20,
        b: var1,
    };
    println!("{}", var2.b.a);
    test(var2);
    println!("{}", var2.b.a);
}