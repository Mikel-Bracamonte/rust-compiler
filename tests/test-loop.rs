fn main(){
    let mut x : i32;
    x = 0;
    while (x <= 5){
        x = x + 1;
        println!("{}", x);
        while (x <= 7){
            x = x + 2;
            if(x == 5){
                continue;
            }
            println!("{}", x);
        }
    }
    println!("{}", 10);
}