# PhotoGeoCmd

Commandline tool used to generate contour geometry from a photo of a drawing. Also serves as an example of how to use the library.

## Options

| Option | Description |
| ---  | --- |
| -i   | Specify filename of source image. |
| -o   | Specify filename of result SVG. |
| -b   | Specify background color. Format: R:G:B |
| -f   | Specify foreground color. Format: R:G:B |
| -tp  | Test image processing. Results are outputted to PNG. |
| -tq  | Test quantization. Results are outputted to PNG. |
| -tt  | Test tracing. Results are outputted to SVG. |
| -tv  | Test vertex reduction. Results are outputted to SVG. |
| -pt  | Profile time. |
| -pm  | Profile memory. |
| -lo  | Specify filename of log file. |
| -li  | Specify how many times to iterate test. Integer values only. |
| -p0  | Gaussian blur. Image processing method. |
| -p1  | Bilateral filter. Image processing method. |
| -p2  | Median filter. Image processing method. |
| -p3  | Kuwahara filter. Image processing method. |
| -q0  | Euclidean distance in sRGB space. Quantization method. |
| -q1  | Euclidean distance in linear RGB space. Quantization method. |
| -q2  | CIE76. Quantization method. |
| -q3  | CIE94. Quantization method. |
| -q4  | CIE2000. Quantization method. |
| -t0  | Marching squares. Tracing method. |
| -v0  | Don't perform any vertex reduction. Vertex reduction method. |
| -v1  | Douglas-Peucker. Vertex reduction method. |
| -v2  | Visvalingam-Whyatt. Vertex reduction method. |
