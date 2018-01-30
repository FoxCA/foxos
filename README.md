# Fox
Fox is a x86_64 kernel and OS developed mainly by jonay2000 and with some help by Sarah.

## To-Do List
A public to-do list can be found on [Trello](https://trello.com/b/Ew3kIkTE).

## Running it

Dependencies (Only \*nix systems, includes macOS, Windows will follow eventually.)
- nasm
- qemu\*
- VirtualBox\*

\* qemu and VirtualBox are optional, you can run it in any emulator or install it on your computer, see [Making the ISO](#making-the-iso) for more information.

### Clone the repository

```bash
$ git clone https://github.com/jonay2000/JOS
```

### Running Fox with qemu

To run the release version:

```bash
$ make runrel
```

If you want to develop low-level things, you might want to run the no-reboot version:

```bash
$ make run
```

### Running it with VirtualBox

Just execute

```bash
$ make runv
```

### Making the ISO

To get the ISO, run

```
$ make ISO
```

Then, the ISO file will be in `build/` as `fox-version-arch.iso`! You won't be able to install or run Fox on an actual computer yet, even though it wouldn't be of much use if you did. Running the ISO on a computer needs a filesystem initialization, even if you don't need it. If you take a look at the [Trello](https://trello.com/b/Ew3kIkTE), you'll see that we're working on the file system. When that's done, you can properly install the OS, but until then you should stick to virtual machines.

TL;DR: Run Fox on a virtual machine for now.
