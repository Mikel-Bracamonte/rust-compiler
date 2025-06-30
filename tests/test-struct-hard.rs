struct T1 {
    a : i32,
    b : i32,
}

struct T2 {
    a : T1,
    b : i32,
}

struct T3 {
    a : T2,
    b : T1,
}

struct T4 {
    a : T3,
    b : T3,
}

struct T5 {
    a : T4,
    b : T1,
}

fn main() {
    let t1 : T1 = T1 {
        a: 1, 
        b: 2,
    };
    
    let t2 : T2 = T2 {
        a: t1, 
        b: 3,
    };
    
    let t3 : T3 = T3 {
        a: t2, 
        b: t1,
    };
    
    let t4 : T4 = T4 {
        a: t3, 
        b: t3,
    };
    
    let t5 : T5 = T5 {
        a: t4, 
        b: t1,
    };
    println!("{}", t5.a.b.a.a.b);
    println!("{}", t5.a.a.a.a.b);
    
}