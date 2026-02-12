# linux-resource-monitor

## Linux Kernel Module
This module registers a virtual proc file. When a user reads that file, the kernel invokes my callback functions, which retrieves per CPU stats from internal kernel structures and returns
formatted output using seq_file interface. The module can be dynamically loaded and removed without affecting the running system.
