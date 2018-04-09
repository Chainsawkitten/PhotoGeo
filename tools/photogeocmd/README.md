# PhotoGeoCmd

Commandline tool used to generate contour geometry from a photo of a drawing. Also serves as an example of how to use the library.

## Options

| Option | Description |
| --- | --- |
| -i  | Specify filename of source image. |
| -o  | Specify filename of result SVG. |
| -b  | Specify background color. Format: R:G:B |
| -f  | Specify foreground color. Format: R:G:B |
| -tp | Test image processing. Results are outputted to PNG. |
| -tq | Test quantization. Results are outputted to PNG. |
| -tt | Test tracing. Results are outputted to SVG. |
| -tv | Test vertex reduction. Results are outputted to SVG. |
| -pt | Profile time. |
| -pm | Profile memory. |
| -lo | Specify filename of log file. |
| -li | Specify how many times to iterate test. Integer values only. |
