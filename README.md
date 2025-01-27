# rSlideShow

This is a desktop program which renders slideshows. It uses a custom file extension called <code>.rslide</code>. Check the .rslide SPECIFICATION section for more info.

## Features

- Renders text and images with transparency channel
- Transitioning between slides

## How to use

- <code>git clone</code> this repo
- Run the command <code>build</code>
- Run the <code>bin/rslideshow.exe</code>

## <code>.rslide</code> Specification

Each .rslide file has three types of "entities", which are texts, images and background.

- For text entities, they should start with the <code>[TXT]</code> string. The parameters are:
	- (X, Y) positions (which go from (0.0, 0.0) (top-left corner) to (1.0, 1.0) (bottom-right corner))
	- color (in hexadecimal, starting with 0x)
	- text (string surrounded by double quotes. e.g.: "Hello World")
	- font_size (a non negative integer number)
	- font (a string with the name of a font)

- For image entities, they should start with the <code>[IMG]</code> string. The parameters are:
	- (X, Y) positions (which go from (0.0, 0.0) (top-left corner) to (1.0, 1.0) (bottom-right corner))
	- width (for the image's width, a non negative integer number)
	- height (for the image's height, a non negative integer number)
	- file_path (string surrounded by double quotes representing the path to the image file)

- For image entities, they should start with the <code>[BACKGROUND]</code> string. The parameters are:
	- color (in hexadecimal, starting with 0x)

Other Rules:
	- [TXT], [IMG] and [BACKGROUND] need equal sign right after.
	- Each one of the parameters must have an equal sign right after and also must be separated by new lines ('\n').
	- Parameters from a specific entity must be surrounded by curly braces.

See the <code>.rslide</code> files in the <code>test_slides</code> folder for some examples.

## Technologies Used

- C99 (gcc for now)
- SDL2
- SDL2_TTF
- OpenGL
- stb_image ([Sean Barret](https://github.com/nothings/stb))
