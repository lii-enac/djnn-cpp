# How to fix performance issues with djnn apps

## Run-time performance

### using remotery
In `config.mk`, set:
```
remotery_cflags := -DRMT_ENABLED=1
```
then recompile `djnn-cpp` entirely.


Open `Remotery/vis/index.html` in a browser, launch your app and see what's taking time (especially rendering).

If drawing takes time:
- consider using `Layer`s (put backgound drawings that do not change often into a pixmap)
- TODO

### using a debugger
Interact with your app to reproduce the slow behavior and break using `CTRL-C`. Most of the time the program will break in the most costly function.

### using a flamegraph (useful for startup time)
`git clone --depth https://github.com/brendangregg/FlameGraph`

edit `fg.sh`:
```
dtrace -x ustackframes=100 -n 'profile-10ms /execname == "radar_image_ATCO"/ { @[ustack()] = count(); } tick-20s { exit(0); }' \
| tee ~/tmp/radar_image_ATCO_sample.txt \
| ../../misc/FlameGraph/stackcollapse.pl \
| ../../misc/FlameGraph/flamegraph.pl \
> ~/tmp/radar_image_ATCO_sample.txt.svg
````

then for each try:
1. launch `fg.sh` in the background:
````
sudo sh fg.sh
(passwd)
CTRL-Z
bg
````

2. then launch your app
```
make -j ... (lancer radar_image_ATCO)
```
3. interact with it
4. stop it
5. see the results:
```
open -a firefox ~/tmp/radar_image_ATCO_sample.txt.svg
```


## Compile-time performance

Use `-O0` and remove `-g` to get the fastest compile time.

To measure compile time:
````
CFLAGS += -ftime-trace
````

1. compile with no parallelism (no -j for make)
2. load any generated .json file in chrome://tracing/
and see what's taking time to compile, fix it (e.g. remove #include stuff), compile again, and reload the .json file


To find out which files take the most time:

1. `brew install jq`

2. edit jq_script:
```
#!/bin/sh
set -o errexit
time=$(jq -c '.traceEvents[] | select(.name | contains("Total ExecuteCompiler")) | .dur' "$1";)
printf '%s %s\n' "$time" "${1%*.json}";
```
```
chmod u+x jq_script
```

3. then after each compilation:
```
find build/*/src -name '*.json' -not -name "*.cccmd.*" -type f -exec ./jq_script {} \; | sort --numeric-sort;
```
or better:
```
(pushd build/*/src && find * -name '*.json' -not -name "*.cccmd.*" -type f -exec ../../../jq_script {} \; | sort --numeric-sort;)
```


