# Drum machine

Basic drum machine written in C++ using iimavlib library.

## How to use

These instructions will show you how to start and use this program.

### Synopsis

Device id has to be specified as first argument. If you want to use default device, use "DEFAULT" keyword.

_./app <device id|DEFAULT> [wav files or directories]_

### Examples

```
Launch with default device and without any wav file:
./app DEFAULT
```

```
Launch with device having id = "hw:0,0" and with two wav files:
./app "hw:0,0" ./wav/kick.wav ./wav/snare.wav
```

```
Launch with default device and with directory, containing several wav files:
./app DEFAULT ./wav
```

### Interface

