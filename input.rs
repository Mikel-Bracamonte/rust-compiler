struct Inner {
    x: i32,
    y: i32
}

struct Middle {
    m1: i32,
    m2: Inner,
    m3: i32
}

struct Outer {
    o1: Middle,
    o2: i32
}

fn make_inner(x: i32, y: i32) -> Inner {
    return Inner { x: x + 1, y: y + 2 };
}

fn make_middle(a: i32, b: i32) -> Middle {
    let inner: Inner = make_inner(a, b);
    return Middle {
        m1: a * b,
        m2: inner,
        m3: inner.x + inner.y
    };
}

fn make_outer(p: i32, q: i32) -> Outer {
    let mid: Middle = make_middle(p + 1, q + 2);
    return Outer {
        o1: mid,
        o2: p - q
    };
}

fn main() {
    let final_struct: Outer = make_outer(10, 20);
    println!("{}", final_struct.o1.m2.y + final_struct.o1.m3 + final_struct.o2);
    let test: Outer = Outer {
        o1: Middle {
            m1: 1,
            m2: Inner {
                x: 1,
                y: 2
            },
            m3: 2
        },
        o2: 0
    };
    println!("{}", test.o1.m2.y);
}