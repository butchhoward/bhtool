# BHTool - Used to Bash Scripts, Now are C++ Binaries
==============

Using a collection of bash scripts I use frequently as the basis for a project to refresh my C++ skills and skills around creating command line utilities.


```shell
mkdir build && cd build && cmake ..
make CTEST_OUTPUT_ON_FAILURE=1 all test
```

## Usage

```
bhtool <command> [<sub-command>] [options]
```

## Commands

### `stderrred <cmd> [args...]`

Execute `<cmd> [args...]`

Color all `stderr` output <span style="color:red">RED</span>.

Leave all `stdout` output the default color.



