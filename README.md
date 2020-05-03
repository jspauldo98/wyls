# wyls

wyls is a utility program that mimics the Linux ls utility. It is designed to list directory contents. This is a project for Linux Programming (COSC 3750).

## Prerequisites

GNU make is required.

## Installation

Use the GNU make utility to compile.

```bash
make
```

## Synopsis

```bash
./wycat [OPTION]... [FILE]...
```

## Description

List information about the FILEs (the current directory by default). Listing is automatically formatted as long listing.

### Options

-h, human readable size
-n, list numeric user and group IDs

## Examples
```bash
./wyls 
    # Write to standard output current directory contents

./wyls -nh dir 
    # Output dir's contents with human readable size and numeric user 
    # and group IDs 
```

## Author

Jared Spaulding
