# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),


## [Unreleased]
### Added
- Update and improvement GUI backend.
- Update and improvement on memory performance.



## [1.3.0] - 2019-03-15
### NEW
- NEW component - BASE - Finder - raise "found"/"not found" if the property of a component (path) is equal to "something"
- NEW machanism for find_component: '//' to start research from ROOT. 
	eg: find_component ("//foo")  from ROOT give me 'foo' component.
- NEW machanism for find_component: '*' research recursively to find the first corresponding element. 
 	eg: find_component ("foo/*/bar") from 'foo' component give me the first 'bar' component you found.
- NEW experimental backend: SDL/Cairo
 	eg: in config.mk change line : "graphics := QT" -> "graphics := CAIRO"

### Added
- added warning on image loading if file is not found
- added watcher on image path. You can now bind to path to modify
- added tools to generate backend's code more easily : tools/gen_prop.py
- remade shape.h and added cicle.h, ellipse.h, line.h, rectangle.h, rectangleclip.h
- added pick() function to Process.

### Changed
- improved the execution graph performance : sort, browse_in_depth and remove_properties, _timestamp
- changed mechanism to access picking_view directly from. 
- improved Thread mechanism.
- improved mainloop mechanism.
- improved djnn-lib.mk files.
- improved README.md with more install info.
- improved Makefile.
- improved Image quality display by Qt (set smoothPixmalTransform filter by default)
- improved backend code : auto-generated
- cleaned up code

### Fixed
- fixed Windows (10/8) compilation.
- fixed delete on all backend objects.
- fixed Memory leaks: Touch, Path, abstract_gshape, set, Text

### Deprecated
- renamed draw_rect -> draw_rectagnle function
- reanmed dran_rect_clip -> draw_rectangleclip function

### Removed
- remove useless draw in FSM.



## [1.2.1] - 2019-02-25 [YANKED]
### Fixed
- fixed move_child_to and add_child_at in djnn core and gui



## [1.2.0] - 2019-02-15
### Added
- cleaned code - especially delete and destructeur.
- Improved Makefile and compilation on Linux.
- Switches can now have unknown branches, use such as default branch and deactivate the current branch if running.
- improved PERF_TEST in gui.cpp.

### Fixed
- improved delete: GUI text, GUI window, FSM
- fixed Mainloop thread creation for Windows.
- fixed side effect in Switch destructor.
- fixed Container destructor, now, delete children items from structure observers 

### Changed
- GUI/cache - x, y, dx, dy ... now notify damaged transform.
- renamed cpp-time into cpp-chrono.



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
### NEW
- NEW anti-binding mechanism
- NEW basic support for Linux display

### Added
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


