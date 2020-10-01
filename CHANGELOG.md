# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),


## [Unreleased]
### Added
    - Update the Docs !

## [1.13.0] - 2020-10-01

### NEW
    - NEW multiple-assignment component
    - NEW multiple-connector 
    - NEW easing function animation component
    - NEW - Qt Only - screenshoot service on Frame/window component

### Added
    - Added left/right/middle buttons in shape’s UI
    - Added debuginfo to in-svg processes
    - Added few methods to process handler
    - Added NativeCollectionAction componant
    - Added preprocessing condition for serialized
    - Added mainloop auto-deactivation and cleanup
    - exec_env - added missing copyright header

### Changed
    - make thread = STD standart option for Linux
    - simplified native_async_action code
    - Improved INSTALL.md instructions
    - Ivy make a graph_exec on each messages not on each value (method 2) by default
    - Set NONE_ACTIVATION in graph::exec () instead of post_deactivation
    - IvyAccess react on each IvyOutAction
    - Factorize abstract_obj generation
    - Improved properies code
    - Improved support for FreeRTOS/crazyfly
    - Improved coverage checking
    - Clean-up useless code 

### Removed
    - Removed the output_node using clear_display
    - Removed useless serialiaze for action 

### Fixed
    - Fixed signedness warnings
    - Fixed thread_local_cancelled for windows
    - Fixed support for Qt 5.12 on Ubuntu 20.04
    - Fixed loadFromXML
    - Fixed a bug in clipping/picking management
    - Fixed Finder component
    - Fixed dictionary destructor
    - Fixed destructor of ProcessCollector, CollectionActivator
    - Fixed deref destructor
    - Fixed MinList, ProductList and SumList
    - Fixed bug on set : removed_child
    - Fixed connector as it should be on the model, using copy_on_activation
    - Fixed double assignment activation in connector
    - Fixed binding as it should be on the model
    - Fixed dx, dy, reset on mouse wheel event
    - Fixed ordering graph between native_expression_action and assignment_sequence component
    - Fixed ordering graph in regex 
    - Fixed serialize names for binaryOperators
    - Fixed destructor of ListOperator
    - Fixed some warnings


## [1.12.0] - 2020-06-25

### NEW
    - Main NEWS: General improvement of all the code by separation of Process's code into 3 kind of processes.
     A lot of change has been made for this and really detailed explanation will be useless. 
     We'll just point API change if necessary. For now, Process is used as CoreProcess.

    - NEW rtmidi in comms module
    - NEW able to generate .deb package for ubuntu/debian
    - NEW NativeAsyncAction component: Natives Action asynchronous
    - NEW process handler : new components designed to ease process management : ProcessDeleter , CollectionDeleter, CollectionActivator, ProcessCollector


### Added
    - Added in debug.h several options to displau debug informations  :
        - _DEBUG_SEE_COLOR_PICKING_VIEW: to view picking view debug
        - _DEBUG_SEE_GUI_INFO_PREF: to view time informations on display
        - _DEBUG_SEE_GRAPH_INFO_PREF: to view time information on graph execution
        - _DEBUG_SEE_CREATION_DESTRUCTION_ORDER: to display order component creatation
        - _DEBUG_SEE_ACTIVATION_SEQUENCE: to display activation/deactivation sequence per graph execution.
    - Added method get_pixel () on Qt backend. 
    - Managed state dependency in synchronizer
    - Added missing edges into fsm code.
    - Added press and move methods on Graphical object


### Changed
    - Picking now call press and move methods on graphical objects.
    - Spike, action, incr, assignment, properties, assignmentsequence, list, set components now use post_activation_auto_deactivation
    - Improved code optimisaton

### Removed
    - Removed useless function redefinition.

### Fixed
    - Fixed the activation coupling if the coupling is created in find_child method
    - Fixed coverage test
    - Fixed compilation on window : mingw64
    - Fixed ref_property get_double () function


## [1.11.0] - 2020-05-07

### NEW
    - NEW deref double component
    - NEW image_data component to prepare graph visualization 
    - NEW simple_connector component

### Added
    - Added c++ operators for abstract properties
    - Added a bool property : pickable for graphical shaps management
    - Added notify_activation () to all process that deactivate by themselves

### Changed
    - now update text size before rendering, not on creation
    - Improved run_stats utility
    - Imporoved error and warning messages
    - Spike now active on somehow_activating
    - Gave name to: mainloop, ExternalSource, to facilitate debugging
    - Made finalize_construction public and callable for native_expression_action
    - Improved code

### Fixed
    - Fixed simple_binding and simple assignment component
    - Fixed compilation issues


## [1.10.0] - 2020-04-03

### NEW
    - Created extra module : phidgets, crazyflie
    - New module crazyflie
    - New Deref component in Base module: dereferencement use in binding, connector, assignment
    - New DerefString component 
    - NEw Regex component: use of regular expression.
    - New serial communication component in comms module
    - New config.default.mk: Please remove your config.mk
    - New Simple_binding component
    - New Simple_assignement Component

### Added
    - Makefile: control verbosity with level: V= 0, 1, 2, 3, max
    - Makefile: support for pkg-config
    - Makefile: "make install" rules and "make install prefix="
    - Makefile: ARFLAGS
    - Makefile: new tree architecture in build_dir
    - Makefile: depend on ivy-c pkgconfig (libivy-3.15)
    - Makefile: stylized messages
    - Makefile: add sanitizer undifined flags
    - Added more support for FreeRTOS
    - Added Cairo matrix, inverted_matrix for graphical shapes

### Changed
    - Changed find_component -> find_child
    - Changed get_cpnt_type -> get_process_type
    - Changed COMPONENT_T -> CONTAINER_T
    - A lot of work on reducing size memory component, dynamic_cast
    - Remade dependancy on symtable mechanism
    - Remade error and warning messages
    - Improved code, independence between modules
    - Cleaned cade, namespaces and headers for compilation perf
    - Improved Makefile and compilation
    - Changed INSTALL.md

### Fixed
    - Fixed timer scheduling warning
    - Fixed clock cancel warning
    - Fixed radial gradient for Cairo
    - Fixed exit on error macro


## [1.9.0] - 2020-03-13
    note: debut on changelog. This section has to be completed, if we have time. 

### NEW
    - NEW backend: kms_drm
    - NEW FileWriter Component
    - SVG opacity is now a property that can be changed

### Added
    - Added new sort algorithms: merge_sort, stable_quick_sort
    - Added few operators over lists: Sum, Product, Max, Min
    - Added 'mspf' child to windows component: millisecond per frame informations
    - Added 'reset' child to Timer component
    - Added 'end' child to Sample (audio) component
    - Added audio style: Volume, gain
    - Added clear_gui ()
    - Added remotery option compilation
    - Added _refresh_rate on Cairo backend
    - Added opacity for image on Qt backend
    - Notify mainloop start
    - Notify frame closed

### Changed
    - Remaded Timemanager
    - Updated gen_prop.py to current status
    - Improved multi_plateform compilation
    - Improved use of IvyAccess (through smala)
    - Improved Makefile: change default C compiler depend on platform, cross_prefix
    - Improved debug info on nb_drawing_objects
    - Changed syshook is now a module and become exec_env
    - IvyAccess: execute GRAPH_EXEC on each message

### Removed
    - Removed Antialiasing on color_picking
    - Removed reference to macport in compilation (macOs)

### Fixed
    - Fixed Timemanager bugs: cancel, reset, precision
    - Fixed thread races: thread join, MouseButttonDblClick
    - Fixed audio headers
    - Fixed bug on font family
    - Fixed a bug on gradient transform parsing


## [1.8.0] - 2020-01-31
### NEW
    - NEW sorter component
    - NEW audio Module (possibility to play wave sound)
    - NEW support for incorporated images in svg files
    - NEW defs components to enhance SVG support
    - NEW a field « value » in colors and  method to get value from is name
    - NEW clamp_min and clamp_max component

### Added
    - Added implementation of a timemanager (internal component)
    - Added the use of sanitizer=(Address, memory) option in Makefile
    - Added the use of scan-build (llvm) option in Makefile
    - Added an update drawing method to be triggered on tree restructuring
    - Added swap_children in container
    - Start working on emscripten compilation
    - Start working on analytical picking
    - Added partial support for USE and DEFS in SVG parsing
    - Added a partial implementation of a CSS parser
    - Added debug MACROS
    - Added Picking method choice in Makefile

### Changed
    - Changed gradient default values to fit with SVG spec
    - Removed all default constructors and especially process ()
    - Changed some constant names to be usable from Smala
    - Improved makefile rule: install-pkgdeps and add upgrade-pkgdeps rule
    - Better gen_prop.py code
    - Moved abstract_gshape to shapes/
    - clean-up includes 

### Removed
    - Get rid of gperf
    - Removed _name in proc, and make parenting code consistent in constructors

### Fixed
    - Fixed container method move_child() - child are now correctly moved.
    - Fixed IvyAccess - preventing overflow error
    - Fixed return value for TextComparator
    - Fixed a bug in gradient management
    - Fixed gradient a now aware of the current opacity
    - Fixed delete on fsm : reverse delete of _transition and _states
    - Fixed the use of find_child ('//') --> returning the root tree
    - Fixed bug on native_expression_action (delete / clean-up graph)
    - Fixed Clock with negative duration
    - Fixed thread on exit and thread race
    - Fixed leaks on threads
    - Fixed a lot of memory leaks
    - Fixed typos
    - Fixed compilation


## [1.7.2] - 2019-11-28 [YANKED]
### Fixed
    - Fixed thread for Qt < 5.10

## [1.7.1] - 2019-11-21 [YANKED]
### Changed
    - Added instructons for Ivy in INSTALL.md

### Fixed
    - Remove potential null pointer access
    - Fixed a bug in children reordering


## [1.7.0] - 2019-11-18
### NEW
    - NEW text field component
    - NEW implement schedule_activation processes and use it in picking, mouse and touch events
    - NEW implement schedule_delete processes
    - NEW core, base, comms, animation component use plain objects for subchildren.
    - NEW Macro for constant keys (QT >= 5.12)
    - NEW find_child (index) : simplify list use
    - NEW support for FreeRTOS (just start) : add static lib

### Added
    - Added graphics option in config.mk
    - Added CAIRO text input
    - Added tools : size.cpp for size measuring of component
    - Added tools : ivy-c patch to apply for windows
    - Added getter/stter on FatProcess component

### Changed
    - Important code refactoring in base and core module
    - Imporved Mainloop and thread management
        - timer and clock leave their thread if they are dead
        - add protection against thread destruction
        - use thread sanitizer
        - fixed dead lock in Qt ans use boost thread with qt
    - Improved UI event management
    - Cleaned color_picking when object are deactivated
    - Changed transform homography propagation 
    - Reorganized constructors and destructors
    - Cleaned code

### Removed
    - Removed use of std::onceFlag and change all unique instances to plain objects

### Fixed
    - Fixed the use of refproperty/$value in assignment, binding and connector
    - Fixed IvyAccess thread
    - Fixed and removes unnecessary string copy
    - Fixed sanitizer address
    - Fixed pointer leaks
    - Fixed Linux/windows compilation
    - Fixed move include in .cpp file
    - Fixed typo


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
    - FatProcess can only have 1 execution graph vertex associate and vice versa

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
    - Replaced FatProcess (parent, name) by FatProcess (name)
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
    - Replaced FatProcess::finalize by FatProcess::finalize_construction
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


## [1.3.0] - 2019-03-15
### NEW
    - NEW component - BASE - Finder - raise "found"/"not found" if the property of a component (path) is equal to "something"
    - NEW machanism for find_child: '//' to start research from ROOT. 
        eg: find_child ("//foo")  from ROOT give me 'foo' component.
    - NEW machanism for find_child: '*' research recursively to find the first corresponding element. 
        eg: find_child ("foo/*/bar") from 'foo' component give me the first 'bar' component you found.
    - NEW experimental backend: SDL/Cairo
        eg: in config.mk change line : "graphics := QT" -> "graphics := CAIRO"

### Added
    - Added warning on image loading if file is not found
    - Added watcher on image path. You can now bind to path to modify
    - Added tools to generate backend's code more easily : tools/gen_prop.py
    - Remade shape.h and added cicle.h, ellipse.h, line.h, rectangle.h, rectangleclip.h
    - Added pick() function to FatProcess.

### Changed
    - Improved the execution graph performance : sort, browse_in_depth and remove_properties, _timestamp
    - Changed mechanism to access picking_view directly from. 
    - Improved Thread mechanism.
    - Improved mainloop mechanism.
    - Improved djnn-lib.mk files.
    - Improved README.md with more install info.
    - Improved Makefile.
    - Improved Image quality display by Qt (set smoothPixmalTransform filter by default)
    - Improved backend code : auto-generated
    - Cleaned up code

### Fixed
    - Fixed Windows (10/8) compilation.
    - Fixed delete on all backend objects.
    - Fixed Memory leaks: Touch, Path, abstract_gshape, set, Text

### Deprecated
    - Renamed draw_rect -> draw_rectagnle function
    - Reanmed dran_rect_clip -> draw_rectangleclip function

### Removed
    - Remove useless draw in FSM.


## [1.2.1] - 2019-02-25 [YANKED]
### Fixed
    - Fixed move_child_to and add_child_at in djnn core and gui


## [1.2.0] - 2019-02-15
### Added
    - Cleaned code - especially delete and destructeur.
    - Improved Makefile and compilation on Linux.
    - Switches can now have unknown branches, use such as default branch and deactivate the current branch if running.
    - Improved PERF_TEST in gui.cpp.

### Fixed
    - Improved delete: GUI text, GUI window, FSM
    - Fixed Mainloop thread creation for Windows.
    - Fixed side effect in Switch destructor.
    - Fixed Container destructor, now, delete children items from structure observers 

### Changed
    - GUI/cache - x, y, dx, dy ... now notify damaged transform.
    - Renamed cpp-time into cpp-chrono.


## [1.1.2] - 2019-02-06 [YANKED]
### Added
    - CHANGELOG.md

### Fixed
    - Fixed gradient loaded from SVG file


## [1.1.1] - 2019-02-04 [YANKED]
### Fixed
    - API - AbstractOpacity API, replaced bad name "alpha" by "a".
    - Activated frame/windows touches.


## [1.1.0] - 2019-02-01
### NEW
    - NEW anti-binding mechanism
    - NEW basic support for Linux display

### Added
    - Makefile : add rule make install-pkgdeps for all platform (MacOS, Linux, Windows).
    - Compilation on Windows (8/10) is working.
    - Properties are now dynamically generated for GUI (shape/transform/style) component.
    - Added cache on GUI component for redraw process.
    - Improved graphics performances.
    - Improved memory performances.
    - Improved qt_picking_view.
    - Improved Makefile.
    - Improved Timer.
    - Support for boost:Fiber.
    - Added PERF_TEST : graph.cpp, qt_window.cpp.
    - Cleaned code and Makefile.

### Changed
    - Renamed DEBUG variable for picking.
    - Changed default style attribute values to comply with SVG spec.
    - Activate matrix on gui component only when necessary.
    - Replaced iSModel and activation_flag fields with bitset and Macro access.
    - API - new  API for state management.
    - API - uniformised API for GUI backend.
    - API - to use time : 1000ms -> 1. 

### Fixed
    - Memory leak and performances on execution graph.
    - Makefile for Linux.

## [1.0.0] - 2018-12-17
    note: debut on changelog. This section has to be completed, if we have time. 

### NEW
### Added
### Changed
### Deprecated
### Removed
### Fixed


