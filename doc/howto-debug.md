# How to debug djnn-cpp applications

set to 1 those variables in your code:
```
_DEBUG_SEE_ACTIVATION_SEQUENCE_2 = 1
_DEBUG_SEE_PROP_SET_VALUE = 1
````

## in smala
```
_main_
Component root {
    _DEBUG_SEE_ACTIVATION_SEQUENCE_2 = 1
    _DEBUG_SEE_PROP_SET_VALUE = 1
    ...
```

or to toggle with `Cmd-D`:

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
