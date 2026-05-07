# How to debug djnn-cpp applications

## compile errors with correct line number in smala code

In `smala`'s or your application's `config.mk`, add `-g` to `SMAFLAGS` to generate `#line` directives in the generated `.cpp` files and get from the compiler the line in the smala code that the compiler finds erroneous. 

## debugger during running time in cpp

To use a debugger and breakpoints:
- in `djnn-cpp`'s `config.mk`, add `-g` and `-O0` to `CFLAGS`
- set `use_pch := no` to keep some name information (if the debugger  complains with `summary unavailable` when browsing structures).
- if you want to break into generated `.cpp` files from `.sma` files, do *not* add `-g` to `SMAFLAGS` (as adding `-g` will change the `#line` directives and prevent the debugger to find the correct line of `cpp` ,source code). Unfortunately this temporarily breaks the correct display of the line number from the compiler when a specific smala line is erroneous...


## activation sequence in cpp

To see the activation sequence, set to `1` the following variables in your `cpp` code:
```
_DEBUG_SEE_ACTIVATION_SEQUENCE_2 = 1
_DEBUG_SEE_PROP_SET_VALUE = 1
```

## activation sequence in smala

You can set the same variables in a smala program like this:

```
_main_
Component root {
    _DEBUG_SEE_ACTIVATION_SEQUENCE_2 = 1
    _DEBUG_SEE_PROP_SET_VALUE = 1
    ...
```

Or, to toggle with `Cmd-D` (to see the activation sequence starting from a specific state of your application):

```
import gui.keyboard.ControlKey

    ...
    ControlKey ckd (f, DJN_Key_D)
    ckd.press -> (root) {
        if (_DEBUG_SEE_ACTIVATION_SEQUENCE_2) {
            _DEBUG_SEE_ACTIVATION_SEQUENCE_2 = 0
            _DEBUG_SEE_PROP_SET_VALUE = 0
        }
        else {
            _DEBUG_SEE_ACTIVATION_SEQUENCE_2 = 1
            _DEBUG_SEE_PROP_SET_VALUE = 1
        }
    }
    ...
```

##
