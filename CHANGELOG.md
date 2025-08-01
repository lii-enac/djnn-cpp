# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),

## [Unreleased]

### Added

    - Update the Docs !

## [1.22.0] - 2025-07-29

### NEW

    - NEW GUIStructureHolder built on the fly
    - NEW Support for multi-assignment and multi-binding across components and graphical objects
    - NEW Global `_SVG_USER_CUSTOM_ATTRS` for parsing custom SVG attributes
    - NEW Global `_STYLUS_EMULATE_MOUSE` for stylus mouse emulation
    - NEW `is_null` property added to `RefProperty`
    - NEW SDL backend updated to SDL3

### Added

    - Added Getter for `GUIStructureHolder::process_content`
    - Remade Support multitouch and interaction behaviors `RR_T` and `RRR_MT`   
    - Added Remotery integration on `GUIStructureHolder::draw`
    - Added `matched` and `include` boolean properties for regex

### Changed

    - Changed APIs `load_from_XML` and `load_from_XML_once` added for parsing custom SVG attributes
    - Changed Child movement tracking in GH containers
    - Improved context save/restore for containers, paths, and polygons
    - List removal now nullifies parent and cleans children
    - Simplified `move_child` and `add_child` logic in tree module
    - Various code cleanups and style fixes in graph, assignment, and properties
    - replaced default option _DEBUG_NO_TOUCH_EVENT = 1 by ENABLE_TOUCHES = 0
    - Removed automatic no-stroke behavior on missing stroke style parsing but existing other styles
    - Makefile and packaging improved for MSYS2 and Debian
    - Formatting scripts improved and sources reformatted

### Fixed

    - Type issues in synchronizer and assignment systems
    - Touch event behavior fixed for hovering non-initial shapes
    - Shape selection under touch input fixed
    - Deletion of `~AbstractRRR` and `~Matrix4x4` fixed
    - Compilation issues fixed in GUI and other modules
    - Null dereference warnings fixed in `gen_prop.py`

### Removed

    - `impl_clone` declarations removed from headers
    - Redundant code in graph and tree modules
    - Deprecated warnings removed in build flags

## [1.21.0] - 2024-07-15

### NEW

    - Added rotation support on Layer in GUI/Qt.
    - Added a script to remove preprocessing in files (tools).
    - Added new component : TextContainer

### Added

    - Added comments to improve documentation in Layer.
    - Added missing options to pen for picking (CapStyle, JoinStyle, MiterLimit) in Qt backend.
    - Added OperationConnector in base/arithm.
    - Added two new mechanisms in IvyAccess: one to react to "die" Ivy messages and another to send a "send_die" message to an Ivy application by name.
    - Added a script to format C++ files using clang-format.

### Changed

    - Renamed w and h to width and height in Layer.
    - Changed z-order management in GUI.
    - Reformatted and improved code structure across multiple modules (FORMAT commits).
    - Changed propagate and lazy to be bool instead of double in core/assignment.
    - Changed regex initialization to use TextProperty.
    - Changed BinaryOperator accessibility in smala.
    - Improved format-clang script and automatic formatting with VSCode.
    - Removed info on CXXFLAGS from Makefile.
    - Reformatted various modules (base, display, comms, exec_env, gui, input, animation, utils, physics, and more).

### Fixed

    - Fixed coupling issue on pad in Layer.
    - Fixed opacity issue in images.
    - Fixed sign multiply defined issue in Layer.
    - Fixed current scale computation in Layer.
    - Fixed debug information in the core module.
    - Fixed __to_string for zero in utils-dev.
    - Fixed incorrect Qt y offset in mouse events and mouse wheel events in Qt backend.
    - Fixed OperationConnector delete memory issue in base/arithm.
    - Fixed issues in IvyAccess destructor related to symbol table cleanup.
    - Fixed getInt, GetDouble, GetString, getBool, and getRef to use AbstractXXXProperty.
    - Fixed includes in audio and gui modules.
    - Fixed methods in DRM window implementation in display.
    - Fixed function call responsible for an infinite loop in display.
    - Fixed regex group processing order in base/regex.
    - Fixed custom cursor images using QPixmap instead of QBitmap.

## [1.20.0] - 2023-10-10

### NEW

    - NEW : phidgets features on LINUX only
    - NEW : FileDialog widget
    - NEW debug option : _DEBUG_SEE_COMPONENTS_DESTRUCTION_TREE to track warning on component destruction
    - NEW debug option : _DEBUG_SEE_COMPONENTS_DESTRUCTION_INFO_LEVEL = 0|1|2 to have several levels of information
    - NEW debug option : _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_LOCATION = file.sma:lineno
    - NEW debug option : _DEBUG_NO_TOUCH_EVENT to enable touch events (they are disable by default due to issues with Qt and apple trackpad)
    - NEW script: make_bundle for MacOs

### Added

    - Added identity transformation
    - Added (de)activate functions on process.
    - Added a wrap algorithm for multiline edit component.
    - Added djnn_mainloop_instance
    - Added skeleton for xml loader visitor
    - Added accessors to src and dst in Connector

### Changed

    - Execution graph : 
        - big clean-up of the code
        - use now local root 
        - better infos on cycles detection 
        - added important comment in code
    - Containers now use a not altered children vector in case of movechild has been use
    - Process_handler : add a size field in ProcessCollector.
    - window : new work on window transparency background for each plateform
    - layer : x, y, w, h and pad are now int
    - dump function, now dump children in order
    - main_loop: join threads when exiting
    - Improved cycle analysis display
    - Improved debug infos.
    - Cleaned Code , format and reorganizing header
    - Improved support for mold.
    - Improved compilation time.
    - Improved string management
    - Reformat Changlog
    - ubuntu 22.04 is now the main reference for package name
    - improved script : gen_prop.py

### Deprecated

    - Process : schedule_delete --> schedule_deletion
    - All components : state_dependency() -> get_state_dependency()
    - All components : Parent/FatChildProcess -> CoreProcess

### Removed

    - Removed cycle_analysis for djnn execution initialization phase.
    - Removed old execution loop from the code.

### Fixed

    - Fixed layer position when cropped.
    - Fixed issues with remove_state_dependency.
    - Fixed Multi-Connector/assignment.
    - Fixed clock: period is now > 0.
    - Fixed the option DJNN_NO_DEBUG to compile again.
    - Fixed clone function for all components using properties.
    - Fixed Bison warnings.
    - Fixed mouse wheel event API.
    - Fixed ambiguous string use.
    - Fixed color computation with 3 bytes.
    - Fixed frame/background_color change.

## [1.19.0] - 2023-01-26

### NEW

    - NEW z_order management for graphical shapes.
    - NEW Wheel has now a position x, y
    - NEW support for Mold

### Added

    - Added is_model (=false) for component and container to start inactived
    - Added missing clone function on Component: Text, Deref, timer, binding, assignmentSequence, assignment 
    - Added a new constructor for dashArray 
    - Added deb_git target in Makefile for daily deb packaging

### Changed

    - make sure that src of coupling is nullptr when about_to_detete_src
    - Abstract LayerCache
    - Fixed several memory leaks on several component
    - Fixed issue  in DashArray
    - Fixed draw method called on Path.
    - Fixed missing activation in text_field
    - Fixed clipping issue with text
    - correctly added "format" property to data_image through auto-generation file
    - Change managing of Parentless_name and fixed deletes 
    - Assignments and Connetor are now templated
    - Clips are now IntersectClip instead of ReplaceClip
    - Updated link to remotery
    - improved component: multiline_edit
    - Improved Makefile
    - Improved Code and typo.

### Deprecated

    - deprecated LoadFromXML and add new load_from_XML (which load then clone) and load_from_XML_once (only load once)

## [1.18.0] - 2022-06-09

### NEW

    - NEW LazyConnector component : if the value did change -> don't raise any event.
    - NEW LazyAssignment component : if the value did change -> don't raise any event.
    - NEW enum type for font weight : DJN_LIGHT, DJN_NORMAL, DJN_DEMI_BOLD, DJN_BOLD, DJN_BLACK
    - NEW add getset macros : GET_CHILD, GET_CHILD_VALUE, SET_CHILD_VALUE (see: src/core/utils/getset.h)
    - NEW a generic keyboard component
    - NEW Makefile: add ninja support

### Added

    - Added new experimental mode - _AUTHORIZE_CYCLE
    - Added support to image raw data.
    - Added Qtablet events management
    - Added remotery sampling in text drawing
    - Added a Clear spike on MultilineEdit componant

### Changed

    - Makefile: reorganize pch
    - Compute bbox of graphical shapes only if a value changed
    - Layers auto re-display when their children changed
    - Rearrangement of source files
    - Made "ms" optional for wallclock
    - Improved debug information on CoreAssignment componant.
    - Removed accumulation on Move Events (mouse and touch)
    - Scheduled activation for all needed mouse/touch events.
    - Remade wheel events computation and added to graphical shapes
    - Improved debug messages.
    - Cleaned Code, comments

### Fixed

    - Fixe a bug on find_chil_impl for djnn::List getting parent eg: ../foobar
    - Fixed clean_up_content on djnn::List
    - Fixed fill color alpha management
    - Fixed a bug in graph cycles detection
    - Fixed bug with empty image

## [1.17.0] - 2022-01-05  

### NEW

    - NEW IMPORTANT : 
        - new GUIStructureHolder management system (change renderer strategy)
        - new sort management on execution graph : to favor local micro sort, no sort after removing edges 
    - NEW Componant :
        - new Previous componant for double, int, bool and text
        - new deref for bool and int
    - NEW child on Wallclock :
        - Add format eq: %Y-%m-%d_%Hh%Mm%Ss
    - NEW child for qt Window :
        - Added frame.background_rect to subscribe directly to background without adding a "fake" rectangle
        - Frameless option on qt window with window.frameless = TRUE | FALSE
        - Background_capacity setting appart from brackground_color (works badly on Linux and Windows)
        - Fixed window default background_color as macOS dark mode (50, 50, 50)
        - Moved UI and PickUI classes into display
    - NEW child on GUI componant :
        - Added PickFill, PickOutline
        - Added NoPickFill, NoPickOutline
    - NEW child on Layer :
        - Added Padding
    - NEW on system :
        - Made Ivy optionnal to facilitate first-time compiling experience   

### Added

    - Added find_optional_child with no "not found" msg
    - Added is_model as an optional argument for Timer
    - Added min_width and min_height properties for Window
    - Added ascent/descent properties to gui text
    - Added support to QOpenGLWidget
    - Improved code coverage.

### Changed

    - Reordered GUI event to improved performence.
    - Removed possible multiple parenthood in list
    - Improved performence on qt_picking_view
    - Improved color_map management
    - Improved Image Renderer
    - Improved support of Layer in GUI
    - Improved XML parsing/loading
    - Improved remoteProperty
    - Improved remotery monitoring Informations 
    - Improved use of qpainter composition_mode with Layer
    - Improved Picking management
    - Auto generate missing id for all SVG elements on parsing
    - Improved code and include.
    - Improved MSYS2/mingw64 compatibility, dependency and instructions
    - Improved Install.md
    - Cleaned Code, comments

### Fixed

    - Fixed execution cycle on screenshot componant
    - Fixed NativeAsyncAction destructor
    - Fixed update layer in picking view
    - Fixed Leaks 

## [1.16.0] - 2021-07-28

### NOTE

    - djnn is still using Qt5. Not compatible with Qt6 yet.

### NEW

    - New Component: 
        - VoidProcess
        - SimpleTextEdit, SimpleText
        - DirectoryObserver
        - LazyAssignment : do not trigger if the new value is the same old value.
    - New execution engine :
        - using deque instead of vector
        - respecting order
        - More sensitive to loops on dataflow but also detect them.
        - A lot more efficient: time, and memory. Execute only what it should
        - Moved _outputs_nodes outside activation loop
        - Old system still exist : _EXEC_FULL_ORDERED_VERTICES = 1
    - New debug options that can be called directly from code :
        - _DEBUG_SEE_COLOR_PICKING_VIEW
        - _DEBUG_GRAPH_CYCLE_DETECT
        - _DEBUG_SEE_ACTIVATION_SEQUENCE
        - _DEBUG_SEE_ACTIVATION_SEQUENCE_TARGET_TIME_US
        - _DEBUG_SEE_ACTIVATION_SEQUENCE_ONLY_TARGETED
    - New Clone API (including complexe component : connector) but implementation for all component other thatn GUI is not complet yet.
    - New Added vscode compatibility
    - New compatibility with C++17
    - New optimise version of assignment 'perform_action' function
    - New eastl support (optionnal)

### Added

    - Incr Component : add a 'step' child
    - Added setfd option on iofd
    - Added background color and opacity on window/frame : background_opacity and background_color
    - Improved remotery support
    - Improved Graph debug info : 
    - Added tools/sdl-drmkms-install.md
    - Added a function to check the order between Processes : graph_check_order

### Changed

    - Improved code and conception of Assignment.
    - Changed on add/remove edge : 
        - Now coupling manages the edge addition/removal in graph 
        - Removed no more necessary add/remove_edge instructions
        - Add a dedicated component to create/delete graph edges
        - NO longer use immediate coupling
    - Changed deactivating instead of DEACTIVATION in actions.
    - Improved code / clean up code.
    - Enabled the isModel argument for Spike
    - Changed and added graph edges between an AssignmentSequence and the destination of its children Assignment
    - Improved package system building : brew, deb ,pkg
    - Improved code on DataImage

### Removed

    - Removed event synthesis on press, release and wheel but keep it for move, close or paint
    - Disabled the possibility to have multiple activation for a process through the activator
    - Removed explicite path to moc in Makefile to be sur to use the correct one (qt5 vs qt6)
    - Removed all using namespace 'std' to prepare for 'stl' implementation swap

### Fixed

    - Fixed FSM : 
        - semantics
        - possible concurrency issue between transitions
        - state are not (de)activate directly in transition action, let the execution graph do the job
    - Fixed color_picking map management.   
    - Fixed Dictonary :  inhibit find when add is triggered
    - Fixed a bug on find_child_impl for Abstract_gshape with ui
    - Fixed ms padding in Wallclock
    - Fixed isModel initialisation
    - Fixed window initialisation.
    - Fixed ordering issue in Finder component.
    - Fixed name on ClampMin/ClampMax.
    - Fixed a touch issue with some trackpads.
    - Fixed code coverage 
    - Fixed dev dependencies and pkgconfig file generation
    - Fixed compilation with qt5 on macosx
    - Fixed SDL Compilation
    - Fixed typo and comments

## [1.15.0] - 2021-01-28

### NEW

    - Lots of works to reduce compilation time : clean headers, precompiled headers, dependencies.
        BECAREFUL: developer has to be more precise on includes declaration
    - Added script to check sources and headers

### Added

    - Added init_xxx and clear_xxx module missing.
    - Added SYNCHRONIZER_T type
    - Added dependence to libgbm (kmsdrm driver) on linux x86
    - Added a missing graph edge in list operators 

### Changed

    - Fixed leaks on display module.
    - Removed move event on press event.
    - Improved dump of : set component.
    - Improved warning messages.
    - Improved clear parentless_names.
    - Improved global memory performances : DJNN_NO_DEBUG, _SERIALIZE
    - Cleaning native_async_action
    - Updated gen_prop script.
    - Renamed djnParseXML -> djnParseXMLFromPath
    - Improved Makefile : add compiler option
    - Separate regex from text.h into regex.h
    - Modularize socket from multi-platform

### Removed

    - Removed prototype of c api.

### Fixed

    - Fixed bug on merging gradient from svg.
    - Fixed event on liux_mouse
    - Fixed SVG parser leaks
    - Fixed IvyAccess leaks on cleaning.
    - Fixed SDL/DRM compilation. 
    - Fixed text geometry/size calculation
    - Fixed a bug in Container::move_child - AFTER

## [1.14.0] - 2020-11-23

### NEW

    - package for windows (and Linux) are avaluable on : https://github.com/lii-enac/djnn-cpp/releases
    - NEW support for layers on GUI backend
    - NEW component WallClock giving time (state) in ms from 01/01/1970 or string (state_text) %Y-%m-%d_%Hh%Mm%Ss format 
    - NEW support for screenshot from window/frame
    - NEW support for window/frame opacity
    - NEW support for window/frame geometry
    - NEW Componant Remote_process to conect to a remote application
    - NEW Componant process_exporter to export part of an application

### Added

    - Added refreshed (blank spike) to windwo/frame
    - Added better handling of audio properties : loop, gain, position, pitch to sound and real-time control of parameters
    - Audio : prepare for mp3 and flac
    - Added the possibility to change the initial state of a FSM before the FSM activation
    - Added silent "-s" option on Makefile 
    - Added bounding_box on path
    - Added warning messages on process::find_child() function else use process::find_child_impl() or not debug mode
    - Added transform method to all transformations

### Changed

    - STD thread are now the standart for all the plateforms
    - Improved physics module 
    - Refreshed informations on INSTALL.md 
    - Improved (better scheme) and clean-up Makefile
    - Clean-up code using tidy

### Fixed

    - Fixed data-race using sanatizer -thread option.
    - Fixed several leaks using sanatizer -leaks -address options
    - Fixed process state_dependency function on cleanings
    - Fixed deref immediate coupling, check on new source, graph cleaning and update source on start
    - Fixed a bug (data-race) in time manager 
    - Fixed window position on cairo/sdl backend
    - Fixed some free/delete call
    - Fixed comms compilation on windows
    - Fixed clear_exec_env

## [1.13.0] - 2020-10-07

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
    - Fixed SDL Keycode
    - Fixed Makefile not quitting on some compilation

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
