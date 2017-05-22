#[macro_use]
extern crate helix;

ruby! {
    class Glare {
        def hello() {
            println!("Hello from glare!");
        }
    }
}
