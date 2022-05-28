# Video Mapper

This is a simple app for macOS to distort and map a video onto a rectangular surface.

<center>
<img width="100%" alt="separate window for settings" src="https://user-images.githubusercontent.com/28928394/170834062-f5cba216-8911-4d1a-9204-75438fdbbb46.png">
</center>

This App is written in C++ using [Cinder](https://www.libcinder.org/) and the awesome: [Cinder-Warping](https://github.com/paulhoux/Cinder-Warping) by [Paul Houx](https://github.com/paulhoux)

## Download

[See releases to download the newest version.](https://github.com/TobiasGrothmann/VideoMapper/releases)

## Usage

1. Open the video file.
2. Move the projection window onto a separate monitor (projector).
3. Enable edit mode.
4. Drag the corners to the desired positions.
5. (optional) Use scale and offset to only show a certain area of the video.

Voilà!

## Build it yourself

If you'd like to build it yourself you need to [download cinder](https://www.libcinder.org/download) and set the correct `CINDER_PATH` in the xcode project's build settings.