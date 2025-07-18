#!/bin/bash
# By Pytel

# colors
Red='\033[0;31m'          # Red
Green='\033[0;32m'        # Green
Blue='\033[0;34m'         # Blue
Yellow='\033[0;33m'       # Yellow
Magenta='\033[0;35m'      # Magenta
Cyan='\033[0;36m'         # Cyan
NC='\033[0m'    # No Color

$apt_dependencies="dependencies.txt"

# Install apt dependencies
echo -e "${Cyan}Installing apt dependencies...${NC}"
sudo apt-get update
if [ ! -f $apt_dependencies ]; then
    echo -e "${Red}Error: $apt_dependencies file not found!${NC}"
    exit 1
fi
xargs sudo apt-get -y install < $apt_dependencies
echo -e "${Green}Apt dependencies installed successfully!${NC}"