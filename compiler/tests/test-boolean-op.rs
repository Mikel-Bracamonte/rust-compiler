
fn main() {
    let mut x : i32;
    x = 0;
    if(x > 0){
        println!("{}", 1);
    }
    if(x >= 0){
        println!("{}", 2);
    }
    if(x == 0){
        println!("{}", 3);
    }
    if(x < 0){
        println!("{}", 4);
    }
    if(x <= 0){
        println!("{}", 5);
    }
    let mut y : bool;
    let mut z : bool;
    y = true;
    z = false;
    if(y && z){
        println!("{}", 10);
    }
    else {
        println!("{}", 11);
    }

    if(!y){
        println!("{}", 12);
    }
    else {
        println!("{}", 13);
    }
    

}