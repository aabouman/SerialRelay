fn main() {
    let a = [1, 2, 3, 4, 5];
    let slice = &a[1..3];

    for x in a {
        print!("{} ", x);
    }
    println!();

    for x in slice {
        print!("{} ", x);
    }
    println!();

}
