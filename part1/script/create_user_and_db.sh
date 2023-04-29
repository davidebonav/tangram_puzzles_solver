#!/bin/bash

# cd ./part1/

# Load config values
# source ./project.conf
. ./project.conf

sudo -u postgres psql  -c "DROP DATABASE IF EXISTS $DB_NAME"
sudo -u postgres psql  -c "DROP USER IF EXISTS $USER"
sudo -u postgres psql  -c "CREATE DATABASE $DB_NAME"
sudo -u postgres psql  -c "CREATE USER $USER WITH ENCRYPTED PASSWORD '$PASSWORD'"
sudo -u postgres psql  -c "GRANT ALL PRIVILEGES ON DATABASE $DB_NAME TO $USER"
# sudo -u postgres psql  -c "\du"
# sudo -u postgres psql  -c "\l"