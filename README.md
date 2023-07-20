# Solaris (work in progress)

![solaris-banner](https://user-images.githubusercontent.com/45200003/236694077-fc4371cf-c827-43bf-8b73-6a56a7df645e.jpg)

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Fast celestial body computation library

- Plain C implementation without any dependencies
- Cross-platform
- Arena allocation for optimal cache use

## Implementation details

- The format of the models must adhere to [this documentation](https://cdsarc.cds.unistra.fr/ftp/VII/118/ReadMe)
- Calculations are based on the book "Astronomie mit dem Personal Computer"
- In some rather unimportant cases, readability is preferred over performance

## Progress

- [x] Transforms
- [ ] Model loading
- [ ] Variable backend (precompiled or dynamic)
- [ ] Rise time calculation
- [ ] Special Events (Alignment of planets, ...)
- [ ] Plate solving
