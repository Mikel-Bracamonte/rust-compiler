
struct T3 {
    a : i32,
    b : i32,
}

struct T1 {
    a : T3,
    b : i32,
}

struct T4 {
    a : T3,
    b : T3,
    c : T1,
}

fn main() {
    
    let t3 : T3 = T3 {
        a: 1, 
        b: 2,
    };
    
    let t1 : T1 = T1 {
        a: t3,
        b: 5,
    };

    let t4 : T4 = T4 {
        a: t3,
        b: t3,
        c: t1,
    };
    
    println!("{}", t4.a.a);
    println!("{}", t4.a.b);
    println!("{}", t4.b.a);
    println!("{}", t4.b.b);
    println!("{}", t4.c.a.a);
    println!("{}", t4.c.a.b);
    println!("{}", t4.c.b);

}