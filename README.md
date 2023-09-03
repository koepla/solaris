# Solaris (work in progress)

![solaris-banner](https://user-images.githubusercontent.com/45200003/236694077-fc4371cf-c827-43bf-8b73-6a56a7df645e.jpg)

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Fast celestial body computation library

- Plain C implementation without any dependencies
- Cross-platform
- Builtin planets and celestial objects
- Zero runtime allocations except for dynamic calculations

## Implementation details

- Calculations are based on the book "Astronomie mit dem Personal Computer"
- In some rather unimportant cases, readability is preferred over performance

## Progress

- [x] Basic model
- [x] Transforms
- [x] Builtin planets
- [x] Builtin celestial objects
- [ ] Spatial acceleration ([quadtree](https://youtu.be/ASAowY6yJII?si=GR5N4C_AqIgDLHQc))
- [ ] Special Events (alignment of planets, ...)
- [ ] Plate solving
