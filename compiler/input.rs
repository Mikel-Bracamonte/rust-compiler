fn main() {
    for i in 0..10 {
        i = i * 2;
        for j in i..10 {
            j = j * 2;
            println!("{}", i);
            println!("{}", j);
        }
    }
