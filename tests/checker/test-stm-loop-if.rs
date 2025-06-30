fn main(){
    if(10){
        println!("{}", 10);
    }
    //
    while(10) {
        println!("{}", 10);
    }
    //
    while(true) {
        println!("{}", 10);
        break;
        continue;
    }
    continue;
    break;
    //
    if(true){
        let mut x:i32 = 10;
        println!("{}", x);
    }
    println!("{}", x);
    //accepted:
    for mut i in 1..3 {
        println!("{}", i);
    }
    //
    for mut i in 1..3 {
        println!("{}", i);
        break;
        continue;
    }
    break;
    continue;
    //
    for mut i in 1..3 {
        println!("{}", i);
    }
    println!("{}", i);
}