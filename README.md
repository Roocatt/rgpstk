# Roos's GPS Tool Kit (RGPSTK)

[![Build and Test](https://github.com/Roocatt/rgpstk/actions/workflows/workflow.yml/badge.svg)](https://github.com/Roocatt/rgpstk/actions/workflows/workflow.yml)
[![codecov](https://codecov.io/github/Roocatt/rgpstk/graph/badge.svg?token=1LYWL3ZMVB)](https://codecov.io/github/Roocatt/rgpstk)
[![CodeQL](https://github.com/Roocatt/rgpstk/actions/workflows/github-code-scanning/codeql/badge.svg)](https://github.com/Roocatt/rgpstk/actions/workflows/github-code-scanning/codeql)
![GitHub Licence](https://img.shields.io/github/license/Roocatt/piMCP2515?label=licence)

A helpful library and tool collection for handling GPS related tasks.
More tools and functionality will be added over time.

## Notable Features and Functionality

 - NMEA 0183 handling
 - Geographic Distance Calculation
   - Vincenty's Formulae
   - Haversine Formula
   - Spherical Law of Cosines

## Documentation

Documentation is available on the
[GitHub wiki](https://github.com/Roocatt/rgpstk/wiki). The wiki can also
be cloned using git from `https://github.com/Roocatt/rgpstk.wiki.git`.

## Building

```sh
# Generate build files with CMake
cmake .

# Optional: Instead of the above, add -DUSE_PICO_LIB=1 to load the
# Raspberry Pi Pico C SDK and compile for Pico devices. This requires
# the `PICO_SDK_PATH` environment variable to be set.
#
# cmake -DUSE_PICO_LIB=1 . 

# Build
make

# Optional: run tests
make test

# Install library
make install
```

## Issues and Suggestions

Find a bug or problem? Have a suggestion for how RGPSTK could be better,
or any tools or features you would like to see added?

[Open a GitHub issue](https://github.com/Roocatt/rgpstk/issues/new/choose)

## Licence

This software is available under the ISC licence. This is a BSD-like
licence suitable for use in closed-source and/or commercial projects.
