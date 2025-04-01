# ft_malcolm - ARP Spoofing Tool

## Overview
**ft_malcolm** is a tool that performs ARP spoofing by intercepting ARP requests and sending fake replies to modify the target's ARP table. It is designed for educational and security research purposes.

## Features
- **Spoofs ARP tables** by responding to ARP requests with fake MAC addresses.
- **Handles IPv4 addresses**.
- **Exits cleanly** when interrupted with `Ctrl+C`.
- **Uses minimal dependencies**, relying only on C and `libft`.

## Installation
### Prerequisites
- **Linux** system
- **Docker** installed and running

### Build
```sh
make
```
This compiles the program and `libft`.

### Clean Up
```sh
make fclean
```
Removes the compiled files.

## Usage
Run the program with `sudo`:
```sh
sudo ./ft_malcolm <source_ip> <source_mac> <target_ip> <target_mac>
```

### Example
```sh
sudo ./ft_malcolm 172.18.0.55 ab:ab:ab:ab:ab:ac 172.18.0.2 02:42:ac:12:00:02
```
This will:
1. Wait for an ARP request from `172.18.0.2`.
2. Respond with a spoofed MAC address `ab:ab:ab:ab:ab:ac` for IP `172.18.0.55`.
3. Update the ARP table on the target.

## Running in Docker
### Start Docker
```sh
sudo systemctl start docker
sudo systemctl enable docker
```

### Run ft_malcolm
```sh
sudo ./ft_malcolm 172.18.0.55 ab:ab:ab:ab:ab:ac 172.18.0.2 02:42:ac:12:00:02 -ibr-48095d7f25f3 -v
```
This runs the program in verbose mode with a specific network bridge.
