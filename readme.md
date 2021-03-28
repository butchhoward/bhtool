# BHTool - Used to Bash Scripts, Now are C++ Binaries <!-- omit in toc -->
==============

Using a collection of bash scripts I use frequently as the basis for a project to refresh my C++ skills and skills around creating command line utilities.


- [Build Process](#build-process)
  - [Install Dependencies](#install-dependencies)
  - [Build](#build)
  - [Build Notes](#build-notes)
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

### Build Notes

It took quite a bit of digging on the Googles to figure out the linking options needed to be able to compile and link with `libgit2`. The thing I found the made it work was using the `-framework Security` and `-framework CoreFoundation`. This is likely not portable, but I'll worry about that when I need to partage this canoe. I came to this point after also trying to link with the OpenSSL libraries, which did not get all the things needed. This works and I'll run with it until I find a better way or a problem.

I rolled all of this finding of the things into some cmake modules for the `find_package` command. It might be mostly wrong, but it seems to work for now.

`libgit2` seems a pretty complete and well supported git library. It is a `C` library, but has done the good things to be friendly in a `C++` project. It does bring in some other dependencies to provide the SSH, SSL, Crypto, compression, and unicode support

#### References <!-- omit in toc -->

[libgit2](https://libgit2.org/)
[Someone that made libgit2 work on macos](https://bvisness.me/libgit2/)

## Usage

```
bhtool <command> [<sub-command>] [options]
```

## Commands

### `stderrred <cmd> [args...]`

Execute `<cmd> [args...]`

Color all `stderr` output <span style="color:red">RED</span>.

Leave all `stdout` output the default color.



