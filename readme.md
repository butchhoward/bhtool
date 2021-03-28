# BHTool - Used to Bash Scripts, Now are C++ Binaries <!-- omit in toc -->
==============

Using a collection of bash scripts I use frequently as the basis for a project to refresh my C++ skills and skills around creating command line utilities.


- [Build Process](#build-process)
  - [Install Dependencies](#install-dependencies)
  - [Build](#build)
- [Usage](#usage)
- [Commands](#commands)
  - [`stderrred <cmd> [args...]`](#stderrred-cmd-args)


## Build Process

### Install Dependencies

Using homebrew for now...

```shell
brew install libgit2
brew install curl
```

Assuming xcode is around

```shell
xcode-select --install
```

### Build

```shell
mkdir build && cd build && cmake ..
make all test
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



