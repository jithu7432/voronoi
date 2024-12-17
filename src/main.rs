use std::{
    io::{self, Write},
    vec,
};

fn main() {
    let width = 400;
    let height = 400;

    let magic = 255;

    let mut image: Vec<usize> = vec![magic; width * height];
    compute_voronoi(&mut image);
    // save_image(&image);

    let mut stdout = io::stdout();
    stdout.write(b"P6\n");
    stdout.write(format!("{} {} {}\n", width, height, magic).as_bytes());

    for i in image {
        let mut bytes: Vec<usize> = vec![0; 3];
        bytes[0] = i & 0x0000FF;
        bytes[1] = i & 0x00FF00;
        bytes[2] = i & 0xFF0000;
        let _ = stdout.write(bytes);
    }
}

// fn save_image(image: &[i32]) {
// }

fn compute_voronoi(image: &mut [usize]) {}
