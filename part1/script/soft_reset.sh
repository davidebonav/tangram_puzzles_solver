#!/bin/bash

# Load config values
# source ./project.conf

sudo -u postgres psql -d tangram_puzzle -c "DROP TABLE IF EXISTS solutions;"
sudo -u postgres psql -d tangram_puzzle -c "DROP TABLE IF EXISTS puzzles;"
sudo -u postgres psql -d tangram_puzzle -c "DROP TABLE IF EXISTS pieces;"
