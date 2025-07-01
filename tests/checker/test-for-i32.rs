fn main(){
    let mut id : i32 = 0;
    let mut id2 : i32 = 10;
    
    for mut i in id..(id2) {
        let mut x : i64 = 0;
        x += i;
        println!("{}", i);
    }
}