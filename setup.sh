#!/bin/bash
sudo ethtool -L enp8s0f0 combined 8
sudo bash -c "echo 256 > /sys/class/net/enp8s0f0/busy_budget"
sudo bash -c "echo 2 > /sys/class/net/enp8s0f0/napi_defer_hard_irqs"
sudo bash -c "echo 200000 > /sys/class/net/enp8s0f0/gro_flush_timeout"
