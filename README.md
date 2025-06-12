# Mandelbrot Viewer

This project renders and visualizes the Mandelbrot set using Intel TBB for parallel tile-based rendering and OpenCV for displaying the image.

## Building

Dependencies:
- Intel TBB
- OpenCV
- A C++17 compiler and CMake 3.14+

```bash
mkdir build && cd build
cmake ..
make
```

Run the application with `./mandelbrot`.

Use the keyboard to move around (`W`, `A`, `S`, `D`) and zoom with `+`/`-`. Press `q` or `Esc` to exit.
