# Fox
Fox is a x86_64 kernel and OS developed mainly by jonay2000 and with some help by Sarah.

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

Then, the ISO file will be in `build/` under the file name `fox-version-arch.iso`!
