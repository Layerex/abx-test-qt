# abx-test-qt

Qt application for [ABX testing](https://en.wikipedia.org/wiki/ABX_test).

## Features

- Conveniently convert files to desired format and/or bitrate with ffmpeg.
- Choose number of tries and segment of files to perform test with.
- Switch between A, B and X, rewind, seek back/forward at any point of time.

## Dependencies

- qt5-base
- qt5-multimedia
- ffmpeg (needed for audio conversion)

## Build

`cmake . && make`

## Usage

1. Run abx-test-qt.
2. Select files for A and B, number of tries, desired segment of files.
3. Press OK, then start test.
4. Play A, B and X, make your guesses.
5. Inspect your results after test ends.

## TODO

- Migitate click when switching players
