#!/bin/bash

# Load config values
# source ./project.conf
. ./project.conf

# 1. Check if PostgreSQL is installed and install it if it's not
if ! command -v psql &> /dev/null
then
    echo "PostgreSQL is not installed. Installing..."
    sudo apt-get update
    sudo apt-get install postgresql postgresql-contrib -y
    echo "PostgreSQL installed successfully."
else
    echo "PostgreSQL is already installed."
fi

# 2. Check if the PostGIS extension is installed and install it if it's not
if ! sudo -u postgres psql -c "SELECT postgis_full_version();" &> /dev/null
then
    echo "PostGIS extension is not installed. Installing..."
    sudo apt-get install postgis -y
    sudo apt-get install postgresql-15-postgis-3 -y
    echo "PostGIS installed successfully."
else
    echo "PostGIS extension is already installed."
fi

# 3. Check if the database exists and drop it if it does
if sudo -u postgres psql -lqt | cut -d \| -f 1 | grep -qw $DB_NAME
then
    echo "Database $DB_NAME exists. Dropping..."
    sudo -u postgres dropdb $DB_NAME
    echo "Database $DB_NAME dropped successfully."
else
    echo "Database $DB_NAME does not exist."
fi

# 4. Check if the user exists and drop it if it does
if sudo -u postgres psql -c "SELECT 1 FROM pg_roles WHERE rolname='$USER'" | grep -q 1
then
    echo "User $USER exists. Dropping..."
    sudo -u postgres dropuser $USER
    echo "User $USER dropped successfully."
else
    echo "User $USER does not exist."
fi

# 5. Create a new database if it doesn't exist
if ! sudo -u postgres psql -lqt | cut -d \| -f 1 | grep -qw $DB_NAME
then
    echo "Creating database $DB_NAME..."
    sudo -u postgres createdb $DB_NAME
    echo "Database $DB_NAME created successfully."
else
    echo "Error: database $DB_NAME already exists."
    exit -1
fi

# 6. Create a new user if it doesn't exist
if ! sudo -u postgres psql -c "SELECT 1 FROM pg_roles WHERE rolname='$USER'" | grep -q 1
then
    echo "Creating user $USER..."
    sudo -u postgres psql -c "CREATE USER $USER WITH PASSWORD '$PASSWORD';"
    echo "User $USER created successfully."
else
    echo "Error: user $USER already exists."
    exit -1
fi

# 7. Grant all privileges on the database to the user
echo "Granting all privileges on DATABASE $DB_NAME to user $USER..."
sudo -u postgres psql -c "GRANT ALL PRIVILEGES ON DATABASE $DB_NAME TO $USER;"
echo "All privileges granted successfully."

# 8. Enable the PostGIS extension on the database
echo "Enabling PostGIS extension on database $DB_NAME..."
sudo -u postgres psql -d $DB_NAME -c "CREATE EXTENSION IF NOT EXISTS postgis;"
echo "PostGIS extension enabled successfully."

# 9. Print the PostGIS version to verify that the extension is enabled
echo "PostGIS version:"
sudo -u postgres psql -d $DB_NAME -c "SELECT PostGIS_version();"

# sudo -u postgres psql -c "\du"
# sudo -u postgres psql -c "\l"