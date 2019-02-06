# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),


## [Unreleased]
### Added
- Update and improvement GUI backend.
- Update and improvement on memory performance.



## [1.1.2] - 2017-02-06 [YANKED]
### Added
- CHANGELOG.md

### Fixed
- fixed gradient loaded from SVG file


## [1.1.1] - 2017-02-04 [YANKED]
### Fixed
- API - AbstractOpacity API, replaced bad name "alpha" by "a".
- activated frame/windows touches.


## [1.1.0] - 2019-02-01
### Added
- added anti-binding.
- added basic support for Linux display
- Makefile : add rule make install-pkgdeps for all platform (MacOS, Linux, Windows).
- compilation on Windows (8/10) is working.
- properties are now dynamically generated for GUI (shape/transform/style) component.
- added cache on GUI component for redraw process.
- improved graphics performances.
- improved memory performances.
- improved qt_picking_view.
- improved Makefile.
- improved Timer.
- support for boost:Fiber.
- added PERF_TEST : graph.cpp, qt_window.cpp.
- cleaned code and Makefile.

### Changed
- renamed DEBUG variable for picking.
- changed default style attribute values to comply with SVG spec.
- activate matrix on gui component only when necessary.
- replaced iSModel and activation_flag fields with bitset and Macro access.
- API - new  API for state management.
- API - uniformised API for GUI backend.
- API - to use time : 1000ms -> 1. 

### Fixed
- memory leak and performances on execution graph.
- Makefile for Linux.


## [1.0.0] - 2018-12-17
note: debut on changelog. This section has to be completed, if we have time. 
### Added
### Changed
### Deprecated
### Removed
### Fixed
### Security


