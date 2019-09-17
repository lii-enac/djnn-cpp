# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),


## [Unreleased]
### Added
- UPdate the Docs !
- Update and improvement GUI backend.
- Update and improvement on memory performance.

#--------------------------------------------

## [1.6.0] - 2019-09-17
### NEW
 - New physics module (optional)
 - New Phidgets module (optional)
 - New utils module
 - New simple file reader component
 - New Switch_range component
 - New support GPIO for linux plateform in input module
 - New implementation of multitouch
 - New UI for graphical object working with mouse and touch
 - New ScaleRotateTranslate component behavior for multitouch purpose.
 - Implemented the general concept of dynamic destruction (delete)
 - Process can only have 1 execution graph vertex associate and vice versa

### Added
 - Added internal enum process_type_e
 - Added Action class
 - Added functions in properties : set*, get*
 - Added new functions in execution graph API
 - Added UI touch
 - Added mechanism to secure process/coupling interaction on process delete : about_to_delete_src ()
 - Added some Makefile elements for code coverage
 - Added dump function for set component

### Changed
 - Improved execution graph and model for performances : can't reactivate a component already activate, except components made for it.
 - Improved Management of edges duplicates in execution graph.
 - Improved parent setting : made only once in finalize_construction () or in add_child ().
 - Replaced Process (parent, name) by Process (name)
 - add_state_dependency are now made in set_parent when the parent is set.
 - Blank component deactivates right after his activation (like a spike component)
 - Moved process and coupling on core/ontology directory
 - Vertices_t is now a std::list instead of std::vecor (perf)
 - Coupling can now change src
 - Mainloop can now stop and restart
 - Improved propagation algorithm
 - Improved state dependence mechanism
 - Improved execution flag management
 - Improved the use of auto generation file using gen_prop.py
 - Replaced Process::finalize by Process::finalize_construction
 - Replaced de/activate () by impl_de/activate ()
 - Replaced de/activation () by de/activate ()
 - Replaced set_activate by set_activation_state
 - Replaced browse_in_depth by traverse_depth_first
 - Improved IvyAccess Code.
 - Improved general performance : memory, leak
 - Improved quality code : sonarqube check
 - Improved Makefile mechanism for Windows, Linux, Raspbian and MacOs
 - Cleaned code
 - Improved Install.md

### Fixed
 - Fixed picking deactivation
 - Fixed Rectangle can now be draw with width/heigh < than 1
 - Fixed several component's destructor
 - Fixed enter/leave event management
 - Fixed bug on activation propagation
 - Fixed synchronizer coupling
 - Fixed Linux compilation : udev, qt5.9
 - Fixed Windows compilation
 - Fixed Cairo compilation

#--------------------------------------------

## [1.5.0] - 2019-06-04
### NEW
- New module : display
- New Synchronizer component to synchronise different source of interaction : eg: to react once x and y of the mouse changed.

### Added
- Added accumulated transform props to Homography
- Added get_string method on properties
- Added a new constructor for counter component

### Changed
- Assignments and connectors can now have dynamic source and destination : automatically reassigned
- redefine execution graph : connectors, bindings, unary operators are directly executed => use synchronizer to create a point of synchronisation
- Frame/window component is now owned by display module and not gui anymore : use initDisplay () and #include "display/display.h
- Changed Touch component constructor
- Connector component trigger a copy if activated
- Updated the documentation of the execution model
- Moved qt mainloop in core/syshook/qt
- Touches is now a list instead of set.
- Cleaned Code

### Removed
- Removed Frame/Window component from gui module

### Fixed
- Fixed some component destructors : homography
- Fixed thread mutex lock in some case : conflic between diplay and gui module, launch and stop
- Fixed bug bon picking and enter/leave mechanism
- Fixed compilation issues on Windows platform
- Fixed malinloop for qt 5.9 
- Fixed extern_source and mainloop handling in SDL
- Fixed clock and timer mechanism with new graph execution
- Fixed Locale for non GUI app.

#--------------------------------------------

## [1.4.0] - 2019-04-05
### NEW
- Added New Display API: move frame/window into display. should use init_display ()
- Added Dictionary class Component : see smala->cookbook->multi-touch as exemple.
- Added generic mouse Class for Qt/SDL backend :
    eg: GenericMouse : x, y, move (x, y), left, middle, right and wheel (dx, dy)
- Added virtual "$value" for refproperty as a dereferencing pointer to use with dynamic assignment/connector
    eg: connector (myref/$value/x, foo/x) will connect any component stored in myref by his "x" child to "foo/x" 
- Added dynamic assignment/connector that will react properly on myref/$value
- Possibility to postpone delete at the end of the graph execution: use for touches
- Added new Global variables : mouse_tracking (=0), full_screen (=0), hide_pointer (=0)
- Added support for custom cursor/pointer

### Added
- Can now use relative path for XML or Image loading (or absolute URI)
    eg : XML::djnLoadFromXML (path/to/file) or XML::djnLoadFromXML (file://absolute/path/to/file)
- Added touch support for SDL
- Added pressure to touch
- Added Touch support for Windows
- Enable immediate processing in couplings - low level API

### Changed
- "Touches" is now a list instead of a set 
- Improved Color_picking
- Improved Qt image loading
- Improved Install.md for ivy on windows (not finished)

### Fixed
- Fixed a bug on thread running < Qt-5.10
- Fixed HDPI for 
- Fixed Linux Compilation

#--------------------------------------------

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

#--------------------------------------------

## [1.2.1] - 2019-02-25 [YANKED]
### Fixed
- fixed move_child_to and add_child_at in djnn core and gui

#--------------------------------------------

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

#--------------------------------------------

## [1.1.2] - 2019-02-06 [YANKED]
### Added
- CHANGELOG.md

### Fixed
- fixed gradient loaded from SVG file

#--------------------------------------------

## [1.1.1] - 2019-02-04 [YANKED]
### Fixed
- API - AbstractOpacity API, replaced bad name "alpha" by "a".
- activated frame/windows touches.

#--------------------------------------------

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

#--------------------------------------------

## [1.0.0] - 2018-12-17
note: debut on changelog. This section has to be completed, if we have time. 
### NEW
### Added
### Changed
### Deprecated
### Removed
### Fixed
### Security


