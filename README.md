# Linux Resource Monitor - Kernel Module

A simple Linux kernel module project to learn how kernel modules work and retrieve system statistics directly from the kernel.

## What is a Kernel Module?

A **kernel module** is a piece of code that can be loaded into the Linux kernel without rebooting. It runs with full system privileges and can access hardware and internal kernel data structures.

Think of it like a plugin for your operating system - you can add features to the kernel dynamically.

## What This Project Does

This kernel module creates a file at `/proc/resource_monitor` that displays real-time CPU statistics for each CPU core in your system:

- **User Time**: Time spent running user programs
- **Nice Time**: Time spent running low-priority programs
- **System Time**: Time spent in kernel operations
- **Idle Time**: Time when CPU was idle
- **IOWait Time**: Time waiting for I/O operations (disk, network)
- **IRQ Time**: Time handling hardware interrupts
- **SoftIRQ Time**: Time handling software interrupts

## Prerequisites

You need a Linux environment with kernel headers installed:

### For Ubuntu/Debian (including WSL):
```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
```

### For Fedora/RHEL:
```bash
sudo dnf install kernel-devel kernel-headers gcc make
```

## Building the Module

1. Navigate to the kernel_module directory:
```bash
cd kernel_module
```

2. Build the module:
```bash
make
```

This will create a file called `resource_monitor.ko` (kernel object).

3. To clean build artifacts:
```bash
make clean
```

## Loading and Testing the Module

### Load the module:
```bash
sudo insmod resource_monitor.ko
```

### Verify it's loaded:
```bash
lsmod | grep resource_monitor
```

### View the CPU statistics:
```bash
cat /proc/resource_monitor
```

You should see output like:
```
Linux Kernel Resource Monitor
---------------------------------
CPU 0:
  User:    12345678
  Nice:    0
  System:  9876543
  Idle:    98765432
  IOWait:  12345
  IRQ:     4567
  SoftIRQ: 8901

CPU 1:
  ...
```

### Check kernel logs (to see module messages):
```bash
dmesg | tail
```

You should see messages like:
```
Resource Monitor Module Loaded
```

### Unload the module:
```bash
sudo rmmod resource_monitor
```

Verify it's unloaded:
```bash
dmesg | tail
```

You should see:
```
Resource Monitor Module Unloaded
```

## Project Structure

```
linux-resource-monitor/
├── kernel_module/
│   ├── resource_monitor.c    # Main kernel module code
│   └── makefile              # Build configuration
├── .gitignore                # Ignore build artifacts
└── README.md                 # This file
```

## How the Code Works (Simple Explanation)

1. **Module Init** (`resource_init`):
   - Runs when you load the module with `insmod`
   - Creates the `/proc/resource_monitor` file
   - Prints "Module Loaded" to kernel log

2. **Reading the File** (`resource_show`):
   - Runs when you `cat /proc/resource_monitor`
   - Loops through each CPU core
   - Reads CPU time statistics from kernel's internal data
   - Formats and displays the information

3. **Module Exit** (`resource_exit`):
   - Runs when you unload with `rmmod`
   - Removes the `/proc/resource_monitor` file
   - Prints "Module Unloaded" to kernel log

## Key Kernel Concepts Used

- **proc filesystem**: Virtual filesystem to expose kernel data to userspace
- **seq_file interface**: Efficient way to generate output for /proc files
- **kernel_cpustat**: Kernel structure holding CPU time accounting
- **module_init/module_exit**: Macros defining entry/exit points

## Troubleshooting

**Module won't load:**
- Make sure you built it for your current kernel version
- Check `dmesg` for error messages
- Verify kernel headers are installed: `ls /lib/modules/$(uname -r)/build`

**Permission denied:**
- You need sudo/root privileges to load/unload modules

**make command not found:**
- Install build-essential tools (see Prerequisites)

## Learning Resources

- [Linux Kernel Module Programming Guide](https://sysprog21.github.io/lkmpg/)
- [The Linux Kernel Documentation](https://www.kernel.org/doc/html/latest/)
- [/proc filesystem documentation](https://www.kernel.org/doc/html/latest/filesystems/proc.html)

## License

GPL (required for all kernel modules)

## Author

Your Name
